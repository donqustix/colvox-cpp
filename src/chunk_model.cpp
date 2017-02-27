#include "chunk_model.h"
#include "chunk_location.h"
#include "mesh_shader_data.h"
#include "mesh.h"
#include "mesh_chunk_vertices.h"
#include "resource_container.h"
#include "shader_program.h"
#include "render_data.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using minecpp::ChunkModel;

ChunkModel::ChunkModel(const ChunkLocation& location, ResourceContainer& resourceContainer) noexcept :
    modelMatrix
    {
        glm::translate(glm::scale(glm::mat4{}, glm::vec3{constants::BLOCK_SIZE}),
        {
            location.getX() * constants::CHUNK_WIDTH,
            0.0F,
            location.getZ() * constants::CHUNK_WIDTH
        })
    }
{
    modelPartDefault.mesh = std::make_unique<Mesh<VertexP3FRGBA8>>();
    modelPartDefault.shaderProgram = resourceContainer.get<ShaderProgram>("chunk_shader");
}

ChunkModel::ChunkModel(ChunkModel&&) noexcept = default;

ChunkModel::~ChunkModel() = default;

ChunkModel& ChunkModel::operator=(ChunkModel&&) noexcept = default;

void ChunkModel::render(const RenderData& renderData) const noexcept
{
    renderPart(modelPartDefault, renderData);
}

void ChunkModel::buffer(const MeshChunkVertices& meshChunkVertices) noexcept
{
    modelPartDefault.mesh->buffer(meshChunkVertices.v_default);
    built = true;
}

void ChunkModel::renderPart(const ChunkModelPart& modelPart, const RenderData& renderData) const noexcept
{
    ::glUseProgram(modelPart.shaderProgram->getHandle());

    const glm::mat4 matrix = renderData.getProjectionMatrix() * renderData.getViewMatrix() * modelMatrix;
    modelPart.shaderProgram->uniform(0, glm::value_ptr(matrix));

    ::glBindVertexArray(modelPart.mesh->getShaderData().getVao());
    ::glDrawArrays(GL_TRIANGLES, 0, modelPart.mesh->getVertexCount());
    ::glBindVertexArray(0);

    ::glUseProgram(0);
}

