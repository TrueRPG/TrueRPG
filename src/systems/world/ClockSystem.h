#ifndef RPG_CLOCKSYSTEM_H
#define RPG_CLOCKSYSTEM_H

#include "../../scene/ISystem.h"
#include "entt.hpp"

class ClockSystem : public ISystem
{
    entt::registry &m_registry;

public:
    ClockSystem(entt::registry &registry);

    void update(float deltaTime) override;
};

#endif // RPG_CLOCKSYSTEM_H
