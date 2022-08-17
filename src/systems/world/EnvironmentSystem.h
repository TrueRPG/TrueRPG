#ifndef RPG_ENVIRONMENTSYSTEM_H
#define RPG_ENVIRONMENTSYSTEM_H

#include "../../scene/ISystem.h"
#include "entt.hpp"

class EnvironmentSystem : public ISystem
{
    entt::registry &m_registry;

public:
    EnvironmentSystem(entt::registry &registry);

    void update(float deltaTime) override;
};

#endif // RPG_ENVIRONMENTSYSTEM_H
