#ifndef RPG_WORLDMAPCOMPONENT_H
#define RPG_WORLDMAPCOMPONENT_H

#include <unordered_map>
#include <vector>
#include "glm/vec2.hpp"
#include "../../../client/graphics/Texture.h"
#include "../../../client/graphics/Rect.h"

struct Tile
{
    Texture *texture{};
    IntRect textureRect{0, 0, 0, 0};
};

struct Object
{
    Texture *texture{};
    IntRect textureRect{0, 0, 0, 0};

    glm::vec2 origin{};

    int orderPivot{0};
};

class IWorldMapGenerator
{
public:
    virtual ~IWorldMapGenerator() = default;

    virtual std::vector<Tile> generateTiles(int x, int y) = 0;

    virtual std::vector<Object> generateObjects(int x, int y, std::vector<Tile>) = 0;
};

struct WorldMapComponent
{
    int tileSize{32};

    IWorldMapGenerator *generator;

    int renderRadius{12};

    int tileLayer{0};
    int objectLayer{1};
};

#endif //RPG_WORLDMAPCOMPONENT_H
