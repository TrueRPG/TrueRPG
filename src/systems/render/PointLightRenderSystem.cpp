#include "PointLightRenderSystem.h"

#include "../../components/render/PoinLightComponent.h"
#include "../../utils/Hierarchy.h"

PointLightRenderSystem::PointLightRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_shader(Shader::createShader("../res/shaders/point_light.vs", "../res/shaders/point_light.fs"))
{
    float quad_vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    m_quadVao.bind();
    m_quadVbo.bind();
    m_quadVbo.setData(quad_vertices, sizeof(quad_vertices), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    m_quadVbo.unbind();
    m_quadVao.unbind();
}

PointLightRenderSystem::~PointLightRenderSystem()
{
    m_quadVbo.destroy();
    m_quadVao.destroy();
}

void PointLightRenderSystem::draw()
{
    auto view = m_registry.view<PointLightComponent>();

    for (auto entity : view)
    {
        auto &pointLightComponent = view.get<PointLightComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        m_shader.setUniform("light.pos", transformComponent.position);
        m_shader.setUniform("light.color", pointLightComponent.color);
        m_shader.setUniform("light.radius", pointLightComponent.radius);
        m_shader.setUniform("light.intensity", pointLightComponent.intensity);

        m_shader.setUniform("viewPos", glm::vec2(0));

        m_quadVao.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_quadVao.unbind();
    }
}

Shader& PointLightRenderSystem::getShader()
{
    return m_shader;
}
