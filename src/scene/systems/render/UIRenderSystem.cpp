#include "UIRenderSystem.h"
#include "glm/vec2.hpp"
#include "../../../client/window/Window.h"
#include "../../components/render/ui/ButtonComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../../client/graphics/Text.h"
#include "../../components/world/InventoryComponent.h"
#include "../../components/render/CameraComponent.h"
#include "../../components/world/ItemComponent.h"

UIRenderSystem::UIRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_emptyTexture(Texture::createEmpty())
{
}

static bool isRectSelected(FloatRect rect, glm::vec2 cursor)
{
    return cursor.x > rect.getLeft() && cursor.x < rect.getLeft() + rect.getWidth() && cursor.y > rect.getBottom() &&
           cursor.y < rect.getBottom() + rect.getHeight();
}

// TODO: It's not ready yet
void UIRenderSystem::draw(SpriteBatch &batch)
{
    // UI rendering
    Window &window = Window::getInstance();
    glm::vec2 cursor = window.getCursorPosition();
    cursor = cursor - glm::vec2(window.getWidth(), window.getHeight()) / 2.f; // the origin is the center of the screen now
    cursor = glm::vec2(-cursor.x, cursor.y); // change the direction of x-axis
    cursor = batch.getViewMatrix() * -glm::vec4(cursor, 0.f, 1.f); // convert it to world coords

    // TODO: make separate classes
    drawButtons(batch, cursor);
    drawInventory(batch, cursor);
}

void UIRenderSystem::drawButtons(SpriteBatch &batch, glm::vec2 cursor)
{
    Window &window = Window::getInstance();
    auto view = m_registry.view<ButtonComponent>();
    for (auto entity : view)
    {
        auto &buttonComponent = view.get<ButtonComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        Sprite sprite(m_emptyTexture);
        sprite.setScale(buttonComponent.size);
        sprite.setPosition(transformComponent.position);

        if (isRectSelected(sprite.getGlobalBounds(), cursor))
        {
            // TODO: hardcoded mouse button
            if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
            {
                sprite.setColor(buttonComponent.pressedColor);
            }
            else
            {
                sprite.setColor(buttonComponent.highlightedColor);
            }
        }
        else
        {
            sprite.setColor(buttonComponent.color);
        }

        batch.draw(sprite, 100);

        Text text(*buttonComponent.font, buttonComponent.text);
        auto textBounds = text.getLocalBounds();
        text.setPosition(
            transformComponent.position + buttonComponent.size / 2.f - glm::vec2(textBounds.getWidth(), textBounds.getHeight()) / 2.f);
        text.draw(batch, 100);
    }
}

void UIRenderSystem::drawInventory(SpriteBatch &batch, glm::vec2 cursor)
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
        Sprite panel(m_emptyTexture);
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
                Sprite cell(m_emptyTexture);
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

                    if (isRectSelected(item.getGlobalBounds(), cursor))
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
