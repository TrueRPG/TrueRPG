#ifndef RPG_SPRITEANIMATOR_HPP
#define RPG_SPRITEANIMATOR_HPP

#include <functional>
#include <variant>
#include <glm/vec2.hpp>

#include "SpriteAnimation.h"

struct SpriteAnimatorNode;

enum class SpriteAnimatorParameterType
{
    Vec2
};

struct SpriteAnimatorParameter
{
    std::string name;
    SpriteAnimatorParameterType type;
};

using SpriteAnimatorParameterVariant = std::variant<glm::vec2>;
using SpriteAnimatorParameterStorage = std::unordered_map<std::string, SpriteAnimatorParameterVariant>;

struct SpriteAnimatorTransition
{
    SpriteAnimatorNode *source;
    SpriteAnimatorNode *destination;
    std::function<bool(const SpriteAnimatorParameterStorage &)> condition;
};

struct SpriteAnimatorNode
{
    std::string name;
    SpriteAnimation animation;
    std::vector<SpriteAnimatorTransition *> fromTransitions;
    std::vector<SpriteAnimatorTransition *> toTransitions;
};

struct SpriteAnimator
{
    std::list<SpriteAnimatorNode> nodes;
    std::list<SpriteAnimatorTransition> transitions;
    std::list<SpriteAnimatorParameter> parameters;
};

class Animation;

class SpriteAnimatorBuilder
{
public:
    class Node
    {
    public:
        void transition(Node &destination, std::function<bool(const SpriteAnimatorParameterStorage &)> condition);

    private:
        friend SpriteAnimatorBuilder;

        SpriteAnimator *m_animator;
        SpriteAnimatorNode *m_node;

        Node(SpriteAnimator *animator, SpriteAnimatorNode *node);
    };

    class Parameter
    {
    public:
        template<class T>
        const T &get(const SpriteAnimatorParameterStorage &storage) const
        {
            return std::get<T>(storage.at(m_parameter->name));
        }

    private:
        friend SpriteAnimatorBuilder;

        SpriteAnimator *m_animator;
        SpriteAnimatorParameter *m_parameter;

        Parameter(SpriteAnimator *animator, SpriteAnimatorParameter *parameter);
    };

    Node entry();

    Node node(std::string name, std::vector<SpriteAnimationFrame> frames);

    Parameter parameter(std::string name, SpriteAnimatorParameterType type);

private:
    SpriteAnimatorBuilder();

    friend Animation;

    SpriteAnimator m_animator;

    SpriteAnimator build();
};

#endif // RPG_SPRITEANIMATOR_HPP
