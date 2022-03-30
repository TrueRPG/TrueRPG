#include "UIRenderSystem.h"
#include "glm/vec2.hpp"
#include "../../../client/window/Window.h"
#include "../../components/render/ui/ButtonComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../../client/graphics/Text.h"
#include "../../components/render/ui/InventoryComponent.h"

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
    Window &window = Window::getInstance();
    auto view = m_registry.view<InventoryComponent>();
    for (auto entity : view)
    {
        auto &inventoryComponent = view.get<InventoryComponent>(entity);
        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        if (inventoryComponent.shown)
        {
            float indent = 10;
            float cellSize = 70;
            glm::vec2 size(inventoryComponent.size * 80 + (int) indent);
            Sprite panel(m_emptyTexture);
            panel.setScale(glm::vec2(size));
            panel.setPosition(transformComponent.position - size / 2.f);
            panel.setColor(inventoryComponent.panelColor);
            batch.draw(panel, 100);

            for (int i = 0; i < inventoryComponent.size.x; i++)
            {
                for (int j = 0; j < inventoryComponent.size.y; j++)
                {
                    Sprite cell(m_emptyTexture);
                    cell.setScale(glm::vec2(cellSize, cellSize));
                    cell.setPosition(transformComponent.position
                                     + glm::vec2(i, j) * (cellSize + indent) - size / 2.f + indent);
                    cell.setColor(inventoryComponent.cellColor);
                    batch.draw(cell, 100);
                }
            }

            Text text(*inventoryComponent.font, inventoryComponent.title);
            auto textBounds = text.getLocalBounds();
            text.setPosition(transformComponent.position
                             - glm::vec2(textBounds.getWidth(), textBounds.getHeight() - size.y - indent) / 2.f);
            text.draw(batch, 100);
        }
    }
}
