#include "TileMapComponent.h"
#include "../../utils/Coordinate.h"

TileMapComponent::TileMapComponent(const IntRect& startBounds) : m_bounds(startBounds) { }

void TileMapComponent::setTilePallet(TilePallet* pallet)
{
    tilePallet = pallet;
}

const TilePallet* TileMapComponent::getPallet() const
{
    return tilePallet;
}

const IntRect& TileMapComponent::getBounds() const
{
    return m_bounds;
}

const IntRect TileMapComponent::getGlobalBounds() const
{
    return m_bounds * CHUNK_SIZE;
}

bool TileMapComponent::hasTile(const glm::ivec3& tilePosition) const
{
    auto chItr = m_chunks.find(utils::toChunkPos(tilePosition));
    if (chItr == m_chunks.cend())
    {
        return false;
    }

    return m_chunks.at(utils::toChunkPos(tilePosition))
        .hasTile(utils::toLocalTilePos(tilePosition));
}

u8 TileMapComponent::getTile(const glm::ivec3& tilePosition) const
{
    auto chItr = m_chunks.find(utils::toChunkPos(tilePosition));
    if (chItr == m_chunks.cend())
    {
        return 0;
    }

    return m_chunks.at(utils::toChunkPos(tilePosition))
        .getTile(utils::toLocalTilePos(tilePosition));
}

void TileMapComponent::setTile(const glm::ivec3& position, const Tile& tile)
{
    auto chItr = m_chunks.find(utils::toChunkPos(position));
    if (chItr == m_chunks.cend())
    {
        m_chunks[utils::toChunkPos(position)] = Chunk(utils::toChunkPos(position));
    }

    m_chunks.at(utils::toChunkPos(position))
        .setTile(utils::toLocalTilePos(position), tile);
}
