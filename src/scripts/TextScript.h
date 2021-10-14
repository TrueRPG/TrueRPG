#ifndef RPG_TEXTSCRIPT_H
#define RPG_TEXTSCRIPT_H

#include "../scene/Script.h"

class TextScript : public Script
{
    Entity m_cameraEntity;

public:
    TextScript(Entity cameraEntity)
            : m_cameraEntity(cameraEntity) {}

    void onUpdate(float deltaTime) override
    {
        static float t = 0;
        t += deltaTime;

        auto &textTransform = getComponent<TransformComponent>();
        auto &cameraComponent = m_cameraEntity.getComponent<CameraComponent>();

        textTransform.position = glm::vec2(0.f, cameraComponent.getHeight() / 2);
        textTransform.scale = glm::vec2(1 / cameraComponent.zoom);

        auto &textRenderer = getComponent<TextRendererComponent>();
        textRenderer.color = glm::vec4(
                (std::sin(t) + 1) / 2,
                (std::sin(2 * t + 1) + 1) / 2,
                (std::sin(0.5 * t) + 2) / 2, 1.f
        );
    }
};

#endif //RPG_TEXTSCRIPT_H
