#include <iomanip>

#include "GlobalLightRenderSystem.h"
#include "../../components/world/ClockComponent.h"

GlobalLightRenderSystem::GlobalLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader(TRUERPG_RES_DIR "/shaders/global_light.vs", TRUERPG_RES_DIR "/shaders/global_light.fs")),
      m_quad()
{
}

void GlobalLightRenderSystem::draw()
{
    auto clockView = m_registry.view<ClockComponent>();
    if (clockView.empty()) return;

    auto clock = m_registry.view<ClockComponent>()[0];

    auto &clockComponent = m_registry.get<ClockComponent>(clock);
    float seconds = clockComponent.clock.getSeconds();

    // the formula of the day-night cycle:
    // brightness = (tanh(10 * sin(pi / 43200 * (x - 23000)) + 3.2) + 1) / 2
    float brightness = (std::tanh(10.f * std::sin(glm::pi<float>() / 43200.f * (seconds - 23000.f)) + 3.2f) + 1.4f) / 2.4f;

    m_shader.setUniform("brightness", brightness);
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
