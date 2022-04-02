#ifndef RPG_SPRITERENDERSYSTEM_H
#define RPG_SPRITERENDERSYSTEM_H

#include "entt.hpp"
#include "../../../client/graphics/SpriteBatch.h"
#include "IRenderSubsystem.h"

class SpriteRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

public:
    SpriteRenderSystem(entt::registry& registry);

    virtual void draw(SpriteBatch& batch);
};

#endif // RPG_SPRITERENDERSYSTEM_H
