#include "chunk_update_service.h"
#include "chunk.h"
#include "chunk_model.h"
#include "mesh_chunk_builder.h"
#include "voxel.h"

using minecpp::ChunkUpdateService;

ChunkUpdateService::ChunkUpdateService(MeshChunkBuilder& meshBuilder) noexcept :
    meshBuilder{&meshBuilder}
{
}

ChunkUpdateService::~ChunkUpdateService()
{
    this->stopRunning();
}

void ChunkUpdateService::working() noexcept
{
    while (!chunksUpdate.empty())
    {
        std::unique_lock<std::mutex> guard{mx};
        
        const ChunkUpdate chunkUpdate{std::move(chunksUpdate.front())};
        chunksUpdate.pop();
        
        guard.unlock();

        for (const auto& u : chunkUpdate.voxelsUpdate)
        {
            switch (voxel::getType(u.voxel))
            {
                case voxel::Types::LIGHT:
                    meshBuilder->placeLight(chunkUpdate.chunk, u.index, voxel::getColor(u.voxel));
                    break;
                case voxel::Types::AIR:
                    meshBuilder->removeLight(chunkUpdate.chunk, u.index);
                    meshBuilder->removeSunLight(chunkUpdate.chunk, u.index);
                    break;
            }

            chunkUpdate.chunk->setVoxel(u.index, u.voxel);
        }

        if (!chunkUpdate.chunk->getModel().isBuilt())
            meshBuilder->prepareSunLights(chunkUpdate.chunk);

        meshBuilder->computeLights();

        MeshChunkVertices meshChunkVertices{meshBuilder->build(*chunkUpdate.chunk)};
        guard.lock();

        chunksBuffer.push({std::move(meshChunkVertices), chunkUpdate.chunk});
    }
}

void ChunkUpdateService::push(const ChunkUpdate& chunkUpdate) noexcept
{
    std::lock_guard<std::mutex> guard{mx};
    chunksUpdate.push(chunkUpdate);
}

std::queue<ChunkUpdateService::ChunkBuffer> ChunkUpdateService::moveChunksBuffer() noexcept
{
    std::lock_guard<std::mutex> guard{mx};
    return std::move(chunksBuffer);
}

