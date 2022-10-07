#include "../../../pch.h"
#include "ButtonRenderSystem.h"

#include "../../../components/render/ui/ButtonComponent.h"
#include "../../../utils/Hierarchy.h"
#include "../../../client/graphics/Text.h"
#include "../../../client/Engine.h"
#include "GLFW/glfw3.h"

ButtonRenderSystem::ButtonRenderSystem(entt::registry& registry)
    : m_registry(registry)
{
}

void ButtonRenderSystem::draw(SpriteBatch &batch, glm::vec2 cursor)
{
    glm::vec4 color{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 highlightedColor{0.7f, 0.7f, 0.7f, 1.f};
    glm::vec4 pressedColor{0.5f, 0.5f, 0.5f, 1.f};

    IWindow &window = Engine::getWindow();
    auto view = m_registry.view<ButtonComponent>();
    for (auto entity : view)
    {
        auto &buttonComponent = view.get<ButtonComponent>(entity);

        auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

        Sprite sprite;
        sprite.setScale(buttonComponent.size);
        sprite.setPosition(transformComponent.position);
        sprite.setColor(color);

        if (sprite.getGlobalBounds().contains(cursor) && buttonComponent.enabled)
        {
            if (!hovered)
            {
                buttonComponent.onHover(hovered = true);
            }
            sprite.setColor(highlightedColor);
            // TODO: hardcoded mouse button
            if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
            {
                sprite.setColor(pressedColor);
                pressed = true;
            }
            if (!window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) && pressed)
            {
                pressed = false;
                buttonComponent.onClick();
            }
        }
        else if (hovered)
        {
            buttonComponent.onHover(hovered = false);
        }

        batch.draw(sprite, 10);

        Text text(*buttonComponent.font, buttonComponent.text);
        auto textBounds = text.getLocalBounds();
        text.setPosition(
            transformComponent.position + buttonComponent.size / 2.f - glm::vec2(textBounds.getWidth(), textBounds.getHeight()) / 2.f);
        text.draw(batch, 10);
    }
}
