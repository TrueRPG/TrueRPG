#include "PointLightRenderSystem.h"

#include "../../components/render/PoinLightComponent.h"
#include "../../utils/Hierarchy.h"

PointLightRenderSystem::PointLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader("../res/shaders/point_light.vs", "../res/shaders/point_light.fs")),
      m_quad()
{ }

PointLightRenderSystem::~PointLightRenderSystem()
{
    m_quad.destroy();
}

void PointLightRenderSystem::draw()
{
    auto view = m_registry.view<PointLightComponent>();

    for (auto entity : view)
    {
        auto &pointLightComponent = view.get<PointLightComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        m_shader.setUniform("light.pos", transformComponent.position + pointLightComponent.posOffset);
        m_shader.setUniform("light.color", pointLightComponent.color);
        m_shader.setUniform("light.radius", pointLightComponent.radius);
        m_shader.setUniform("light.intensity", pointLightComponent.intensity);

        m_shader.setUniform("viewPos", glm::vec2(0));

        m_quad.draw();
    }
}

Shader& PointLightRenderSystem::getShader()
{
    return m_shader;
}
