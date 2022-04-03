#ifndef RPG_INVENTORYRENDERSYSTEM_H
#define RPG_INVENTORYRENDERSYSTEM_H

#include "IUIRenderSubsystem.h"
#include "entt.hpp"
#include "../../../scene/Entity.h"

class InventoryRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    Entity m_draggedEntity;
    glm::ivec2 m_itemLastPos;
    glm::vec2 m_itemDelta;

public:
    InventoryRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch, glm::vec2 cursor) override;
};

#endif // RPG_INVENTORYRENDERSYSTEM_H
