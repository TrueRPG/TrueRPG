#include "../pch.h"
#include "DebugInfoScript.h"

#include <GLFW/glfw3.h>
#include "../components/basic/HierarchyComponent.h"
#include "../components/world/ClockComponent.h"
#include "../components/render/CameraComponent.h"
#include "../components/render/TextRendererComponent.h"

DebugInfoScript::DebugInfoScript(Entity cameraEntity, Entity clockEntity)
    : m_cameraEntity(cameraEntity),
      m_clockEntity(clockEntity)
{
}

void DebugInfoScript::onUpdate(float deltaTime)
{
    // TODO: we invokes glfw function, it's not good
    m_currentTime = glfwGetTime();
    m_frameCount++;

    // if a second has passed
    if (m_currentTime - m_lastTime >= 1.0)
    {
        m_fps = m_frameCount;

        m_frameCount = 0;
        m_lastTime = m_currentTime;
    }

    auto &transform = getComponent<TransformComponent>();
    auto &cameraComponent = m_cameraEntity.getComponent<CameraComponent>();

    transform.position = glm::vec2(-cameraComponent.getWidth() / 2, -cameraComponent.getHeight() / 2);
    transform.scale = glm::vec2(1 / cameraComponent.zoom);

    auto &textRenderer = getComponent<TextRendererComponent>();
    textRenderer.text = "FPS: " + std::to_string(m_fps);

    textRenderer.text += "\ntime: " + m_clockEntity.getComponent<ClockComponent>().clock.toString();

    Entity playerEntity = getComponent<HierarchyComponent>().parent;
    auto &playerPosition = playerEntity.getComponent<TransformComponent>().position;

    textRenderer.text += "\nx: " + std::to_string(playerPosition.x / 64) + " y: " + std::to_string(playerPosition.y / 64);
}
