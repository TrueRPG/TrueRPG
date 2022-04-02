#ifndef RPG_INVENTORYRENDERSYSTEM_H
#define RPG_INVENTORYRENDERSYSTEM_H

#include "IUIRenderSubsystem.h"
#include "entt.hpp"

class InventoryRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

public:
    InventoryRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch, glm::vec2 cursor) override;
};

#endif // RPG_INVENTORYRENDERSYSTEM_H
