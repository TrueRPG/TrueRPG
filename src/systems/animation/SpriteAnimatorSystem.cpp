#include "../../pch.h"
#include "SpriteAnimatorSystem.h"

#include <algorithm>

#include "../../components/render/SpriteRendererComponent.h"
#include "../../components/animation/SpriteAnimatorComponent.h"

SpriteAnimatorSystem::SpriteAnimatorSystem(entt::registry &registry) : m_registry(registry)
{
}

void SpriteAnimatorSystem::update(float deltaTime)
{
    auto view = m_registry.view<SpriteRendererComponent, SpriteAnimatorComponent>();

    for (auto viewElement : view.each())
    {
        auto& rendererComponent = std::get<1>(viewElement);
        auto& animatorComponent = std::get<2>(viewElement);
        
        // Handle transitions

        auto transitionConditionMet = [&](SpriteAnimatorTransition *t) {
            return t->condition(animatorComponent.parameterStorage);
        };

        do
        {
            auto *fromTransitions = &animatorComponent.activeAnimation.node->fromTransitions;

            if (auto found = std::find_if(fromTransitions->begin(), fromTransitions->end(), transitionConditionMet);
                found != fromTransitions->end())
            {
                animatorComponent.activeAnimation.node = (*found)->destination;
                animatorComponent.activeAnimation.frame.index = 0;
                animatorComponent.activeAnimation.frame.time = 0;
                continue;
            }

            break;
        } while (true);

        // Handle animation frame swap

        auto &frames = animatorComponent.activeAnimation.node->animation.frames;

        auto *activeFrameInfo = &animatorComponent.activeAnimation.frame;
        auto *activeFrame = &frames[activeFrameInfo->index];

        activeFrameInfo->time += deltaTime;

        while (activeFrame->duration < activeFrameInfo->time)
        {
            float leftTime = activeFrame->duration - activeFrameInfo->time;

            activeFrameInfo->index = (activeFrameInfo->index + 1) % frames.size();
            activeFrameInfo->time = leftTime;

            activeFrame = &frames[activeFrameInfo->index];
        }

        rendererComponent.textureRect = activeFrame->rect;
    }
}
