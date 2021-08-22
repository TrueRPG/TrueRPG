#ifndef RPG_BOTSCRIPT_H
#define RPG_BOTSCRIPT_H

#include <glm/gtc/random.hpp>

enum BotState
{
    IDLE, WALK
};

class BotScript : public Script
{
    float m_speed{1.3f};

    Entity m_spriteEntity{};

    float m_animationDelay{0.f};
    int m_currentAnimation{3};
    int m_frame{0};

    BotState m_currentState{IDLE};
    glm::ivec2 m_dir{0};

    float m_idleTime{1.f};
    float m_walkTime{1.f};

    float m_time{0.f};
public:
    void onCreate() override
    {
        m_spriteEntity = Hierarchy::find(getEntity(), "sprite");
    }

    void onUpdate(float deltaTime) override
    {
        auto &rigidbody = getComponent<RigidbodyComponent>();

        if (m_currentState == IDLE)
        {
            rigidbody.velocity = glm::vec2(0.f);
            if (m_time > m_idleTime) {
                m_currentState = WALK;
                m_time = 0.f;

                glm::ivec2 dirs[] = {glm::ivec2(1, 0), glm::ivec2(0, 1), glm::ivec2(-1, 0), glm::ivec2(0, -1)};
                int r = glm::linearRand(0, 3);

                m_dir = dirs[r];
                m_walkTime = glm::linearRand(1.f, 2.f);
            }
        }
        else if (m_currentState == WALK)
        {
            rigidbody.velocity = glm::vec2(m_dir) * m_speed * 200.f;
            if (m_time > m_walkTime) {
                m_currentState = IDLE;
                m_time = 0.f;
                m_idleTime = glm::linearRand(1.f, 2.f);
            }
        }

        m_time += deltaTime;

        if (m_dir == glm::ivec2(0, 1))
        {
            m_currentAnimation = 0;
        }
        if (m_dir == glm::ivec2(-1, 0))
        {
            m_currentAnimation = 2;
        }
        if (m_dir == glm::ivec2(0, -1))
        {
            m_currentAnimation = 3;
        }
        if (m_dir == glm::ivec2(1, 0))
        {
            m_currentAnimation = 1;
        }

        // Animation
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

        if (m_currentState == IDLE)
        {
            m_frame = 1;
        }
    }
};

#endif //RPG_BOTSCRIPT_H
