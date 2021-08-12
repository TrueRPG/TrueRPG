#include "Chunk.h"
#include "../../utils/Coordinate.h"
#include <cstring>


Chunk::Chunk() : m_pos(0, 0, 0), m_size(CHUNK_SIZE) { }

    // TODO: Отрефакторить конструтор. Сделать генерацию в отдельном объекте
Chunk::Chunk(const glm::ivec3& pos) : m_pos(pos), m_size(CHUNK_SIZE) 
{
    std::memset(m_tiles, 0, sizeof(u8) * CHUNK_VOLUME);
}

const TileArray& Chunk::getTiles() const
{
    return m_tiles;
}

bool Chunk::hasTile(const glm::ivec2 tilePos) const
{
    return &m_tiles[utils::toLocalTileIndex(tilePos)] != nullptr;
}

const glm::ivec3& Chunk::getPosition() const
{
    return m_pos;
}

u8 Chunk::getTile(const glm::ivec2& tilePos) const
{
    return m_tiles[utils::toLocalTileIndex(tilePos)];
}

void Chunk::setTile(const glm::ivec2& tilePos, const Tile& tile)
{
    m_tiles[utils::toLocalTileIndex(tilePos)] = tile.getId();
}
