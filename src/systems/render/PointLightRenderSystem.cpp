#include "../../pch.h"
#include "PointLightRenderSystem.h"

#include "../../components/render/PointLightComponent.h"
#include "../../utils/Hierarchy.h"

PointLightRenderSystem::PointLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader(TRUERPG_RES_DIR "/shaders/point_light.vs", TRUERPG_RES_DIR "/shaders/point_light.fs")),
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

        m_shader.setUniform("light.pos", transformComponent.position);
        m_shader.setUniform("light.color", pointLightComponent.color);
        m_shader.setUniform("light.radius", pointLightComponent.radius);
        m_shader.setUniform("light.intensity", pointLightComponent.intensity);

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
