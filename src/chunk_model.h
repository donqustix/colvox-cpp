#ifndef CHUNKMODEL_H
#define CHUNKMODEL_H

#include "chunk_model_part.h"

#include <glm/mat4x4.hpp>

namespace minecpp
{
    struct MeshChunkVertices;

    class ResourceContainer;
    class ChunkLocation;
    class RenderData;

    class ChunkModel
    {
        ChunkModelPart modelPartDefault;
        glm::mat4 modelMatrix;
        
        bool built = false;

        void renderPart(const ChunkModelPart& modelPart, const RenderData& renderData) const noexcept;

    public:
        ChunkModel(const ChunkLocation& location, ResourceContainer& resourceContainer) noexcept;
        ChunkModel(ChunkModel&&) noexcept;
        ~ChunkModel();

        ChunkModel& operator=(ChunkModel&&) noexcept;

        void render(const RenderData& renderData) const noexcept;
        void buffer(const MeshChunkVertices& meshChunkVertices) noexcept;

        auto isBuilt() const noexcept {return built;}
    };
}

#endif
