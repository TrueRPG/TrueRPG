#include "WorldMapRenderSystem.h"

#include "../../components/world/WorldMapComponent.h"
#include "../../utils/Hierarchy.h"
#include "../../components/render/CameraComponent.h"

WorldMapRenderSystem::WorldMapRenderSystem(entt::registry &registry)
    : m_registry(registry)
{
}

void WorldMapRenderSystem::draw(SpriteBatch &batch)
{
    auto cameraView = m_registry.view<CameraComponent>();
    auto cameraTransform = Hierarchy::computeTransform({cameraView[0], &m_registry});

    auto worldView = m_registry.view<WorldMapComponent>();
    for (auto entity : worldView)
    {
        auto &worldMapComponent = worldView.get<WorldMapComponent>(entity);
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

                    batch.draw(tileSprite, worldMapComponent.tileLayer);
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

                    batch.draw(objectSprite, worldMapComponent.objectLayer,-(int) objectSprite.getPosition().y - object.orderPivot);
                }
            }
        }
    }
}
