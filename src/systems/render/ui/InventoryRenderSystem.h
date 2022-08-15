#ifndef RPG_INVENTORYRENDERSYSTEM_H
#define RPG_INVENTORYRENDERSYSTEM_H

#include "IUIRenderSubsystem.h"
#include "entt.hpp"
#include "../../../scene/Entity.h"
#include "../../../client/graphics/Font.h"

#define DESCRIPTION_TIMER 0.4f

class InventoryRenderSystem : public IUIRenderSubsystem
{
    entt::registry& m_registry;

    Entity m_selectedEntity;

    Entity m_draggedEntity;
    glm::ivec2 m_itemLastPos;
    glm::vec2 m_itemDelta;

    Font font{TRUERPG_RES_DIR "/fonts/vt323.ttf", 32};

    glm::vec2 prevCursor{};
    float m_descriptionTimer{DESCRIPTION_TIMER};

public:
    InventoryRenderSystem(entt::registry& registry);

    void draw(SpriteBatch& batch, glm::vec2 cursor) override;

    void update(float deltaTime) override;
};

#endif // RPG_INVENTORYRENDERSYSTEM_H
