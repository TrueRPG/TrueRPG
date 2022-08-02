#include "PointLightRenderSystem.h"

PointLightRenderSystem::PointLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader("../res/shaders/point_light.vs", "../res/shaders/point_light.fs"))
{
}

void PointLightRenderSystem::draw()
{
    m_shader.setUniform("light.pos", glm::vec2(0));
    m_shader.setUniform("light.color", glm::vec3(1));
    m_shader.setUniform("light.linear", 0.1f / 1000.f);
    m_shader.setUniform("light.quadratic", 1.8f / 100000.f);

    m_shader.setUniform("viewPos", glm::vec2(0));
}

Shader& PointLightRenderSystem::getShader()
{
    return m_shader;
}
