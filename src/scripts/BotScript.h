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

        // SpriteAnimator
        auto &animator = m_spriteEntity.getComponent<SpriteAnimatorComponent>();

        animator.parameterStorage.at("velocity") = rigidbody.velocity;
    }
};

#endif //RPG_BOTSCRIPT_H
