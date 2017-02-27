#include "chunk_load_service.h"
#include "chunk.h"
#include "chunk_default_generator.h"

using minecpp::ChunkLoadService;
using minecpp::Chunk;

ChunkLoadService::ChunkLoadService(ResourceContainer& resourceContainer) noexcept :
    chunkGenerator{std::make_unique<ChunkDefaultGenerator>()},
    resourceContainer{&resourceContainer}
{
}

ChunkLoadService::~ChunkLoadService()
{
    this->stopRunning();
}

void ChunkLoadService::working() noexcept
{
    while (!chunksLoad.empty())
    {
        std::unique_lock<std::mutex> guard{mx};

        const ChunkLocation chunkLocation{chunksLoad.front()};
        chunksLoad.pop();

        guard.unlock();

        Chunk* const chunk = chunkGenerator->generate(chunkLocation, *resourceContainer);
        guard.lock();

        chunksBind.push(chunk);
    }
}

void ChunkLoadService::load(const ChunkLocation& chunkLocaton) noexcept
{
    std::lock_guard<std::mutex> guard{mx};
    chunksLoad.push(chunkLocaton);
}

std::queue<Chunk*> ChunkLoadService::moveChunksBind() noexcept
{
    std::lock_guard<std::mutex> guard{mx};
    return std::move(chunksBind);
}

