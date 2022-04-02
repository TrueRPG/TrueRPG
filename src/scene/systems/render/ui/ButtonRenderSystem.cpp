#include "ButtonRenderSystem.h"

#include "../../../../client/window/Window.h"
#include "../../../components/render/ui/ButtonComponent.h"
#include "../../../utils/Hierarchy.h"
#include "../../../../client/graphics/Text.h"

ButtonRenderSystem::ButtonRenderSystem(entt::registry& registry)
    : m_registry(registry)
{
}

static bool isRectSelected(FloatRect rect, glm::vec2 cursor)
{
    return cursor.x > rect.getLeft() && cursor.x < rect.getLeft() + rect.getWidth() && cursor.y > rect.getBottom() &&
           cursor.y < rect.getBottom() + rect.getHeight();
}

void ButtonRenderSystem::draw(SpriteBatch &batch, glm::vec2 cursor)
{
    Window &window = Window::getInstance();
    auto view = m_registry.view<ButtonComponent>();
    for (auto entity : view)
    {
        auto &buttonComponent = view.get<ButtonComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        Sprite sprite;
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
