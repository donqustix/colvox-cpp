#ifndef CHUNKLOCATIONHASH_H
#define CHUNKLOCATIONHASH_H

#include "chunk_location.h"

#include <functional>

namespace std
{
    template<>
    struct hash<minecpp::ChunkLocation>
    {
        size_t operator()(const minecpp::ChunkLocation& chunkLocation) const noexcept;
    };
}

#endif
