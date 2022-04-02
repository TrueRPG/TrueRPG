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
    glm::vec4 color{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 highlightedColor{0.7f, 0.7f, 0.7f, 1.f};
    glm::vec4 pressedColor{0.5f, 0.5f, 0.5f, 1.f};

    Window &window = Window::getInstance();
    auto view = m_registry.view<ButtonComponent>();
    for (auto entity : view)
    {
        auto &buttonComponent = view.get<ButtonComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        Sprite sprite;
        sprite.setScale(buttonComponent.size);
        sprite.setPosition(transformComponent.position);
        sprite.setColor(color);

        if (isRectSelected(sprite.getGlobalBounds(), cursor) && buttonComponent.enabled)
        {
            // TODO: hardcoded mouse button
            if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
            {
                sprite.setColor(pressedColor);
            }
            else
            {
                sprite.setColor(highlightedColor);
            }
        }

        batch.draw(sprite, 100);

        Text text(*buttonComponent.font, buttonComponent.text);
        auto textBounds = text.getLocalBounds();
        text.setPosition(
            transformComponent.position + buttonComponent.size / 2.f - glm::vec2(textBounds.getWidth(), textBounds.getHeight()) / 2.f);
        text.draw(batch, 100);
    }
}
