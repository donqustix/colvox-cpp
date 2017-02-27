#include "mesh_chunk_builder.h"
#include "mesh_chunk_vertices.h"
#include "chunk.h"
#include "color.h"
#include "voxel.h"

using minecpp::MeshChunkBuilder;
using minecpp::MeshChunkVertices;
using minecpp::RGBA8;

void MeshChunkBuilder::computeLightsPlacing(unsigned componentIndex) noexcept
{
    while (!lightsPlace[componentIndex].empty())
    {
        const LightPlace& lightPlace = lightsPlace[componentIndex].front();

        const int x =  lightPlace.index       & (constants::CHUNK_WIDTH - 1);
        const int y =  lightPlace.index >> 8;
        const int z = (lightPlace.index >> 4) & (constants::CHUNK_WIDTH - 1);

        Chunk* const chunk = lightPlace.chunk;

        const unsigned lightComponent = color::getComponent(chunk->getLight(lightPlace.index), componentIndex);

        lightsPlace[componentIndex].pop();

        processLightsPlacing(chunk, x - 1, y    , z    , lightComponent, componentIndex);
        processLightsPlacing(chunk, x + 1, y    , z    , lightComponent, componentIndex);
        processLightsPlacing(chunk, x    , y - 1, z    , lightComponent, componentIndex);
        processLightsPlacing(chunk, x    , y + 1, z    , lightComponent, componentIndex);
        processLightsPlacing(chunk, x    , y    , z - 1, lightComponent, componentIndex);
        processLightsPlacing(chunk, x    , y    , z + 1, lightComponent, componentIndex);
    }
}

void MeshChunkBuilder::processLightsPlacing(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
        unsigned componentIndex) noexcept
{
    ptrdiff_t index;
    Chunk* const chunkFound = chunk->findChunk(x, y, z, index);

    if (chunkFound && !chunkFound->getVoxel(index))
    {
        const unsigned neightborLight = chunkFound->getLight(index);
        const unsigned neightborLightComponent = color::getComponent(neightborLight, componentIndex);

        if (neightborLightComponent + 1 < lightComponent)
        {
            lightsPlace[componentIndex].push({index, chunkFound});
            chunkFound->setLight(index, color::setComponent(neightborLight, componentIndex, lightComponent - 1));
        }
    }
}

void MeshChunkBuilder::computeLightsRemoving(unsigned componentIndex) noexcept
{
    while (!lightsRemove[componentIndex].empty())
    {
        const LightRemove& lightRemove = lightsRemove[componentIndex].front();

        const int x =  lightRemove.index       & (constants::CHUNK_WIDTH - 1);
        const int y =  lightRemove.index >> 8;
        const int z = (lightRemove.index >> 4) & (constants::CHUNK_WIDTH - 1);

        Chunk* const chunk = lightRemove.chunk;

        const unsigned lightComponent = color::getComponent(lightRemove.light, componentIndex);

        lightsRemove[componentIndex].pop();

        processLightsRemoving(chunk, x - 1, y    , z    , lightComponent, componentIndex);
        processLightsRemoving(chunk, x + 1, y    , z    , lightComponent, componentIndex);
        processLightsRemoving(chunk, x    , y - 1, z    , lightComponent, componentIndex);
        processLightsRemoving(chunk, x    , y + 1, z    , lightComponent, componentIndex);
        processLightsRemoving(chunk, x    , y    , z - 1, lightComponent, componentIndex);
        processLightsRemoving(chunk, x    , y    , z + 1, lightComponent, componentIndex);
    }
}

