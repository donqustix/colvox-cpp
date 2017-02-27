#include "chunk_manager.h"
#include "chunk_update_service.h"
#include "chunk_load_service.h"
#include "mesh_chunk_builder.h"
#include "chunk.h"
#include "chunk_model.h"

using minecpp::ChunkManager;

ChunkManager::ChunkManager(ResourceContainer& resourceContainer) noexcept :
    meshBuilder{std::make_unique<MeshChunkBuilder>()},
    updateService{std::make_unique<ChunkUpdateService>(*meshBuilder)},
    loadService{std::make_unique<ChunkLoadService>(resourceContainer)},
    resourceContainer{&resourceContainer}
{
    loadService->start();
    updateService->start();

    for (int x = 0; x < 30; ++x)
    {
        for (int z = 0; z < 30; ++z)
        {
            loadService->load({x, z});
        }
    }
    loadService->doWork();
}

ChunkManager::ChunkManager(ChunkManager&&) = default;

ChunkManager::~ChunkManager() = default;

ChunkManager& ChunkManager::operator=(ChunkManager&&) noexcept = default;

void ChunkManager::processBindingChunks() noexcept
{
    std::queue<Chunk*> chunksBind{loadService->moveChunksBind()};
    while (!chunksBind.empty())
    {
        Chunk* const chunk = chunksBind.front();
        chunksBind.pop();

        bindChunk(chunk);
    }
}

void ChunkManager::processUpdatingChunks() noexcept
{
    for (const auto& c : chunks)
    {
        Chunk* const chunk = c.second.get();

        if (chunk->getUpdate())
        {
            updateChunkAndNeightbors(chunk);
            chunk->setUpdate(false);
        }
    }

    const bool work = !chunksUpdate.empty();

    while (!chunksUpdate.empty())
    {
        Chunk* const chunk = chunksUpdate.front();
        chunksUpdate.pop();

        updateService->push({chunk->moveVoxelsUpdate(), chunk});
    }

    if (work)
        updateService->doWork();
}

void ChunkManager::processBufferingChunks() noexcept
{
    std::queue<ChunkUpdateService::ChunkBuffer> chunksBuffer{updateService->moveChunksBuffer()};
    while (!chunksBuffer.empty())
    {
        const auto& chunkBuffer = chunksBuffer.front();
        chunkBuffer.chunk->getModel().buffer(chunkBuffer.meshVertices);
        chunksBuffer.pop();
    }
}

void ChunkManager::update() noexcept
{
    processBindingChunks();
    processUpdatingChunks();
    processBufferingChunks();
}

void ChunkManager::render(const RenderData& renderData) const noexcept
{
    for (const auto& p : chunks)
    {
        const ChunkModel& model = p.second->getModel();

        if (model.isBuilt())
            model.render(renderData);
    }
}

void ChunkManager::updateChunk(Chunk* chunk) noexcept
{
    chunksUpdate.push(chunk);
}

void ChunkManager::updateChunkAndNeightbors(Chunk* chunk) noexcept
{
    updateChunk(chunk);

    const Chunk::Neightbors& neightbors = chunk->getNeightbors();

    if (neightbors.left)
        updateChunk(neightbors.left);

    if (neightbors.right)
        updateChunk(neightbors.right);

    if (neightbors.back)
    {
        updateChunk(neightbors.back);

        const Chunk::Neightbors& neightborsBack = neightbors.back->getNeightbors();

        if (neightborsBack.left)
            updateChunk(neightborsBack.left);

        if (neightborsBack.right)
            updateChunk(neightborsBack.right);
    }

    if (neightbors.front)
    {
        updateChunk(neightbors.front);

        const Chunk::Neightbors& neightborsFront = neightbors.front->getNeightbors();

        if (neightborsFront.left)
            updateChunk(neightborsFront.left);
        
        if (neightborsFront.right)
            updateChunk(neightborsFront.right);
    }
}

void ChunkManager::setVoxel(int x, int y, int z, unsigned voxel) noexcept
{
    if (y < 0 || y >= constants::CHUNK_HEIGHT) return;

    const ChunkLocation chunkLocaton{ChunkLocation::fromWorld(x, z)};

    auto iter = chunks.find(chunkLocaton);
    if (iter != chunks.cend())
    {
        Chunk* chunk = iter->second.get();

        const int cx = x - chunkLocaton.getX() * constants::CHUNK_WIDTH;
        const int cz = z - chunkLocaton.getZ() * constants::CHUNK_WIDTH;

        chunk->updateVoxel({Chunk::getVoxelIndex(cx, y, cz), voxel});
        chunk->setUpdate(true);
    }
}

void ChunkManager::bindChunk(Chunk* chunk) noexcept
{
    chunks.emplace(chunk->getLocation(), chunk);

    const ChunkLocation& chunkLocation = chunk->getLocation();

    auto chunkLeft  = chunks.find({chunkLocation.getX() - 1, chunkLocation.getZ()    });
    auto chunkRight = chunks.find({chunkLocation.getX() + 1, chunkLocation.getZ()    });
    auto chunkFront = chunks.find({chunkLocation.getX()    , chunkLocation.getZ() + 1});
    auto chunkBack  = chunks.find({chunkLocation.getX()    , chunkLocation.getZ() - 1});

    Chunk::Neightbors& neightbors = chunk->getNeightbors();

    if (chunkLeft != chunks.cend())
    {
        chunkLeft->second->getNeightbors().right = chunk;
        neightbors.left = chunkLeft->second.get();
    }
    if (chunkRight != chunks.cend())
    {
        chunkRight->second->getNeightbors().left = chunk;
        neightbors.right = chunkRight->second.get();
    }
    if (chunkFront != chunks.cend())
    {
        chunkFront->second->getNeightbors().back = chunk;
        neightbors.front = chunkFront->second.get();
    }
    if (chunkBack != chunks.cend())
    {
        chunkBack->second->getNeightbors().front = chunk;
        neightbors.back = chunkBack->second.get();
    }

    updateChunkAndNeightbors(chunk);
}

