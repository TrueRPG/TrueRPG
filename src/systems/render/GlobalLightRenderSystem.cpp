#include "../../pch.h"

#include "GlobalLightRenderSystem.h"
#include "../../components/world/ClockComponent.h"
#include "../../utils/DayNightCycle.h"

GlobalLightRenderSystem::GlobalLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader(TRUERPG_RES_DIR "/shaders/global_light.vs", TRUERPG_RES_DIR "/shaders/global_light.fs", {false, false, true})),
      m_quad()
{
}

void GlobalLightRenderSystem::draw()
{
    auto clockView = m_registry.view<ClockComponent>();
    if (clockView.empty()) return;

    auto clock = clockView[0];

    auto &clockComponent = m_registry.get<ClockComponent>(clock);
    float seconds = clockComponent.clock.getSeconds();

    float brightness = DayNightCycle::computeSunBrightness(seconds);

    m_shader.setUniform("brightness", brightness);
    m_shader.updateUbo();
    m_quad.draw();
}

Shader &GlobalLightRenderSystem::getShader()
{
    return m_shader;
}

void GlobalLightRenderSystem::destroy()
{
    m_quad.destroy();
}
