#ifndef RPG_WORLDMAPRENDERSYSTEM_H
#define RPG_WORLDMAPRENDERSYSTEM_H

#include "entt.hpp"
#include "../../../client/graphics/SpriteBatch.h"
#include "IRenderSystem.h"

class WorldMapRenderSystem : public IRenderSystem
{
    entt::registry& m_registry;

public:
    WorldMapRenderSystem(entt::registry& registry);

    virtual void draw(SpriteBatch& batch);
};

#endif // RPG_WORLDMAPRENDERSYSTEM_H
