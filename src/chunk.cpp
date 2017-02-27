#include "chunk.h"
#include "chunk_model.h"
#include "voxel.h"

using minecpp::Chunk;

Chunk::Chunk(const ChunkLocation& location, ResourceContainer& resourceContainer) noexcept :
    model{std::make_unique<ChunkModel>(location, resourceContainer)}, location{location}
{
}

Chunk::Chunk(Chunk&&) noexcept = default;

Chunk::~Chunk() = default;

Chunk& Chunk::operator=(Chunk&&) noexcept = default;

const Chunk* Chunk::findChunk(int x, int y, int z, ptrdiff_t& index) const noexcept
{
    if (x >= 0 && x < constants::CHUNK_WIDTH &&
        z >= 0 && z < constants::CHUNK_WIDTH && y >= 0 && y < constants::CHUNK_HEIGHT)
    {
        index = Chunk::getVoxelIndex(x, y, z);

        return this;
    }

    if (x < 0)
    {
        if (neightbors.left)
        {
            return neightbors.left->findChunk(constants::CHUNK_WIDTH + x, y, z, index);
        }
    }
    else if (x >= constants::CHUNK_WIDTH)
    {
        if (neightbors.right)
        {
            return neightbors.right->findChunk(x - constants::CHUNK_WIDTH, y, z, index);
        }
    }

    if (z < 0)
    {
        if (neightbors.back)
        {
            return neightbors.back->findChunk(x, y, constants::CHUNK_WIDTH + z, index);
        }
    }
    else if (z >= constants::CHUNK_WIDTH)
    {
        if (neightbors.front)
        {
            return neightbors.front->findChunk(x, y, z - constants::CHUNK_WIDTH, index);
        }
    }

    return nullptr;
}

Chunk* Chunk::findChunk(int x, int y, int z, ptrdiff_t& index) noexcept
{
    return const_cast<Chunk*>(const_cast<const Chunk*>(this)->findChunk(x, y, z, index));
}

unsigned Chunk::findVoxel(int x, int y, int z) const noexcept
{
    ptrdiff_t index;
    const Chunk* const chunkFound = findChunk(x, y, z, index);

    if (chunkFound)
        return *(chunkFound->voxels + index);

    return voxel::Types::NUM_TYPES;
}

unsigned Chunk::findLight(int x, int y, int z) const noexcept
{
    ptrdiff_t index;
    const Chunk* const chunkFound = findChunk(x, y, z, index);

    if (chunkFound)
        return *(chunkFound->lights + index);

    return 0;
}

