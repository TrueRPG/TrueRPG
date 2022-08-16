#include "../pch.h"
#include "WorldMapScript.h"

#include "../client/graphics/Rect.h"
#include "../client/Engine.h"
#include <vector>

WorldMapGenerator::WorldMapGenerator(Texture &texture, Entity player)
    : m_simplexNoise(DEBUG_SEED),
      m_texture(texture),
      m_player(player)
{
}

std::vector<Tile> WorldMapGenerator::generateTiles(int x, int y)
{
    auto &transform = m_player.getComponent<TransformComponent>();

    double value = m_simplexNoise.getNoise(x, y);

    if (value > 0.3f)
    {
        return {dirtTile};
    }
    if (value > -0.2f)
    {
        if ((int)(value * 10000) % 83 == 1)
        {
            return {grassTile, mushroomTile};
        }
        return {grassTile};
    }
    return {sandTile};
}

std::vector<Object> WorldMapGenerator::generateObjects(int x, int y, std::vector<Tile> tiles)
{
    double value = m_simplexNoise.getNoise(x, y);

    if (value > -0.15f && value < 0.3f && tiles.size() == 1)
    {
        if ((int)(value * 10000) % 11 == 1)
        {
            return {treeObject};
        }
        else if ((int)(value * 10000) % 31 == 1)
        {
            return {bushObject};
        }
    }
    return {};
}

WorldMapScript::WorldMapScript(Texture &texture, Entity player)
    : m_worldMapGenerator(texture, player)
{
}

void WorldMapScript::onCreate()
{
    m_worldMap = &getComponent<WorldMapComponent>();
    m_worldTransform = &getComponent<TransformComponent>();
    m_worldMap->generator = &m_worldMapGenerator;
}

void WorldMapScript::onUpdate(float deltaTime)
{
    IWindow &window = Engine::getWindow();
    float radius = std::max(window.getWidth(), window.getHeight()) / 2;
    float scale = std::max(m_worldTransform->scale.x, m_worldTransform->scale.y);
    m_worldMap->renderRadius = radius / (scale * m_worldMap->tileSize) + 3;
}
