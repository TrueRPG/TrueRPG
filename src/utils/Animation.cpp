#include "../pch.h"
#include "Animation.h"

namespace
{
using ValueType = std::variant<bool, float, glm::vec2>;

struct Expression
{
    virtual ValueType evaluate(const SpriteAnimatorParameterStorage &storage) = 0;
    virtual ~Expression() = default;
};

struct FloatExpression : Expression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override { return value; }

    float value{};
};

struct ParameterExpression : Expression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        return std::visit([](auto value) { return value; }, storage.at(name));
    }

    std::string name;
};

struct AttributeExpression : Expression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        auto leftValue = left->evaluate(storage);

        return std::visit(
            [&](auto value)
            {
                if constexpr (std::is_same_v<glm::vec2, decltype(value)>)
                {
                    if (right == "x")
                    {
                        return value.x;
                    }

                    if (right == "y")
                    {
                        return value.y;
                    }
                }

                return 0.f;
            },
            leftValue);
    }

    std::shared_ptr<Expression> left;
    std::string right;
};

struct BinaryExpression : Expression
{
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
};

struct LtExpression : BinaryExpression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        return std::get<float>(left->evaluate(storage)) < std::get<float>(right->evaluate(storage));
    }
};

struct GtExpression : BinaryExpression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        return std::get<float>(left->evaluate(storage)) > std::get<float>(right->evaluate(storage));
    }
};

struct LeExpression : BinaryExpression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        return std::get<float>(left->evaluate(storage)) <= std::get<float>(right->evaluate(storage));
    }
};

struct GeExpression : BinaryExpression
{
    ValueType evaluate(const SpriteAnimatorParameterStorage &storage) override
    {
        return std::get<float>(left->evaluate(storage)) >= std::get<float>(right->evaluate(storage));
    }
};

std::shared_ptr<Expression> parseExpression(const std::set<std::string> &parameters, std::string_view type, const YAML::Node &node)
{
    if (type == "lt")
    {
        auto lt = std::make_shared<LtExpression>();
        lt->left = parseExpression(parameters, "l", node["l"]);
        lt->right = parseExpression(parameters, "r", node["r"]);

        return lt;
    }

    if (type == "le")
    {
        auto le = std::make_shared<LeExpression>();
        le->left = parseExpression(parameters, "l", node["l"]);
        le->right = parseExpression(parameters, "r", node["r"]);

        return le;
    }

    if (type == "gt")
    {
        auto gt = std::make_shared<GtExpression>();
        gt->left = parseExpression(parameters, "l", node["l"]);
        gt->right = parseExpression(parameters, "r", node["r"]);

        return gt;
    }

    if (type == "ge")
    {
        auto ge = std::make_shared<GeExpression>();
        ge->left = parseExpression(parameters, "l", node["l"]);
        ge->right = parseExpression(parameters, "r", node["r"]);

        return ge;
    }

    if (type == "attr")
    {
        auto attr = std::make_shared<AttributeExpression>();
        attr->left = parseExpression(parameters, "l", node["l"]);
        attr->right = node["r"].as<std::string>();

        return attr;
    }

    if (type == "l" || type == "r")
    {
        if (node.IsScalar())
        {
            auto value = node.as<std::string>();

            if (parameters.find(value) != parameters.end())
            {
                auto parameterExpression = std::make_shared<ParameterExpression>();
                parameterExpression->name = std::move(value);

                return parameterExpression;
            }

            auto floatExpression = std::make_shared<FloatExpression>();
            floatExpression->value = node.as<float>();

            return floatExpression;
        }

        return parseExpression(parameters, node.begin()->first.as<std::string>(), node.begin()->second);
    }

    return std::make_shared<FloatExpression>();
}

std::function<bool(const SpriteAnimatorParameterStorage &)> parseCondition(
    const std::set<std::string> &parameters, const YAML::Node &conditionNode)
{
    auto expressionType = conditionNode.begin()->first.as<std::string>();
    auto expressionNode = conditionNode.begin()->second;

    auto expression = parseExpression(parameters, expressionType, expressionNode);

    return [e = std::move(expression)](const SpriteAnimatorParameterStorage &storage) {
        return std::get<bool>(e->evaluate(storage));
    };
}

} // namespace

SpriteAnimator Animation::createAnimator(const std::function<void(SpriteAnimatorBuilder &)> &setup)
{
    SpriteAnimatorBuilder builder;
    setup(builder);

    return builder.build();
}

SpriteAnimator Animation::loadAnimatorFromFile(const char *filename)
{
    return createAnimator(
        [=](SpriteAnimatorBuilder &builder)
        {
            auto root = YAML::LoadFile(filename);

            auto nodes = root["nodes"];
            auto parameters = root["parameters"];

            std::set<std::string> parameterSet;

            for (auto p = parameters.begin(); p != parameters.end(); ++p)
            {
                auto name = p->first.as<std::string>();
                auto type = [&]() -> SpriteAnimatorParameterType
                {
                    auto type = p->second["type"].as<std::string>();

                    if (type == "vec2")
                    {
                        return SpriteAnimatorParameterType::Vec2;
                    }

                    return SpriteAnimatorParameterType::Vec2;
                }();

                parameterSet.insert(name);
                builder.parameter(std::move(name), type);
            }

            std::map<std::string, SpriteAnimatorBuilder::Node> nodesByName;

            for (auto n = nodes.begin(); n != nodes.end(); ++n)
            {
                auto name = n->first.as<std::string>();
                auto frames = n->second["frames"];

                std::vector<SpriteAnimationFrame> animationFrames;
                for (auto f = frames.begin(); f != frames.end(); ++f)
                {
                    auto rect = (*f)["rect"];

                    int left = rect["left"].as<int>();
                    int bottom = rect["bottom"].as<int>();
                    int width = rect["width"].as<int>();
                    int height = rect["height"].as<int>();

                    auto duration = [&]() -> float
                    {
                        auto duration = (*f)["duration"];

                        if (!duration.IsDefined())
                        {
                            return 0.0f;
                        }

                        return static_cast<float>(duration.as<int>()) * 1e-3f;
                    }();

                    animationFrames.push_back({IntRect(left, bottom, width, height), duration});
                }

                nodesByName.insert({name, builder.node(name, std::move(animationFrames))});
            }

            for (auto n = nodes.begin(); n != nodes.end(); ++n)
            {
                auto name = n->first.as<std::string>();
                auto transitions = n->second["transitions"];

                auto srcNode = nodesByName.at(name);

                auto entry = n->second["entry"];

                if (entry.IsDefined() && entry.as<bool>())
                {
                    builder.entry().transition(srcNode, [](const auto &s) { return true; });
                }

                for (auto t = transitions.begin(); t != transitions.end(); ++t)
                {
                    auto dstNode = nodesByName.at((*t)["to"].as<std::string>());
                    auto conditionNode = (*t)["condition"];

                    srcNode.transition(dstNode, parseCondition(parameterSet, conditionNode));
                }
            }
        });
}

void Animation::addAnimator(Entity entity, const SpriteAnimator *animator)
{
    auto &animatorComponent = entity.addComponent<SpriteAnimatorComponent>();

    animatorComponent.animator = animator;

    for (const auto &parameter : animator->parameters)
    {
        switch (parameter.type)
        {
        case SpriteAnimatorParameterType::Vec2:
            animatorComponent.parameterStorage.emplace(parameter.name, glm::vec2{});
            break;
        }
    }

    animatorComponent.activeAnimation.node = &animator->nodes.front();
    animatorComponent.activeAnimation.frame.index = 0;
    animatorComponent.activeAnimation.frame.time = 0;
}
