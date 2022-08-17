#include "../../pch.h"

#include "ClockSystem.h"
#include "../../components/world/ClockComponent.h"

ClockSystem::ClockSystem(entt::registry &registry)
    : m_registry(registry)
{}

void ClockSystem::update(float deltaTime)
{
    auto clockView = m_registry.view<ClockComponent>();
    if (clockView.empty()) return;

    auto clock = clockView[0];

    auto &clockComponent = m_registry.get<ClockComponent>(clock);
    clockComponent.clock += deltaTime * clockComponent.clockSpeed;
}
