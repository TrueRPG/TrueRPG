#ifndef RPG_PHYSICSSYSTEM_H
#define RPG_PHYSICSSYSTEM_H

#include <entt.hpp>

class PhysicsSystem
{
    entt::registry& m_registry;
public:
    PhysicsSystem(entt::registry& registry);

    void update(float deltaTime);
};

#endif //RPG_PHYSICSSYSTEM_H
