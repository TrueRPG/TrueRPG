#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H

#include "../scene/Script.h"

// TODO: refactor
class PlayerScript : public Script
{
    Entity m_cameraEntity{};
    Entity m_hpEntity{};
    Entity m_spriteEntity{};

public:
    void onCreate() override;
    void onUpdate(float deltaTime) override;
};

#endif // RPG_PLAYER_H
