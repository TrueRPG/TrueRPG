#include "TilePallet.h"

void TilePallet::setTexture(Texture* texture)
{
    m_texture = texture;
}

Texture* TilePallet::getTexture() const
{
    return m_texture;
}

void TilePallet::setCellSize(const glm::vec2& cellSize)
{
    m_cellSize = cellSize;
}

const glm::vec2& TilePallet::getCellSize() const
{
    return m_cellSize;
}

void TilePallet::setCellScale(const glm::vec2& cellScale)
{
    m_cellScale = cellScale;
}

const glm::vec2& TilePallet::getCellScale() const
{
    return m_cellScale;
}

void TilePallet::setCellOrigin(const glm::vec2& origin)
{
    m_origin = origin;
}

const glm::vec2& TilePallet::getCellOrigin() const
{
    return m_origin;
}

void TilePallet::addTile(const IntRect& rect)
{
    m_tiles.emplace_back(Tile(m_tiles.size(), {0, 0, 0, 0}, rect, false));
}

const Tile& TilePallet::getTile(u32 id) const
{
    return m_tiles.at(id);
}
