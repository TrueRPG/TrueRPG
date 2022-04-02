#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H

#include <vector>
#include "../scene/Script.h"
#include "../client/window/Window.h"
#include "../components/physics/RigidbodyComponent.h"
#include "../components/world/HpComponent.h"
#include "../components/world/InventoryComponent.h"
#include "../utils/Hierarchy.h"

// TODO: refactor
class PlayerScript : public Script
{
    float m_speed{1.3f};

    Entity m_spriteEntity{};
    Entity m_stepsEntity{};

    Entity m_cameraEntity{};
    Entity m_hpEntity{};

    std::vector<int> m_inputStack;

    float m_animationDelay{0.f};
    int m_currentAnimation{3};
    int m_frame{0};

    bool pressedK;
    bool pressedI;

public:
    void onCreate() override
    {
        m_spriteEntity = Hierarchy::find(getEntity(), "sprite");
        m_stepsEntity = Hierarchy::find(getEntity(), "stepsSound");

        m_cameraEntity = Hierarchy::find(getEntity(), "camera");
        m_hpEntity = Hierarchy::find(getEntity(), "hp");
    }

    void onUpdate(float deltaTime) override
    {
        Window &window = Window::getInstance();

        // TODO: temporary solution
        if (window.getKey(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        // TODO: just for a test, delete this in the future
        if (window.getKey(GLFW_KEY_K) && !pressedK)
        {
            auto &hpComponent = getComponent<HpComponent>();
            hpComponent.value -= 10;
            pressedK = true;
            auto &renderer = m_spriteEntity.getComponent<SpriteRendererComponent>();
            renderer.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
        }
        if (!window.getKey(GLFW_KEY_K))
        {
            pressedK = false;
            auto &renderer = m_spriteEntity.getComponent<SpriteRendererComponent>();
            renderer.color = glm::vec4(1.f);
        }

        // Inventory opening/closing
        auto& inventory = getComponent<InventoryComponent>();

        if (window.getKey(GLFW_KEY_I) && !pressedI)
        {
            inventory.shown = !inventory.shown;
            pressedI = true;
        }
        else if (!window.getKey(GLFW_KEY_I))
        {
            pressedI = false;
        }

        auto &rigidbody = getComponent<RigidbodyComponent>();
        auto &stepsSound = m_stepsEntity.getComponent<AudioSourceComponent>();

        updateInput();
        int currentKey = m_inputStack.empty() ? -1 : m_inputStack.back();

        // Controls
        glm::ivec2 movement(0);
        if (currentKey == GLFW_KEY_W)
        {
            movement = glm::ivec2(0, 1);
            m_currentAnimation = 0;
        }
        if (currentKey == GLFW_KEY_A)
        {
            movement = glm::ivec2(-1, 0);
            m_currentAnimation = 2;
        }
        if (currentKey == GLFW_KEY_S)
        {
            movement = glm::ivec2(0, -1);
            m_currentAnimation = 3;
        }
        if (currentKey == GLFW_KEY_D)
        {
            movement = glm::ivec2(1, 0);
            m_currentAnimation = 1;
        }

        rigidbody.velocity = glm::vec2(movement) * m_speed * 200.f;

        // Animation magic
        auto &renderer = m_spriteEntity.getComponent<SpriteRendererComponent>();

        if (m_animationDelay > 30.f)
        {
            renderer.textureRect = IntRect(m_frame * 32, m_currentAnimation * 32, 32, 32);
            m_frame++;
            m_animationDelay = 0.f;
        }
        m_animationDelay += deltaTime * 200.f;

        if (m_frame > 2)
        {
            m_frame = 0;
        }

        if (movement == glm::ivec2(0.f))
        {
            m_frame = 1;
            stepsSound.pause();
        }
        else
        {
            stepsSound.play();
        }

        updateUi();
    }

private:
    void updateInput()
    {
        Window &window = Window::getInstance();

        int keys[] = {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D};

        for (const auto &key : keys)
        {
            if (window.getKey(key))
            {
                if (std::find(m_inputStack.begin(), m_inputStack.end(), key) == m_inputStack.end())
                {
                    m_inputStack.push_back(key);
                }
            }
            else
            {
                auto newEnd = std::remove_if(m_inputStack.begin(), m_inputStack.end(),
                                          [key](const int &k){return k == key;});
                m_inputStack.erase(newEnd, m_inputStack.end());
            }
        }
    }

    void updateUi()
    {
        auto &textTransform = m_hpEntity.getComponent<TransformComponent>();
        auto &cameraComponent = m_cameraEntity.getComponent<CameraComponent>();

        textTransform.position = glm::vec2(cameraComponent.getWidth() / 2, cameraComponent.getHeight() / 2);
        textTransform.scale = glm::vec2(1 / cameraComponent.zoom);

        auto &hpComponent = getComponent<HpComponent>();
        auto &textRenderer = m_hpEntity.getComponent<TextRendererComponent>();
        textRenderer.text = "HP: " + std::to_string(hpComponent.value);
    }
};

#endif //RPG_PLAYER_H
