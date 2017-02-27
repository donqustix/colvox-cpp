#ifndef CHUNKGENERATOR_H
#define CHUNKGENERATOR_H

namespace minecpp
{
    class ChunkLocation;
    class Chunk;
    class ResourceContainer;

    struct ChunkGenerator
    {
        virtual ~ChunkGenerator() = default;
        virtual Chunk* generate(const ChunkLocation& chunkLocaton, ResourceContainer& resourceContainer) noexcept = 0;
    };
}

#endif
