#ifndef CHUNKLOCATION_H
#define CHUNKLOCATION_H

#include "constants.h"

namespace minecpp
{
    class ChunkLocation
    {
        int x;
        int z;

    public:
        static constexpr ChunkLocation fromWorld(int x, int z) noexcept
        {
            return
            {
                x < 0 ? (x + 1) / constants::CHUNK_WIDTH - 1 : x / constants::CHUNK_WIDTH,
                z < 0 ? (z + 1) / constants::CHUNK_WIDTH - 1 : z / constants::CHUNK_WIDTH
            };
        }

        constexpr ChunkLocation(int x, int z) noexcept : x{x}, z{z} {}

        constexpr bool operator==(const ChunkLocation& chunkLocation) const noexcept
        {
            return x == chunkLocation.x && z == chunkLocation.z;
        }

        constexpr auto getX() const noexcept {return x;}
        constexpr auto getZ() const noexcept {return z;}
    };
}

#endif
