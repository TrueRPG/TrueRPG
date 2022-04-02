#ifndef RPG_WORLDMAPRENDERSYSTEM_H
#define RPG_WORLDMAPRENDERSYSTEM_H

#include "entt.hpp"
#include "../../../client/graphics/SpriteBatch.h"
#include "IRenderSubsystem.h"

class WorldMapRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

public:
    WorldMapRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch) override;
};

#endif // RPG_WORLDMAPRENDERSYSTEM_H
