#include "Animation.h"

SpriteAnimator Animation::createAnimator(const std::function<void(SpriteAnimatorBuilder &)>& setup)
{
    SpriteAnimatorBuilder builder;
    setup(builder);

    return builder.build();
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
