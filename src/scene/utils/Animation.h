#ifndef RPG_ANIMATION_HPP
#define RPG_ANIMATION_HPP

#include "../Entity.h"
#include "../components/SpriteAnimatorComponent.h"

class Animation
{
public:
    static SpriteAnimator createAnimator(const std::function<void(SpriteAnimatorBuilder&)>& setup);

    static void addAnimator(Entity entity, const SpriteAnimator *animator);
};

#endif // RPG_ANIMATION_HPP
