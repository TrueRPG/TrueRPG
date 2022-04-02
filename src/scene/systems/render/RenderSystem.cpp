#include "RenderSystem.h"

#include "glm/ext/matrix_transform.hpp"
#include "../../../client/window/Window.h"
#include "../../components/render/CameraComponent.h"
#include "../../components/basic/HierarchyComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/world/WorldMapComponent.h"

RenderSystem::RenderSystem(entt::registry &registry)
        : m_registry(registry),
          m_shader(Shader::createShader("../res/shaders/shader.vs", "../res/shaders/shader.fs")),
          m_batch(m_shader, 30000)
{
    Window::getInstance().onResize += createEventHandler(&RenderSystem::resize);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderSystem::addSubsystem(IRenderSubsystem &renderSubsystem)
{
    m_subsystems.push_back(&renderSubsystem);
}

void RenderSystem::draw()
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

    for (const auto &item : m_subsystems)
    {
        item->draw(m_batch);
    }

    m_batch.end();
}

void RenderSystem::destroy()
{
    m_batch.destroy();
    m_shader.destroy();
}

// Every time when the window size is changed (by user or OS), this callback function is invoked
void RenderSystem::resize(int width, int height)
{
    glViewport(0, 0, width, height);
}
