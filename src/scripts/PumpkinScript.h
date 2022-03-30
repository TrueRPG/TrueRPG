#ifndef RPG_PUMPKINSCRIPT_H
#define RPG_PUMPKINSCRIPT_H

#include "../scene/Script.h"
#include <glm/glm.hpp>
#include "../scene/utils/Hierarchy.h"
#include "../client/window/Window.h"
#include "../scene/components/basic/TransformComponent.h"
#include "../scene/components/render/TextRendererComponent.h"
#include "../scene/components/audio/AudioSourceComponent.h"

class PumpkinScript : public Script
{
    Entity m_playerEntity;

    Entity m_textEntity{};

public:
    PumpkinScript(Entity playerEntity)
            : m_playerEntity(playerEntity) {}

    void onCreate() override
    {
        m_textEntity = Hierarchy::find(getEntity(), "text");
    }

    void onUpdate(float deltaTime) override
    {
        static float t = 0;
        t += 5 * deltaTime;

        Window &window = Window::getInstance();

        auto &textTransform = m_textEntity.getComponent<TransformComponent>();
        textTransform.position.y = 5 * std::sin(t) + 32;

        auto &pumpkinTransform = getComponent<TransformComponent>();
        auto &playerTransform = m_playerEntity.getComponent<TransformComponent>();

        auto &textRenderer = m_textEntity.getComponent<TextRendererComponent>();
        auto &audioSource = getComponent<AudioSourceComponent>();

        if (glm::distance(pumpkinTransform.position, playerTransform.position) < 64.f)
        {
            textRenderer.text = "Press [E]";
            if (window.getKey(GLFW_KEY_E))
            {
                audioSource.play();
            }
        }
        else
        {
            textRenderer.text = "";
        }

        if (audioSource.state == AudioState::Play)
        {
            textRenderer.color = glm::vec4(1.f, 1.f, 1.f, std::clamp(textRenderer.color.a - 6.f * deltaTime, 0.f, 1.f));
        }
    }
};

#endif //RPG_PUMPKINSCRIPT_H
