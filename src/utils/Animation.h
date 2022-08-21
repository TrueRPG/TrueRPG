#ifndef RPG_ANIMATION_HPP
#define RPG_ANIMATION_HPP

#include "../scene/Entity.h"
#include "../components/animation/SpriteAnimatorComponent.h"

class Animation
{
public:
    static SpriteAnimator createAnimator(const std::function<void(SpriteAnimatorBuilder&)>& setup);

    static SpriteAnimator loadAnimatorFromFile(const char* filename);

    static void addAnimator(Entity entity, const SpriteAnimator *animator);
};

#endif // RPG_ANIMATION_HPP
