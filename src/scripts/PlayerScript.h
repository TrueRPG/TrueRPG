#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H

#include <vector>
#include "../scene/Script.h"
#include "../client/window/Window.h"
#include "../scene/components/RigidbodyComponent.h"

class PlayerScript : public Script
{
    float m_speed{1.3f};

    Entity m_spriteEntity{};
    Entity m_stepsEntity{};

    std::vector<int> m_inputStack;

    float m_animationDelay{0.f};
    int m_currentAnimation{3};
    int m_frame{0};

public:
    void onCreate() override
    {
        m_spriteEntity = Hierarchy::find(getEntity(), "sprite");
        m_stepsEntity = Hierarchy::find(getEntity(), "stepsSound");
    }

    void onUpdate(float deltaTime) override
    {
        Window &window = Window::getInstance();

        // Пока поместил выход из игры сюда
        if (window.getKey(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        auto &rigidbody = getComponent<RigidbodyComponent>();
        auto &stepsSound = m_stepsEntity.getComponent<AudioSourceComponent>();

        updateInput();
        int currentKey = m_inputStack.empty() ? -1 : m_inputStack.back();

        // Управление
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

        // Шаманю с анимацией
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
};

#endif //RPG_PLAYER_H
