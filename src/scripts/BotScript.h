#ifndef RPG_BOTSCRIPT_H
#define RPG_BOTSCRIPT_H

#include "../scene/Script.h"

enum BotState
{
    IDLE,
    WALK
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
    void onCreate() override;
    void onUpdate(float deltaTime) override;
};

#endif // RPG_BOTSCRIPT_H
