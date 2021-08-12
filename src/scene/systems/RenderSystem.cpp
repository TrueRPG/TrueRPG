#include "RenderSystem.h"

#include <entt.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../client/window/Window.h"
#include "../../client/graphics/Text.h"
#include "../components/CameraComponent.h"
#include "../components/TileMapComponent.h"
#include "../components/SpriteRendererComponent.h"
#include "../components/TextRendererComponent.h"
#include "../components/HierarchyComponent.h"
#include "../utils/Hierarchy.h"

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
    // TODO: Пока не очень понятно, как быть с приоритетом отрисовки.
    //  Например, если нам нужно нарисовать спрайт поверх текста.

    // Находим камеру
    glm::mat4 viewMatrix(1.f);
    CameraComponent *cameraComponent = nullptr;
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            cameraComponent = &view.get<CameraComponent>(entity);
            auto transformComponent = Hierarchy::computeTransform({entity, &m_registry});
            viewMatrix = glm::translate(glm::mat4(1), glm::vec3(-transformComponent.position, 0));
        }
    }
    if (cameraComponent != nullptr)
    {
        glm::vec4 back = cameraComponent->background;
        glClearColor(back.r, back.g, back.b, back.a);
        glClear(GL_COLOR_BUFFER_BIT);

        m_batch.setViewMatrix(viewMatrix);
        m_batch.setProjectionMatrix(cameraComponent->getProjectionMatrix());
        m_batch.begin();

        // TODO: добавить отсечение невидимых тайлов.
        // Рендеринг карты тайлов
        {
            auto view = m_registry.view<TileMapComponent>();
            for (auto entity : view)
            {
                auto &tilemapComponent = view.get<TileMapComponent>(entity);
                for (auto &chunkPair : tilemapComponent.m_chunks)
                {
                    auto &chunk = chunkPair.second;
                    for (size_t y = 0; y < CHUNK_SIZE; y++)
                    {
                        for (size_t x = 0; x < CHUNK_SIZE; x++)
                        {
                            u8 tileId = chunk.getTile({x, y});
                            Sprite tSprite(*tilemapComponent.getPallet()->getTexture());
                            tSprite.setTextureRect(tilemapComponent.getPallet()->getTile(tileId).getRect());
                            tSprite.setScale(tilemapComponent.getPallet()->getCellScale());
                            tSprite.setOrigin(tilemapComponent.getPallet()->getCellOrigin());

                            int realX = chunk.getPosition().x * CHUNK_SIZE + x;
                            int realY = chunk.getPosition().y * CHUNK_SIZE + y;

                            tSprite.setPosition({
                                                    realX * tSprite.getGlobalBounds().getWidth(),
                                                    realY * tSprite.getGlobalBounds().getHeight()
                                                });

                            m_batch.draw(tSprite);
                        }
                    }
                }
            }
        }

        // Рендеринг спрайтов
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

                m_batch.draw(sprite);
            }
        }

        // Рендеринг текста
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

                text.draw(m_batch);
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
