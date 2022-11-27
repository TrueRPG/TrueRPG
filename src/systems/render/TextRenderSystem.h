#ifndef RPG_TEXTRENDERSYSTEM_H
#define RPG_TEXTRENDERSYSTEM_H

#include "entt.hpp"
#include "IRenderSubsystem.h"

class ISpriteBatch;

class TextRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

public:
    TextRenderSystem(entt::registry& registry);

    void draw(ISpriteBatch& batch) override;
};

#endif // RPG_TEXTRENDERSYSTEM_H
