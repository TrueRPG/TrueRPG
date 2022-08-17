#include "../../pch.h"

#include "EnvironmentSystem.h"
#include "../../components/world/EnvironmentComponent.h"
#include "../../components/world/ClockComponent.h"
#include "../../utils/DayNightCycle.h"

EnvironmentSystem::EnvironmentSystem(entt::registry &registry)
    : m_registry(registry)
{}

void EnvironmentSystem::update(float deltaTime)
{
    auto envView = m_registry.view<EnvironmentComponent>();
    if (envView.empty()) return;
    auto env = envView[0];
    auto &envComponent = m_registry.get<EnvironmentComponent>(env);

    auto clockView = m_registry.view<ClockComponent>();
    if (clockView.empty()) return;
    auto clock = clockView[0];
    auto &clockComponent = m_registry.get<ClockComponent>(clock);

    Entity nightAudio = envComponent.nightAudio;
    if (nightAudio)
    {
        float seconds = clockComponent.clock.getSeconds();
        float sunBrightness = DayNightCycle::computeSunBrightness(seconds);
        auto& audio = nightAudio.getComponent<AudioSourceComponent>();
        audio.volume = 1 - sunBrightness;
        if (audio.volume > 0)
        {
            audio.play();
        }
        else
        {
            audio.pause();
        }
    }
}
