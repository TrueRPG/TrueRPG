#ifndef RPG_WORLDMAPSCRIPT_H
#define RPG_WORLDMAPSCRIPT_H

#include "../scene/Script.h"
#include "../utils/OpenSimplexNoise.h"
#include "../components/world/WorldMapComponent.h"

#define DEBUG_SEED 2

class WorldMapGenerator : public IWorldMapGenerator
{
    OpenSimplexNoise m_simplexNoise;

    Texture &m_texture;

    Tile sandTile{&m_texture, IntRect(192, 4224, 32, 32)};
    Tile grassTile{&m_texture, IntRect(96, 4224, 32, 32)};
    Tile dirtTile{&m_texture, IntRect(160, 4224, 32, 32)};
    Tile mushroomTile{&m_texture, IntRect(0, 4000, 32, 32)};

    Object treeObject{&m_texture, IntRect(64, 4160, 64, 64), glm::vec2(16, -8), 20};
    Object bushObject{&m_texture, IntRect(32, 4064, 32, 32), glm::vec2(0.f), 4};

    Entity m_player;

public:
    WorldMapGenerator(Texture &texture, Entity player);

    std::vector<Tile> generateTiles(int x, int y) override;
    std::vector<Object> generateObjects(int x, int y, std::vector<Tile> tiles) override;
};

class WorldMapScript : public Script
{
private:
    WorldMapComponent *m_worldMap{};
    TransformComponent *m_worldTransform{};
    WorldMapGenerator m_worldMapGenerator;

public:
    WorldMapScript(Texture &texture, Entity player);

    void onCreate() override;
    void onUpdate(float deltaTime) override;
};

#endif // RPG_WORLDMAPSCRIPT_H
