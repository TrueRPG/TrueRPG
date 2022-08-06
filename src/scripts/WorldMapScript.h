#ifndef RPG_WORLDMAPSCRIPT_H
#define RPG_WORLDMAPSCRIPT_H

#include <vector>
#include "../scene/Script.h"
#include "../components/world/WorldMapComponent.h"
#include "../client/graphics/Rect.h"
#include "../utils/OpenSimplexNoise.h"
#include "../client/Engine.h"

#define DEBUG_SEED 2

class WorldMapGenerator : public IWorldMapGenerator
{
    OpenSimplexNoise m_simplexNoise;

    Texture& m_texture;

    Tile sandTile{&m_texture, IntRect(192, 4224, 32, 32)};
    Tile grassTile{&m_texture, IntRect(96, 4224, 32, 32)};
    Tile dirtTile{&m_texture, IntRect(160, 4224, 32, 32)};
    Tile mushroomTile{&m_texture, IntRect(0, 4000, 32, 32)};

    Object treeObject{
            &m_texture, IntRect(64, 4160, 64, 64), glm::vec2(16, -8), 20
    };
    Object bushObject{
            &m_texture, IntRect(32, 4064, 32, 32), glm::vec2(0.f), 4
    };

    Entity m_player;

public:
    WorldMapGenerator(Texture& texture, Entity player)
            : m_simplexNoise(DEBUG_SEED), m_texture(texture), m_player(player) {}

    std::vector<Tile> generateTiles(int x, int y) override
    {
        auto &transform = m_player.getComponent<TransformComponent>();

        double value = m_simplexNoise.getNoise(x, y);

        if (value > 0.3f)
        {
            return {dirtTile};
        }
        if (value > -0.2f)
        {
            if ((int) (value * 10000) % 83 == 1)
            {
                return {grassTile, mushroomTile};
            }
            return {grassTile};
        }
        return {sandTile};
    }

    std::vector<Object> generateObjects(int x, int y, std::vector<Tile> tiles) override
    {
        double value = m_simplexNoise.getNoise(x, y);

        if (value > -0.15f && value < 0.3f && tiles.size() == 1)
        {
            if ((int) (value * 10000) % 11 == 1)
            {
                return {treeObject};
            }
            else if ((int) (value * 10000) % 31 == 1)
            {
                return {bushObject};
            }
        }
        return {};
    }
};

class WorldMapScript : public Script
{
private:
    WorldMapComponent *m_worldMap{};
    TransformComponent *m_worldTransform{};
    WorldMapGenerator m_worldMapGenerator;
public:
    WorldMapScript(Texture& texture, Entity player)
            : m_worldMapGenerator(texture, player) {}

    void onCreate() override
    {
        m_worldMap = &getComponent<WorldMapComponent>();
        m_worldTransform = &getComponent<TransformComponent>();
        m_worldMap->generator = &m_worldMapGenerator;
    }

    void onUpdate(float deltaTime) override
    {
        IWindow &window = Engine::getWindow();
        float radius = std::max(window.getWidth(), window.getHeight()) / 2;
        float scale = std::max(m_worldTransform->scale.x, m_worldTransform->scale.y);
        m_worldMap->renderRadius = radius / (scale * m_worldMap->tileSize) + 3;
    }
};

#endif //RPG_WORLDMAPSCRIPT_H
