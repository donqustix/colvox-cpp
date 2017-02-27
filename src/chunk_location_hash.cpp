#include "chunk_location_hash.h"
#include "utility.h"

using minecpp::ChunkLocation;

size_t std::hash<ChunkLocation>::operator()(const ChunkLocation& chunkLocation) const noexcept
{
    size_t hash = 0;
    minecpp::utility::hash_combine(hash, chunkLocation.getX());
    minecpp::utility::hash_combine(hash, chunkLocation.getZ());
    return hash;
}

