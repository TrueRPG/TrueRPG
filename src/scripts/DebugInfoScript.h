#ifndef RPG_DEBUGINFOSCRIPT_H
#define RPG_DEBUGINFOSCRIPT_H

#include <GLFW/glfw3.h>
#include "../scene/components/HierarchyComponent.h"

class DebugInfoScript : public Script
{
    Entity m_cameraEntity;

    float m_currentTime{0.f};
    int m_frameCount{0};
    float m_lastTime{0.f};
    int m_fps{0};

public:
    explicit DebugInfoScript(Entity cameraEntity)
            : m_cameraEntity(cameraEntity) {}

    void onUpdate(float deltaTime)
    {
        // TODO: Приходится обращаться к функции glfw, это плохо
        m_currentTime = glfwGetTime();
        m_frameCount++;

        // Если прошла секунда
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

        Entity playerEntity = getComponent<HierarchyComponent>().parent;
        auto &playerPosition = playerEntity.getComponent<TransformComponent>().position;

        textRenderer.text +=
                "\nx: " + std::to_string(playerPosition.x / 64) + " y: " + std::to_string(playerPosition.y / 64);
    }
};

#endif //RPG_DEBUGINFOSCRIPT_H
