#include "../../pch.h"
#include "SpriteRenderSystem.h"

#include "../../components/render/SpriteRendererComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/render/AutoOrderComponent.h"

SpriteRenderSystem::SpriteRenderSystem(entt::registry &registry)
    : m_registry(registry)
{
}

void SpriteRenderSystem::draw(SpriteBatch &batch)
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

        batch.draw(sprite, spriteComponent.layer, order);
    }
}
