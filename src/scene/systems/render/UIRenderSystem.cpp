#include "UIRenderSystem.h"
#include "glm/vec2.hpp"
#include "../../../client/window/Window.h"
#include "../../components/render/ui/ButtonComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../../client/graphics/Text.h"

UIRenderSystem::UIRenderSystem(entt::registry &registry)
    : m_registry(registry),
      m_emptyTexture(Texture::createEmpty())
{
}

static bool isRectSelected(FloatRect rect, glm::vec2 cursor)
{
    return cursor.x > rect.getLeft() && cursor.x < rect.getLeft() + rect.getWidth()
           && cursor.y > rect.getBottom() && cursor.y < rect.getBottom() + rect.getHeight();
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
