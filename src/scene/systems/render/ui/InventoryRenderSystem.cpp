#include "InventoryRenderSystem.h"
#include "../../../components/render/CameraComponent.h"
#include "../../../utils/Hierarchy.h"
#include "../../../../client/window/Window.h"
#include "../../../components/world/InventoryComponent.h"
#include "../../../components/world/ItemComponent.h"

InventoryRenderSystem::InventoryRenderSystem(entt::registry& registry)
    : m_registry(registry)
{
}

void InventoryRenderSystem::draw(SpriteBatch &batch, glm::vec2 cursor)
{
    glm::vec4 cellColor{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 panelColor{0.7f, 0.7f, 0.7f, 1.f};

    // If this method is invoked, it means we have at least one camera
    auto cameraEntity = m_registry.view<CameraComponent>()[0];
    auto cameraTransform = Hierarchy::computeTransform({cameraEntity, &m_registry});

    Window &window = Window::getInstance();
    auto view = m_registry.view<InventoryComponent>();

    // we can show only one inventory, so find the first one
    entt::entity inventoryEntity = entt::null;
    for (auto entity : view)
    {
        auto &inventoryComponent = view.get<InventoryComponent>(entity);
        if (inventoryComponent.shown)
        {
            inventoryEntity = entity;
        }
    }
    if (inventoryEntity != entt::null)
    {
        auto &inventoryComponent = m_registry.get<InventoryComponent>(inventoryEntity);

        float indent = 10;
        float cellSize = 70;
        glm::ivec2 inventorySize(inventoryComponent.items.size(), inventoryComponent.items[0].size());
        glm::vec2 menuSize(inventorySize * 80 + (int) indent);

        // draw panel
        Sprite panel;
        panel.setScale(glm::vec2(menuSize));
        panel.setPosition(cameraTransform.position - menuSize / 2.f);
        panel.setColor(panelColor);
        batch.draw(panel, 100);

        for (int i = 0; i < inventorySize.x; i++)
        {
            for (int j = 0; j < inventorySize.y; j++)
            {
                glm::vec2 cellPos = cameraTransform.position + glm::vec2(i, j) * (cellSize + indent) - menuSize / 2.f + indent;

                // draw cells
                Sprite cell;
                cell.setScale(glm::vec2(cellSize, cellSize));
                cell.setPosition(cellPos);
                cell.setColor(cellColor);
                batch.draw(cell, 100);

                // draw items
                Entity itemEntity = inventoryComponent.items[i][inventorySize.y - j - 1];
                if (itemEntity)
                {
                    auto &itemComponent = itemEntity.getComponent<ItemComponent>();
                    Sprite item(itemComponent.icon);
                    item.setTextureRect(itemComponent.iconRect);

                    // we need to fill the whole cell with the sprite
                    FloatRect bounds = item.getLocalBounds();
                    glm::vec2 itemSize(bounds.getWidth(), bounds.getWidth());
                    item.setScale(cellSize / itemSize);

                    item.setPosition(cellPos);

                    if (item.getGlobalBounds().contains(cursor))
                    {
                        // highlight the item
                        item.setColor({1.1f, 1.1f, 1.1f, 1.f});
                    }
                    else
                    {
                        item.setColor({1.f, 1.f, 1.f, 1.f});
                    }

                    batch.draw(item, 110);
                }
            }
        }
    }
}
