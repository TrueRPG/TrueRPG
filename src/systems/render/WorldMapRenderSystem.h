#ifndef RPG_WORLDMAPRENDERSYSTEM_H
#define RPG_WORLDMAPRENDERSYSTEM_H

#include "entt.hpp"
#include "IRenderSubsystem.h"

class ISpriteBatch;

class WorldMapRenderSystem : public IRenderSubsystem
{
    entt::registry& m_registry;

public:
    WorldMapRenderSystem(entt::registry& registry);

    void draw(ISpriteBatch& batch) override;
};

#endif // RPG_WORLDMAPRENDERSYSTEM_H
