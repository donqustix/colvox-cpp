#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "chunk_location_hash.h"

#include <unordered_map>
#include <memory>
#include <queue>

namespace minecpp
{
    class RenderData;
    class ChunkLoadService;
    class ChunkUpdateService;
    class Chunk;
    class ResourceContainer;
    class MeshChunkBuilder;

    class ChunkManager
    {
        std::unordered_map<ChunkLocation, std::unique_ptr<Chunk>> chunks;

        std::unique_ptr<MeshChunkBuilder> meshBuilder;

        std::unique_ptr<ChunkUpdateService> updateService;
        std::unique_ptr<ChunkLoadService> loadService;

        std::queue<Chunk*> chunksUpdate;

        ResourceContainer* resourceContainer;

        void updateChunk(Chunk* chunk) noexcept;
        void updateChunkAndNeightbors(Chunk* chunk) noexcept;
        void bindChunk(Chunk* chunk) noexcept;

        void processBindingChunks() noexcept;
        void processUpdatingChunks() noexcept;
        void processBufferingChunks() noexcept;

    public:
        explicit ChunkManager(ResourceContainer& resourceContainer) noexcept;
        ChunkManager(ChunkManager&&);
        ~ChunkManager();

        ChunkManager& operator=(ChunkManager&&) noexcept;

        void update() noexcept;
        void render(const RenderData& renderData) const noexcept;

        void setVoxel(int x, int y, int z, unsigned voxel) noexcept;
    };
}

#endif
