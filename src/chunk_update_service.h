#ifndef CHUNKUPDATESERVICE_H
#define CHUNKUPDATESERVICE_H

#include "mesh_chunk_vertices.h"
#include "voxel_update.h"
#include "service.h"

#include <queue>

namespace minecpp
{
    class Chunk;
    class MeshChunkBuilder;

    class ChunkUpdateService : public Service
    {
    public:
        struct ChunkUpdate
        {
            std::vector<VoxelUpdate> voxelsUpdate;
            Chunk* chunk;
        };

        struct ChunkBuffer
        {
            MeshChunkVertices meshVertices;
            Chunk* chunk;
        };

    private:
        std::queue<ChunkUpdate> chunksUpdate;
        std::queue<ChunkBuffer> chunksBuffer;

        MeshChunkBuilder* meshBuilder;
        
        void working() noexcept override;
        bool waiting() const noexcept override {return chunksUpdate.empty();}

    public:
        explicit ChunkUpdateService(MeshChunkBuilder& meshBuilder) noexcept;
        ~ChunkUpdateService();

        void push(const ChunkUpdate& chunkUpdate) noexcept;

        std::queue<ChunkBuffer> moveChunksBuffer() noexcept;
    };
}

#endif
