#ifndef RPG_SPRITEANIMATIONCOMPONENT_HPP
#define RPG_SPRITEANIMATIONCOMPONENT_HPP

#include <vector>
#include <variant>
#include <unordered_map>
#include <functional>
#include <glm/vec3.hpp>

#include "../../client/animation/SpriteAnimator.h"

struct SpriteAnimatorComponent
{
    const SpriteAnimator *animator;
    SpriteAnimatorParameterStorage parameterStorage;

    struct
    {
        const SpriteAnimatorNode *node;

        struct
        {
            std::size_t index;
            float time;
        } frame;
    } activeAnimation;
};

#endif // RPG_SPRITEANIMATIONCOMPONENT_HPP
