#ifndef MESHCHUNKBUILDER_H
#define MESHCHUNKBUILDER_H

#include <cstddef>
#include <vector>
#include <queue>

namespace minecpp
{
    struct MeshChunkVertices;
    struct VertexP3FRGBA8;
    struct RGBA8;

    class Chunk;

    class MeshChunkBuilder
    {
        struct LightPlace
        {
            ptrdiff_t index;
            Chunk* chunk;
        };

        struct LightRemove
        {
            ptrdiff_t index;
            Chunk* chunk;
            unsigned light;
        };

        struct BuildingInfo
        {
            std::vector<VertexP3FRGBA8>* vertices;
            const Chunk* chunk;
            int x;
            int y;
            int z;
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
            bool side;
        };

        std::queue<LightPlace> lightsPlace[4];
        std::queue<LightRemove> lightsRemove[4];

        void quadXY(const BuildingInfo& buildingInfo) const noexcept;
        void quadYZ(const BuildingInfo& buildingInfo) const noexcept;
        void quadXZ(const BuildingInfo& buildingInfo) const noexcept;

        void computeLightsPlacing(unsigned componentIndex) noexcept;
        void processLightsPlacing(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
                unsigned componentIndex) noexcept;

        void computeLightsRemoving(unsigned componentIndex) noexcept;
        void processLightsRemoving(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
                unsigned componentIndex) noexcept;

        void computeSunLightsPlacing() noexcept;
        void processSunLightsPlacing(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
                bool vertDown) noexcept;

        void computeSunLightsRemoving() noexcept;
        void processSunLightsRemoving(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
                bool vertDown) noexcept;

        RGBA8 computeVertexXYColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept;
        RGBA8 computeVertexYZColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept;
        RGBA8 computeVertexXZColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept;

        RGBA8 combineColors(unsigned light1, unsigned light2, unsigned light3, unsigned light4) const noexcept;

    public:
        void computeLights() noexcept;

        void prepareSunLights(Chunk* chunk) noexcept;

        void placeLight(Chunk* chunk, ptrdiff_t index, unsigned light) noexcept;
        void removeLight(Chunk* chunk, ptrdiff_t index) noexcept;

        void removeSunLight(Chunk* chunk, ptrdiff_t index) noexcept;

        MeshChunkVertices build(const Chunk& chunk) const noexcept;
    };
}

#endif
