#ifndef CHUNKLOADSERVICE_H
#define CHUNKLOADSERVICE_H

#include "chunk_location.h"
#include "service.h"

#include <memory>
#include <queue>

namespace minecpp
{
    class ChunkGenerator;
    class Chunk;
    class ResourceContainer;

    class ChunkLoadService : public Service
    {
        std::queue<ChunkLocation> chunksLoad;
        std::queue<Chunk*> chunksBind;

        std::unique_ptr<ChunkGenerator> chunkGenerator;

        ResourceContainer* resourceContainer;

        void working() noexcept override;
        bool waiting() const noexcept override {return chunksLoad.empty();}

    public:
        explicit ChunkLoadService(ResourceContainer& resourceContainer) noexcept;
        ~ChunkLoadService();

        void load(const ChunkLocation& chunkLocation) noexcept;

        std::queue<Chunk*> moveChunksBind() noexcept;
    };
}

#endif
