#ifndef COORDINATE_H
#define COORDINATE_H

#include <glm/vec2.hpp>
#include "WorldConst.h"

namespace utils
{
    inline glm::ivec3 toChunkPos(const glm::ivec3& globalTilePos)
    {
        int x = globalTilePos.x;
        int y = globalTilePos.y;
        int z = globalTilePos.z;

        return 
        {
            x < 0 ? ((x - CHUNK_SIZE + 1) / CHUNK_SIZE) : (x / CHUNK_SIZE),
            y < 0 ? ((y - CHUNK_SIZE + 1) / CHUNK_SIZE) : (y / CHUNK_SIZE),
            z < 0 ? ((z - CHUNK_SIZE + 1) / CHUNK_SIZE) : (z / CHUNK_SIZE)
        };
    }

    inline glm::ivec3 toLocalTilePos(const glm::ivec3& globalTilePos)
    {
        int x = globalTilePos.x;
        int y = globalTilePos.y;
        int z = globalTilePos.z;

        return
        {
            ((CHUNK_SIZE + (x % CHUNK_SIZE)) % CHUNK_SIZE),
            ((CHUNK_SIZE + (y % CHUNK_SIZE)) % CHUNK_SIZE),
            ((CHUNK_SIZE + (z % CHUNK_SIZE)) % CHUNK_SIZE)
        };
    }

    inline glm::ivec3 toGlobalTilePos(const glm::ivec3& locallTilePos, const glm::ivec3& chunkPos)
    {
        return 
        {
            (locallTilePos.x + chunkPos.x * CHUNK_SIZE),
            (locallTilePos.y + chunkPos.y * CHUNK_SIZE),
            (locallTilePos.z + chunkPos.z * CHUNK_SIZE)
        };
    }

    inline std::size_t toLocalTileIndex(const glm::ivec2& localTilePos)
    {
        return localTilePos.y * CHUNK_SIZE + localTilePos.x;
    }
} // namespace map

#endif