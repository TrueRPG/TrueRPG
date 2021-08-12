#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "Tile.h"
#include "../../client/graphics/SpriteBatch.h"
#include "../../utils/WorldConst.h"
#include "../../utils/Types.h"


using TileArray = u8[CHUNK_VOLUME];

class Chunk
{
private:
    TileArray m_tiles;
    glm::ivec3 m_pos;
    int m_size;
public:
    Chunk();
    Chunk(const glm::ivec3& pos);
    ~Chunk() = default;
    
    /**
     * <p>Возвращает массив тайлов чанка</p>
     * 
     * @return размер тайлов чанка
    */
    const TileArray& getTiles() const;
    /**
     * <p>Возвращает позицию в мире</p>
     * 
     * @return позицию в карте тайлов
    */
    const glm::ivec3& getPosition() const;
    
    bool hasTile(const glm::ivec2 tilePos) const;
    
    /**
     * <p>Возвращает id тайла по позиции в чанке</p>
     * 
     * @param tilePos Позиция тайла в чанке
     * @return id тайла
    */
    u8 getTile(const glm::ivec2& tilePos) const;
    
    /**
     * <p>Возвращает id тайла по позиции в чанке</p>
     * 
     * @param tilePos Позиция тайла в чанке
     * @param tile устанавлеваемый тайл
    */
    void setTile(const glm::ivec2& tilePos, const Tile& tile);
};


#endif