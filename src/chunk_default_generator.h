#ifndef CHUNKDEFAULTGENERATOR_H
#define CHUNKDEFAULTGENERATOR_H

#include "chunk_generator.h"
#include "constants.h"

namespace minecpp
{
    class ChunkDefaultGenerator : public ChunkGenerator
    {
    public:
        Chunk* generate(const ChunkLocation& chunkLocaton, ResourceContainer& resourceContainer) noexcept override;
    };
}

#endif
