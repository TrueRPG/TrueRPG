#include "UIRenderSystem.h"

#include "glm/vec2.hpp"
#include "../../../../client/window/Window.h"
#include "IUIRenderSubsystem.h"

UIRenderSystem::UIRenderSystem(entt::registry &registry)
    : m_registry(registry)
{
}

static bool isRectSelected(FloatRect rect, glm::vec2 cursor)
{
    return cursor.x > rect.getLeft() && cursor.x < rect.getLeft() + rect.getWidth() && cursor.y > rect.getBottom() &&
           cursor.y < rect.getBottom() + rect.getHeight();
}

void UIRenderSystem::addSubsystem(IUIRenderSubsystem& renderSystem)
{
    m_subsystems.push_back(&renderSystem);
}

void UIRenderSystem::draw(SpriteBatch &batch)
{
    // UI rendering
    Window &window = Window::getInstance();
    glm::vec2 cursor = window.getCursorPosition();
    cursor = cursor - glm::vec2(window.getWidth(), window.getHeight()) / 2.f; // the origin is the center of the screen now
    cursor = glm::vec2(-cursor.x, cursor.y); // change the direction of x-axis
    cursor = batch.getViewMatrix() * -glm::vec4(cursor, 0.f, 1.f); // convert it to world coords

    for (const auto &item : m_subsystems)
    {
        item->draw(batch, cursor);
    }
}
