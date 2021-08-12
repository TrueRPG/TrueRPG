#ifndef TILEMAPCOMPONENT_H
#define TILEMAPCOMPONENT_H

#include <unordered_map>
#include <glm/vec2.hpp>
#include "../../utils/Types.h"
#include "../resources/Chunk.h"
#include "../resources/TilePallet.h"

struct ChunkHash
{
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const glm::ivec3 position) const
    {
        return (position.x * 56554) ^ (position.y * 211287) ^ (position.z * 565114);
    }
};

using ChunkHashMap = std::unordered_map<glm::ivec3, Chunk, ChunkHash>;

class TileMapComponent
{
private:
    ChunkHashMap m_chunks;
    IntRect m_bounds;
    
    const TilePallet* tilePallet;

public:
    TileMapComponent(const IntRect& startBounds);

    void setTilePallet(TilePallet* pallet);

    const TilePallet* getPallet() const;

    const IntRect& getBounds() const;

    const IntRect getGlobalBounds() const;

    bool hasTile(const glm::ivec3& tilePosition) const;

    u8 getTile(const glm::ivec3& tilePosition) const;

    void setTile(const glm::ivec3& position, const Tile& tile);

    friend class RenderSystem;
};


#endif
