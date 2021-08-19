#include "RenderSystem.h"

#include <entt.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../client/window/Window.h"
#include "../../client/graphics/Text.h"
#include "../components/CameraComponent.h"
#include "../components/SpriteRendererComponent.h"
#include "../components/TextRendererComponent.h"
#include "../components/HierarchyComponent.h"
#include "../utils/Hierarchy.h"
#include "../components/WorldMapComponent.h"

// всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void resizeCallback(Window *window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, что высота и ширина будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}

RenderSystem::RenderSystem(entt::registry &registry)
        : m_registry(registry),
          m_shader(Shader::createShader("../res/shaders/shader.vs", "../res/shaders/shader.fs")),
          m_batch(m_shader, 30000)
{
    Window::getInstance().setResizeCallback(resizeCallback);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

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
                        std::vector<Object> objects = worldMapComponent.generator->generateObjects(x, y);
                        for (const auto &object : objects)
                        {
                            Sprite objectSprite(*object.texture);
                            objectSprite.setTextureRect(object.textureRect);
                            objectSprite.setPosition(glm::vec2(x, y) * (float) worldMapComponent.tileSize * transformComponent.scale);
                            objectSprite.setOrigin(object.origin);
                            objectSprite.setScale(transformComponent.scale);

                            m_batch.draw(objectSprite, worldMapComponent.objectLayer,
                                         (-(int) objectSprite.getPosition().y - object.orderPivot) * (int) transformComponent.scale.y);
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

                m_batch.draw(sprite, spriteComponent.layer, spriteComponent.order);
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
