#ifndef CHUNK_H
#define CHUNK_H

#include "chunk_location.h"
#include "constants.h"
#include "voxel_update.h"

#include <utility>
#include <memory>
#include <vector>

namespace minecpp
{
    class ResourceContainer;
    class ChunkModel;

    class Chunk
    {
    public:
        struct Neightbors
        {
            Chunk* left;
            Chunk* right;
            Chunk* front;
            Chunk* back;
        };

    private:
        unsigned voxels[constants::CHUNK_WIDTH * constants::CHUNK_WIDTH * constants::CHUNK_HEIGHT]{};
        unsigned lights[constants::CHUNK_WIDTH * constants::CHUNK_WIDTH * constants::CHUNK_HEIGHT]{};

        Neightbors neightbors{};

        std::vector<VoxelUpdate> voxelsUpdate;

        std::unique_ptr<ChunkModel> model;

        ChunkLocation location;

        bool update = false;

    public:
        static constexpr ptrdiff_t getVoxelIndex(int x, int y, int z) noexcept
        {
            return constants::CHUNK_WIDTH * constants::CHUNK_WIDTH * y + constants::CHUNK_WIDTH * z + x;
        }

        Chunk(const ChunkLocation& location, ResourceContainer& resourceContainer) noexcept;
        Chunk(Chunk&& chunk) noexcept;
        ~Chunk();

        Chunk& operator=(Chunk&&) noexcept;

        const Chunk* findChunk(int x, int y, int z, ptrdiff_t& index) const noexcept;
              Chunk* findChunk(int x, int y, int z, ptrdiff_t& index)       noexcept;

        std::vector<VoxelUpdate> moveVoxelsUpdate() noexcept {return std::move(voxelsUpdate);}

        void updateVoxel(const VoxelUpdate& voxelUpdate) noexcept {voxelsUpdate.push_back(voxelUpdate);}

        void setUpdate(bool update) noexcept {this->update = update;}
        auto getUpdate() const noexcept {return update;}

        void setVoxel(ptrdiff_t index, unsigned voxel) noexcept {*(voxels + index) = voxel;}
        auto getVoxel(ptrdiff_t index) const noexcept {return *(voxels + index);}

        void setLight(ptrdiff_t index, unsigned light) noexcept {*(lights + index) = light;}
        auto getLight(ptrdiff_t index) const noexcept {return *(lights + index);}

        unsigned findVoxel(int x, int y, int z) const noexcept;
        unsigned findLight(int x, int y, int z) const noexcept;

        const auto& getModel() const noexcept {return *model;}
              auto& getModel()       noexcept {return *model;}

        const auto& getNeightbors() const noexcept {return neightbors;}
              auto& getNeightbors()       noexcept {return neightbors;}

        const auto& getLocation() const noexcept {return location;}
    };
}

#endif
