#include "../../../pch.h"
#include "InventoryRenderSystem.h"

#include "../../../client/graphics/ISpriteBatch.h"
#include "../../../components/render/CameraComponent.h"
#include "../../../utils/Hierarchy.h"
#include "../../../components/world/InventoryComponent.h"
#include "../../../components/world/ItemComponent.h"
#include "../../../client/graphics/Text.h"
#include "GLFW/glfw3.h"

InventoryRenderSystem::InventoryRenderSystem(entt::registry& registry)
    : m_registry(registry)
{
}

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(' ');
    return (start == std::string::npos) ? "" : s.substr(start);
}

static std::string normalizeText(const std::string& text, size_t lineLength)
{
    std::string result;
    for (size_t i = 0; i < text.length();) {
        if (i != 0) result += "\n";

        if (text.length() - i <= lineLength)
        {
            result += ltrim(text.substr(i, lineLength));
            break;
        }

        size_t last = text.find_last_of(' ', i + lineLength);
        if (last == std::string::npos)
        {
            result += ltrim(text.substr(i, lineLength));
            i += lineLength;
        }
        else
        {
            result += ltrim(text.substr(i, last - i));
            i = last;
        }
    }
    return result;
}

// TODO: this method is too big
void InventoryRenderSystem::draw(ISpriteBatch &batch, glm::vec2 cursor)
{
    glm::vec4 cellColor{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 panelColor{0.7f, 0.7f, 0.7f, 1.f};

    // If this method is invoked, it means we have at least one camera
    auto cameraEntity = m_registry.view<CameraComponent>()[0];
    auto cameraTransform = Hierarchy::computeTransform({cameraEntity, &m_registry});

    IWindow &window = Engine::getWindow();
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
        glm::vec2 menuSize(inventorySize * 80 + (int)indent);

        // draw panel
        Sprite panel;
        panel.setScale(glm::vec2(menuSize));
        panel.setPosition(cameraTransform.position - menuSize / 2.f);
        panel.setColor(panelColor);
        batch.draw(panel, 10);

        m_selectedEntity = Entity();

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
                batch.draw(cell, 10);

                // place the dragged item
                if (m_draggedEntity && !window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
                {
                    // the cell must be empty
                    auto currentCellEntity = inventoryComponent.items[i][inventorySize.y - j - 1];
                    if (cell.getGlobalBounds().contains(cursor) && (!currentCellEntity || currentCellEntity == m_draggedEntity))
                    {
                        inventoryComponent.items[m_itemLastPos.x][m_itemLastPos.y] = Entity();
                        inventoryComponent.items[i][inventorySize.y - j - 1] = m_draggedEntity;
                        m_draggedEntity = Entity();
                    }
                }

                // draw items
                Entity itemEntity = inventoryComponent.items[i][inventorySize.y - j - 1];
                if (itemEntity)
                {
                    auto &itemComponent = itemEntity.getComponent<ItemComponent>();
                    Sprite item(*itemComponent.icon);
                    item.setTextureRect(itemComponent.iconRect);

                    // we need to fill the whole cell with the sprite
                    FloatRect bounds = item.getLocalBounds();
                    glm::vec2 itemSize(bounds.getWidth(), bounds.getWidth());
                    item.setScale(cellSize / itemSize);

                    item.setPosition(cellPos);

                    if (item.getGlobalBounds().contains(cursor) && !m_draggedEntity)
                    {
                        // highlight the item
                        item.setColor({1.1f, 1.1f, 1.1f, 1.f});
                        m_selectedEntity = itemEntity;

                        // TODO: hardcoded mouse button
                        if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
                        {
                            m_draggedEntity = itemEntity;
                            // we want to drag an item by the point where we clicked, so we need this variable to offset it
                            m_itemDelta = cursor - item.getPosition();
                            m_itemLastPos = glm::ivec2(i, inventorySize.y - j - 1);
                        }
                    }
                    else
                    {
                        item.setColor({1.f, 1.f, 1.f, 1.f});
                    }

                    if (itemEntity == m_draggedEntity)
                    {
                        item.setPosition(cursor - m_itemDelta);
                        batch.draw(item, 12);
                    }
                    else
                    {
                        batch.draw(item, 11);
                    }
                }
            }
        }

        // draw description panel
        if (m_selectedEntity && m_descriptionTimer <= 0)
        {
            auto &itemComponent = m_selectedEntity.getComponent<ItemComponent>();

            Text text(font, itemComponent.name + "\n" + normalizeText(itemComponent.description, 40));
            FloatRect localBounds = text.getLocalBounds();
            text.setPosition(cursor + glm::vec2(24, -localBounds.getHeight() - 24));

            FloatRect textBounds = text.getGlobalBounds();
            Sprite descriptionPanel;
            descriptionPanel.setPosition(glm::vec2(textBounds.getLeft(), textBounds.getBottom()) - 10.f);
            descriptionPanel.setScale(glm::vec2(textBounds.getWidth(), textBounds.getHeight()) + 20.f);
            descriptionPanel.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
            batch.draw(descriptionPanel, 13);

            text.draw(batch, 13);
        }

        if (prevCursor != cursor)
        {
            m_descriptionTimer = DESCRIPTION_TIMER;
        }
        prevCursor = cursor;
    }
}

void InventoryRenderSystem::update(float deltaTime)
{
    if (m_descriptionTimer > 0)
    {
        m_descriptionTimer -= deltaTime;
    }
    else
    {
        m_descriptionTimer = 0;
    }
}
