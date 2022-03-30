#include "RenderSystem.h"

#include <entt.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../client/window/Window.h"
#include "../../client/graphics/Text.h"
#include "../components/render/CameraComponent.h"
#include "../components/render/SpriteRendererComponent.h"
#include "../components/render/TextRendererComponent.h"
#include "../components/basic/HierarchyComponent.h"
#include "../utils/Hierarchy.h"
#include "../components/world/WorldMapComponent.h"
#include "../components/render/AutoOrderComponent.h"
#include "../components/render/ui/ButtonComponent.h"

RenderSystem::RenderSystem(entt::registry &registry)
        : m_registry(registry),
          m_shader(Shader::createShader("../res/shaders/shader.vs", "../res/shaders/shader.fs")),
          m_batch(m_shader, 30000),
          m_emptyTexture(Texture::createEmpty())
{
    Window::getInstance().onResize += createEventHandler(&RenderSystem::resize);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static bool isRectSelected(FloatRect rect, glm::vec2 cursor)
{
    return cursor.x > rect.getLeft() && cursor.x < rect.getLeft() + rect.getWidth()
           && cursor.y > rect.getBottom() && cursor.y < rect.getBottom() + rect.getHeight();
}

// TODO: This method is too big
void RenderSystem::draw()
{
    // Find the camera
    CameraComponent *cameraComponent = nullptr;
    TransformComponent cameraTransform;
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            cameraComponent = &view.get<CameraComponent>(entity);
            cameraTransform = Hierarchy::computeTransform({entity, &m_registry});
        }
    }
    if (cameraComponent != nullptr)
    {
        glm::vec4 back = cameraComponent->background;
        glClearColor(back.r, back.g, back.b, back.a);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::translate(glm::mat4(1), glm::vec3(-cameraTransform.position, 0));
        m_batch.setViewMatrix(viewMatrix);
        m_batch.setProjectionMatrix(cameraComponent->getProjectionMatrix());
        m_batch.begin();

        // World map rendering
        {
            auto view = m_registry.view<WorldMapComponent>();
            for (auto entity : view)
            {
                auto &worldMapComponent = view.get<WorldMapComponent>(entity);
                auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

                int currentX = (int) std::floor(cameraTransform.position.x / ((float) worldMapComponent.tileSize * transformComponent.scale.x));
                int currentY = (int) std::floor(cameraTransform.position.y / ((float) worldMapComponent.tileSize * transformComponent.scale.y));

                for (int y = currentY + worldMapComponent.renderRadius - 1; y >= currentY - worldMapComponent.renderRadius + 1; y--)
                {
                    for (int x = currentX - worldMapComponent.renderRadius + 1; x < currentX + worldMapComponent.renderRadius; x++)
                    {
                        // Generate tiles
                        std::vector<Tile> tiles = worldMapComponent.generator->generateTiles(x, y);
                        for (const auto &tile : tiles)
                        {
                            Sprite tileSprite(*tile.texture);
                            tileSprite.setTextureRect(tile.textureRect);
                            tileSprite.setPosition(glm::vec2(x, y) * (float) worldMapComponent.tileSize * transformComponent.scale);
                            tileSprite.setScale(transformComponent.scale);

                            m_batch.draw(tileSprite, worldMapComponent.tileLayer);
                        }
                        // Generate objects
                        std::vector<Object> objects = worldMapComponent.generator->generateObjects(x, y, tiles);
                        for (const auto &object : objects)
                        {
                            Sprite objectSprite(*object.texture);
                            objectSprite.setTextureRect(object.textureRect);
                            objectSprite.setPosition(glm::vec2(x, y) * (float) worldMapComponent.tileSize * transformComponent.scale);
                            objectSprite.setOrigin(object.origin);
                            objectSprite.setScale(transformComponent.scale);

                            m_batch.draw(objectSprite, worldMapComponent.objectLayer,-(int) objectSprite.getPosition().y - object.orderPivot);
                        }
                    }
                }
            }
        }

        // Sprites rendering
        {
            auto view = m_registry.view<SpriteRendererComponent>();
            for (auto entity : view)
            {
                auto &spriteComponent = view.get<SpriteRendererComponent>(entity);
                Sprite sprite(spriteComponent.texture);
                sprite.setTextureRect(spriteComponent.textureRect);
                sprite.setColor(spriteComponent.color);

                auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

                sprite.setPosition(transformComponent.position);
                sprite.setOrigin(transformComponent.origin);
                sprite.setScale(transformComponent.scale);

                int order = spriteComponent.order;
                if (m_registry.all_of<AutoOrderComponent>(entity))
                {
                    auto &orderComponent = m_registry.get<AutoOrderComponent>(entity);
                    order = -(int) transformComponent.position.y - orderComponent.orderPivot;
                }

                m_batch.draw(sprite, spriteComponent.layer, order);
            }
        }

        // UI rendering
        // TODO: It's not ready yet
        {
            Window &window = Window::getInstance();
            glm::vec2 cursor = window.getCursorPosition();
            cursor = cursor - glm::vec2(window.getWidth(), window.getHeight()) / 2.f; // the origin is the center of the screen now
            cursor = glm::vec2(-cursor.x, cursor.y); // change the direction of x-axis
            cursor = viewMatrix * -glm::vec4(cursor, 0.f, 1.f); // convert it to world coords

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
                m_batch.draw(sprite, 100);

                Text text(*buttonComponent.font, buttonComponent.text);
                auto textBounds = text.getLocalBounds();
                text.setPosition(transformComponent.position + buttonComponent.size / 2.f - glm::vec2(textBounds.getWidth(), textBounds.getHeight()) / 2.f);
                text.draw(m_batch, 100);
            }
        }

        // Text rendering
        {
            auto view = m_registry.view<TextRendererComponent>();
            for (auto entity : view)
            {
                auto &textComponent = view.get<TextRendererComponent>(entity);
                Text text(*textComponent.font, textComponent.text);
                text.setColor(textComponent.color);

                auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});

                text.setPosition(transformComponent.position);
                FloatRect localBound = text.getLocalBounds();
                glm::vec2 textOrigin = transformComponent.origin;
                if (textComponent.horizontalAlign == HorizontalAlign::Center)
                {
                    textOrigin += glm::vec2(localBound.getWidth() / 2, 0.f);
                }
                if (textComponent.horizontalAlign == HorizontalAlign::Right)
                {
                    textOrigin += glm::vec2(localBound.getWidth(), 0.f);
                }
                if (textComponent.verticalAlign == VerticalAlign::Center)
                {
                    textOrigin += glm::vec2(0.f, localBound.getHeight() / 2);
                }
                if (textComponent.verticalAlign == VerticalAlign::Top)
                {
                    textOrigin += glm::vec2(0.f, localBound.getHeight());
                }
                text.setOrigin(textOrigin);
                text.setScale(transformComponent.scale);

                text.draw(m_batch, textComponent.layer, textComponent.order);
            }
        }
        m_batch.end();
    }
}

void RenderSystem::destroy()
{
    m_batch.destroy();
    m_shader.destroy();
}

// Every time when the window size is changed (by user or OS), this callback function is invoked
void RenderSystem::resize(int width, int height)
{
    glViewport(0, 0, width, height);
}
