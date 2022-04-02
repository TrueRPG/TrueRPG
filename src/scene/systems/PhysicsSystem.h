#ifndef RPG_PHYSICSSYSTEM_H
#define RPG_PHYSICSSYSTEM_H

#include "entt.hpp"
#include "ISystem.h"

class PhysicsSystem : public ISystem
{
    entt::registry& m_registry;
public:
    PhysicsSystem(entt::registry& registry);

    void update(float deltaTime) override;
};

#endif //RPG_PHYSICSSYSTEM_H
