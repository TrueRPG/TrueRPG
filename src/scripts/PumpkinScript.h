#ifndef RPG_PUMPKINSCRIPT_H
#define RPG_PUMPKINSCRIPT_H

#include "../scene/Script.h"

class PumpkinScript : public Script
{
    Entity m_playerEntity;
    Entity m_textEntity{};

public:
    PumpkinScript(Entity playerEntity);

    void onCreate() override;
    void onUpdate(float deltaTime) override;
};

#endif // RPG_PUMPKINSCRIPT_H
