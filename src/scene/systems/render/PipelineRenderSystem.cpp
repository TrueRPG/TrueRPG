#include "PipelineRenderSystem.h"

#include <utility>

#include "entt.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "../../../client/window/Window.h"
#include "../../components/render/CameraComponent.h"
#include "../../components/basic/HierarchyComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/world/WorldMapComponent.h"

PipelineRenderSystem::PipelineRenderSystem(entt::registry &registry, std::vector<IRenderSystem*> pipeline)
        : m_registry(registry),
          m_shader(Shader::createShader("../res/shaders/shader.vs", "../res/shaders/shader.fs")),
          m_batch(m_shader, 30000),
          m_pipeline(std::move(pipeline))
{
    Window::getInstance().onResize += createEventHandler(&PipelineRenderSystem::resize);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PipelineRenderSystem::draw()
{
    // Find the first camera
    auto cameraView = m_registry.view<CameraComponent>();
    if (cameraView.empty()) return;
    auto cameraComponent = m_registry.get<CameraComponent>(cameraView[0]);
    TransformComponent cameraTransform = Hierarchy::computeTransform({cameraView[0], &m_registry});

    glm::vec4 back = cameraComponent.background;
    glClearColor(back.r, back.g, back.b, back.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::translate(glm::mat4(1), glm::vec3(-cameraTransform.position, 0));
    m_batch.setViewMatrix(viewMatrix);
    m_batch.setProjectionMatrix(cameraComponent.getProjectionMatrix());
    m_batch.begin();

    for (const auto &item : m_pipeline)
    {
        item->draw(m_batch);
    }

    m_batch.end();
}

void PipelineRenderSystem::destroy()
{
    m_batch.destroy();
    m_shader.destroy();
}

// Every time when the window size is changed (by user or OS), this callback function is invoked
void PipelineRenderSystem::resize(int width, int height)
{
    glViewport(0, 0, width, height);
}
