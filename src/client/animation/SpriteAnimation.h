#ifndef RPG_SPRITEANIMATION_HPP
#define RPG_SPRITEANIMATION_HPP

#include <vector>

#include "../graphics/Rect.h"

struct SpriteAnimationFrame
{
    IntRect rect;
    float duration;
};

struct SpriteAnimation
{
    std::vector<SpriteAnimationFrame> frames;
};

#endif // RPG_SPRITEANIMATION_HPP
