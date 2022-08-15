#ifndef RPG_SPRITEANIMATORSYSTEM_H
#define RPG_SPRITEANIMATORSYSTEM_H

#include "entt.hpp"
#include "../../scene/ISystem.h"

class SpriteAnimatorSystem : public ISystem
{
    entt::registry &m_registry;

public:
    SpriteAnimatorSystem(entt::registry &registry);

    void update(float deltaTime);
};

#endif // RPG_SPRITEANIMATORSYSTEM_H
