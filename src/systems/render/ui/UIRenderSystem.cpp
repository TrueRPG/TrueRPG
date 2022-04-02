#include "UIRenderSystem.h"

#include "glm/vec2.hpp"
#include "../../../client/window/Window.h"

UIRenderSystem::UIRenderSystem(entt::registry &registry)
    : m_registry(registry)
{
}

UIRenderSystem::~UIRenderSystem()
{
    for (const auto &system : m_subsystems)
    {
        delete system;
    }
}

void UIRenderSystem::draw(SpriteBatch &batch)
{
    // UI rendering
    Window &window = Window::getInstance();
    glm::vec2 cursor = window.getCursorPosition();
    cursor = cursor - glm::vec2(window.getWidth(), window.getHeight()) / 2.f; // the origin is the center of the screen now
    cursor = glm::vec2(-cursor.x, cursor.y); // change the direction of x-axis
    cursor = batch.getViewMatrix() * -glm::vec4(cursor, 0.f, 1.f); // convert it to world coords

    for (auto &item : m_subsystems)
    {
        item->draw(batch, cursor);
    }
}
