#ifndef RPG_TEXTRENDERSYSTEM_H
#define RPG_TEXTRENDERSYSTEM_H

#include "entt.hpp"
#include "../../client/graphics/OpenGL/SpriteBatch.h"
#include "IRenderSubsystem.h"

class TextRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

public:
    TextRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch) override;
};

#endif // RPG_TEXTRENDERSYSTEM_H
