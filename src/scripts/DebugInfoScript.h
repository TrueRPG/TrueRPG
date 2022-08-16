#ifndef RPG_DEBUGINFOSCRIPT_H
#define RPG_DEBUGINFOSCRIPT_H

#include "../scene/Script.h"

class DebugInfoScript : public Script
{
    Entity m_cameraEntity;
    Entity m_clockEntity;

    float m_currentTime{0.f};
    int m_frameCount{0};
    float m_lastTime{0.f};
    int m_fps{0};

public:
    explicit DebugInfoScript(Entity cameraEntity, Entity clockEntity);

    void onUpdate(float deltaTime);
};

#endif // RPG_DEBUGINFOSCRIPT_H
