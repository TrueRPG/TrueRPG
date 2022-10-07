#include "../../pch.h"
#include "PointLightRenderSystem.h"

#include "../../components/render/PointLightComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/world/ClockComponent.h"
#include "../../utils/DayNightCycle.h"

PointLightRenderSystem::PointLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader(TRUERPG_RES_DIR "/shaders/point_light.vs", TRUERPG_RES_DIR "/shaders/point_light.fs", {false, true})),
      m_quad()
{ }

void PointLightRenderSystem::draw()
{
    auto view = m_registry.view<PointLightComponent>();

    for (auto entity : view)
    {
        auto &pointLightComponent = view.get<PointLightComponent>(entity);

        if (!pointLightComponent.enabled)
        {
            continue;
        }

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        float intensity = pointLightComponent.intensity;

        auto clockView = m_registry.view<ClockComponent>();
        if (!clockView.empty())
        {
            auto clock = m_registry.view<ClockComponent>()[0];
            auto &clockComponent = m_registry.get<ClockComponent>(clock);
            float seconds = clockComponent.clock.getSeconds();

            float sunBrightness = DayNightCycle::computeSunBrightness(seconds);
            intensity *= (1 - sunBrightness);
        }

        m_shader.setUniform("light.pos", transformComponent.position);
        m_shader.setUniform("light.color", pointLightComponent.color);
        m_shader.setUniform("light.radius", pointLightComponent.radius);
        m_shader.setUniform("light.intensity", intensity);
        m_shader.updateUbo();

        m_quad.draw();
    }
}

Shader& PointLightRenderSystem::getShader()
{
    return m_shader;
}

void PointLightRenderSystem::destroy()
{
    m_quad.destroy();
}