void MeshChunkBuilder::processLightsRemoving(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
        unsigned componentIndex) noexcept
{
    ptrdiff_t index;
    Chunk* const chunkFound = chunk->findChunk(x, y, z, index);

    if (chunkFound)
    {
        const unsigned neightborLight = chunkFound->getLight(index);
        const unsigned neightborLightComponent = color::getComponent(neightborLight, componentIndex);

        if (neightborLightComponent && neightborLightComponent < lightComponent)
        {
            lightsRemove[componentIndex].push({index, chunkFound, neightborLightComponent});
            chunkFound->setLight(index, color::setComponent(neightborLight, componentIndex, 0));
        }
        else if (neightborLightComponent >= lightComponent)
        {
            lightsPlace[componentIndex].push({index, chunkFound});
        }
    }
}

void MeshChunkBuilder::computeLights() noexcept
{
    for (unsigned i = 0; i < 3; ++i)
    {
        computeLightsRemoving(i);
        computeLightsPlacing(i);
    }

    computeSunLightsRemoving();
    computeSunLightsPlacing();
}

void MeshChunkBuilder::computeSunLightsPlacing() noexcept
{
    while (!lightsPlace[3].empty())
    {
        const LightPlace& lightPlace = lightsPlace[3].front();

        const int x =  lightPlace.index       & (constants::CHUNK_WIDTH - 1);
        const int y =  lightPlace.index >> 8;
        const int z = (lightPlace.index >> 4) & (constants::CHUNK_WIDTH - 1);

        Chunk* const chunk = lightPlace.chunk;

        const unsigned lightComponent = chunk->getLight(lightPlace.index) >> 12 & 15;

        lightsPlace[3].pop();

        processSunLightsPlacing(chunk, x - 1, y    , z    , lightComponent, 0);
        processSunLightsPlacing(chunk, x + 1, y    , z    , lightComponent, 0);
        processSunLightsPlacing(chunk, x    , y - 1, z    , lightComponent, 1);
        processSunLightsPlacing(chunk, x    , y + 1, z    , lightComponent, 0);
        processSunLightsPlacing(chunk, x    , y    , z - 1, lightComponent, 0);
        processSunLightsPlacing(chunk, x    , y    , z + 1, lightComponent, 0);
    }
}

void MeshChunkBuilder::processSunLightsPlacing(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
        bool vertDown) noexcept
{
    ptrdiff_t index;
    Chunk* const chunkFound = chunk->findChunk(x, y, z, index);

    if (chunkFound && !chunkFound->getVoxel(index))
    {
        const unsigned neightborLight = chunkFound->getLight(index);
        const unsigned neightborLightComponent = neightborLight >> 12 & 15;

        if (vertDown && lightComponent == constants::MAX_LIGHT_LEVEL)
        {
            lightsPlace[3].push({index, chunkFound});
            chunkFound->setLight(index, (neightborLight & 0x0FFF) | 0xF000);
        }
        else if (neightborLightComponent + 1 < lightComponent)
        {
            lightsPlace[3].push({index, chunkFound});
            chunkFound->setLight(index, (neightborLight & 0x0FFF) | (lightComponent - 1) << 12);
        }
    }
}

void MeshChunkBuilder::computeSunLightsRemoving() noexcept
{
    while (!lightsRemove[3].empty())
    {
        const LightRemove& lightRemove = lightsRemove[3].front();

        const int x =  lightRemove.index       & (constants::CHUNK_WIDTH - 1);
        const int y =  lightRemove.index >> 8;
        const int z = (lightRemove.index >> 4) & (constants::CHUNK_WIDTH - 1);

        Chunk* const chunk = lightRemove.chunk;

        const unsigned lightComponent = lightRemove.light >> 12 & 15;

        lightsRemove[3].pop();

        processSunLightsRemoving(chunk, x - 1, y    , z    , lightComponent, 0);
        processSunLightsRemoving(chunk, x + 1, y    , z    , lightComponent, 0);
        processSunLightsRemoving(chunk, x    , y - 1, z    , lightComponent, 1);
        processSunLightsRemoving(chunk, x    , y + 1, z    , lightComponent, 0);
        processSunLightsRemoving(chunk, x    , y    , z - 1, lightComponent, 0);
        processSunLightsRemoving(chunk, x    , y    , z + 1, lightComponent, 0);
    }
}

