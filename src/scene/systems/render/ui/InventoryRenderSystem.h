#ifndef RPG_INVENTORYRENDERSYSTEM_H
#define RPG_INVENTORYRENDERSYSTEM_H

#include "IUIRenderSubsystem.h"
#include "entt.hpp"

class InventoryRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    Texture m_texture;

public:
    InventoryRenderSystem(entt::registry& registry, Texture texture);

    virtual void draw(SpriteBatch& batch, glm::vec2 cursor);
};

#endif // RPG_INVENTORYRENDERSYSTEM_H