void MeshChunkBuilder::processSunLightsRemoving(Chunk* chunk, int x, int y, int z, unsigned lightComponent,
        bool vertDown) noexcept
{
    ptrdiff_t index;
    Chunk* const chunkFound = chunk->findChunk(x, y, z, index);

    if (chunkFound)
    {
        const unsigned neightborLight = chunkFound->getLight(index);
        const unsigned neightborLightComponent = neightborLight >> 12 & 15;

        if (neightborLightComponent)
        {
            if (neightborLightComponent < lightComponent || (vertDown && lightComponent == constants::MAX_LIGHT_LEVEL))
            {
                lightsRemove[3].push({index, chunkFound, neightborLightComponent});
                chunkFound->setLight(index, neightborLight & 0xFFF);
            }
            else if (neightborLightComponent >= lightComponent)
            {
                lightsPlace[3].push({index, chunkFound});
            }
        }
    }
}

void MeshChunkBuilder::prepareSunLights(Chunk* chunk) noexcept
{
    for (int i = 0; i < constants::CHUNK_WIDTH; ++i)
    {
        for (int j = 0; j < constants::CHUNK_WIDTH; ++j)
        {
            for (int k = constants::CHUNK_HEIGHT - 1; k >= 0; --k)
            {
                const ptrdiff_t index = Chunk::getVoxelIndex(j, k, i);

                if (chunk->getVoxel(index))
                    break;

                chunk->setLight(index, (chunk->getLight(index) & 0xFFF) | 0xF000);
            }
        }
    }
    for (int i = 0; i < constants::CHUNK_WIDTH; ++i)
    {
        for (int j = 0; j < constants::CHUNK_WIDTH; ++j)
        {
            for (int k = constants::CHUNK_HEIGHT - 1; k >= 0; --k)
            {
                if (chunk->getVoxel(Chunk::getVoxelIndex(j, k, i)))
                    break;

                processLightsPlacing(chunk, k - 1, i    , j    , constants::MAX_LIGHT_LEVEL, 3);
                processLightsPlacing(chunk, k + 1, i    , j    , constants::MAX_LIGHT_LEVEL, 3);
                processLightsPlacing(chunk, k    , i    , j - 1, constants::MAX_LIGHT_LEVEL, 3);
                processLightsPlacing(chunk, k    , i    , j + 1, constants::MAX_LIGHT_LEVEL, 3);
            }
        }
    }
}

MeshChunkVertices MeshChunkBuilder::build(const Chunk& chunk) const noexcept
{
    std::vector<VertexP3FRGBA8> verticesDefault;

    for (int i = 0; i < constants::CHUNK_HEIGHT; ++i)
    {
        for (int j = 0; j < constants::CHUNK_WIDTH; ++j)
        {
            for (int k = 0; k < constants::CHUNK_WIDTH; ++k)
            {
                const unsigned voxel = chunk.getVoxel(Chunk::getVoxelIndex(k, i, j));

                if (!voxel) continue;

                const unsigned voxelColor = voxel::getColor(voxel);

                const unsigned char r = voxelColor       & 15;
                const unsigned char g = voxelColor >> 4  & 15;
                const unsigned char b = voxelColor >> 8  & 15;
                const unsigned char a = voxelColor >> 12 & 15;

                if (!chunk.findVoxel(k - 1, i, j))
                {
                    quadYZ({&verticesDefault, &chunk, k, i, j, r, g, b, a, 0});
                }
                if (!chunk.findVoxel(k + 1, i, j))
                {
                    quadYZ({&verticesDefault, &chunk, k, i, j, r, g, b, a, 1});
                }
                if (!chunk.findVoxel(k, i, j - 1))
                {
                    quadXY({&verticesDefault, &chunk, k, i, j, r, g, b, a, 0});
                }
                if (!chunk.findVoxel(k, i, j + 1))
                {
                    quadXY({&verticesDefault, &chunk, k, i, j, r, g, b, a, 1});
                }
                if (!chunk.findVoxel(k, i - 1, j))
                {
                    quadXZ({&verticesDefault, &chunk, k, i, j, r, g, b, a, 0});
                }
                if (!chunk.findVoxel(k, i + 1, j))
                {
                    quadXZ({&verticesDefault, &chunk, k, i, j, r, g, b, a, 1});
                }
            }
        }
    }

    return {std::move(verticesDefault)};
}

void MeshChunkBuilder::placeLight(Chunk* chunk, ptrdiff_t index, unsigned light) noexcept
{
    for (unsigned i = 0; i < 3; ++i)
        lightsPlace[i].push({index, chunk});

    chunk->setLight(index, light);
}

void MeshChunkBuilder::removeLight(Chunk* chunk, ptrdiff_t index) noexcept
{
    const unsigned light = chunk->getLight(index);

    for (unsigned i = 0; i < 3; ++i)
        lightsRemove[i].push({index, chunk, color::getComponent(light, i)});

    chunk->setLight(index, light & 0xF000);
}

void MeshChunkBuilder::removeSunLight(Chunk* chunk, ptrdiff_t index) noexcept
{
    const unsigned light = chunk->getLight(index);
    lightsRemove[3].push({index, chunk, light & 0xF000});
    chunk->setLight(index, light & 0xFFF);
}

void MeshChunkBuilder::quadXY(const BuildingInfo& buildingInfo) const noexcept
{
    const float x = buildingInfo.x;
    const float y = buildingInfo.y;
    const float z = buildingInfo.z + buildingInfo.side;

    const int dz = (buildingInfo.side << 1) - 1;

    const RGB8 vertexColor{buildingInfo.r, buildingInfo.g, buildingInfo.b};

    const RGBA8 color1 = computeVertexXYColor(buildingInfo, -1, -1, dz);
    const RGBA8 color2 = computeVertexXYColor(buildingInfo,  1,  1, dz);
    const RGBA8 color3 = computeVertexXYColor(buildingInfo,  1, -1, dz);
    const RGBA8 color4 = computeVertexXYColor(buildingInfo, -1,  1, dz);

    buildingInfo.vertices->push_back({{x       , y       , z}, vertexColor, color1});
    buildingInfo.vertices->push_back({{x + 1.0F, y       , z}, vertexColor, color3});
    buildingInfo.vertices->push_back({{x + 1.0F, y + 1.0F, z}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x + 1.0F, y + 1.0F, z}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x       , y + 1.0F, z}, vertexColor, color4});
    buildingInfo.vertices->push_back({{x       , y       , z}, vertexColor, color1});
}

void MeshChunkBuilder::quadYZ(const BuildingInfo& buildingInfo) const noexcept
{
    const float x = buildingInfo.x + buildingInfo.side;
    const float y = buildingInfo.y;
    const float z = buildingInfo.z;

    const int dx = (buildingInfo.side << 1) - 1;

    const RGB8 vertexColor{buildingInfo.r, buildingInfo.g, buildingInfo.b};

    const RGBA8 color1 = computeVertexYZColor(buildingInfo, dx, -1, -1);
    const RGBA8 color2 = computeVertexYZColor(buildingInfo, dx,  1,  1);
    const RGBA8 color3 = computeVertexYZColor(buildingInfo, dx, -1,  1);
    const RGBA8 color4 = computeVertexYZColor(buildingInfo, dx,  1, -1);

    buildingInfo.vertices->push_back({{x, y       , z       }, vertexColor, color1});
    buildingInfo.vertices->push_back({{x, y       , z + 1.0F}, vertexColor, color3});
    buildingInfo.vertices->push_back({{x, y + 1.0F, z + 1.0F}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x, y + 1.0F, z + 1.0F}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x, y + 1.0F, z       }, vertexColor, color4});
    buildingInfo.vertices->push_back({{x, y       , z       }, vertexColor, color1});
}

void MeshChunkBuilder::quadXZ(const BuildingInfo& buildingInfo) const noexcept
{
    const float x = buildingInfo.x;
    const float y = buildingInfo.y + buildingInfo.side;
    const float z = buildingInfo.z;

    const int dy = (buildingInfo.side << 1) - 1;

    const RGB8 vertexColor{buildingInfo.r, buildingInfo.g, buildingInfo.b};

    const RGBA8 color1 = computeVertexXZColor(buildingInfo, -1, dy, -1);
    const RGBA8 color2 = computeVertexXZColor(buildingInfo,  1, dy,  1);
    const RGBA8 color3 = computeVertexXZColor(buildingInfo,  1, dy, -1);
    const RGBA8 color4 = computeVertexXZColor(buildingInfo, -1, dy,  1);

    buildingInfo.vertices->push_back({{x       , y, z       }, vertexColor, color1});
    buildingInfo.vertices->push_back({{x + 1.0F, y, z       }, vertexColor, color3});
    buildingInfo.vertices->push_back({{x + 1.0F, y, z + 1.0F}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x + 1.0F, y, z + 1.0F}, vertexColor, color2});
    buildingInfo.vertices->push_back({{x       , y, z + 1.0F}, vertexColor, color4});
    buildingInfo.vertices->push_back({{x       , y, z       }, vertexColor, color1});
}

RGBA8 MeshChunkBuilder::computeVertexXYColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept
{
    const unsigned light1 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y     , buildingInfo.z + dz);
    const unsigned light2 = buildingInfo.chunk->findLight(buildingInfo.x     , buildingInfo.y     , buildingInfo.z + dz);
    const unsigned light3 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y + dy, buildingInfo.z + dz);
    const unsigned light4 = buildingInfo.chunk->findLight(buildingInfo.x     , buildingInfo.y + dy, buildingInfo.z + dz);

    return combineColors(light1, light2, light3, light4);
}

RGBA8 MeshChunkBuilder::computeVertexYZColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept
{
    const unsigned light1 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y     , buildingInfo.z + dz);
    const unsigned light2 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y     , buildingInfo.z     );
    const unsigned light3 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y + dy, buildingInfo.z + dz);
    const unsigned light4 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y + dy, buildingInfo.z     );

    return combineColors(light1, light2, light3, light4);
}

RGBA8 MeshChunkBuilder::computeVertexXZColor(const BuildingInfo& buildingInfo, int dx, int dy, int dz) const noexcept
{
    const unsigned light1 = buildingInfo.chunk->findLight(buildingInfo.x     , buildingInfo.y + dy, buildingInfo.z + dz);
    const unsigned light2 = buildingInfo.chunk->findLight(buildingInfo.x     , buildingInfo.y + dy, buildingInfo.z     );
    const unsigned light3 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y + dy, buildingInfo.z + dz);
    const unsigned light4 = buildingInfo.chunk->findLight(buildingInfo.x + dx, buildingInfo.y + dy, buildingInfo.z     );

    return combineColors(light1, light2, light3, light4);
}

RGBA8 MeshChunkBuilder::combineColors(unsigned light1, unsigned light2, unsigned light3, unsigned light4) const noexcept
{
    const GLubyte r = ((light1 & 0x000F) + (light2 & 0x000F) + (light3 & 0x000F) + (light4 & 0x000F)) >> 2;
    const GLubyte g = ((light1 & 0x00F0) + (light2 & 0x00F0) + (light3 & 0x00F0) + (light4 & 0x00F0)) >> 6;
    const GLubyte b = ((light1 & 0x0F00) + (light2 & 0x0F00) + (light3 & 0x0F00) + (light4 & 0x0F00)) >> 10;
    const GLubyte a = ((light1 & 0xF000) + (light2 & 0xF000) + (light3 & 0xF000) + (light4 & 0xF000)) >> 14;

    return {r, g, b, a};
}

