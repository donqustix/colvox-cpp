#include "render_data.h"
#include "textured_rect_mesh.h"

#include <glm/gtc/matrix_transform.hpp>

using minecpp::RenderData;

RenderData::RenderData(float fovy, float aspect, float zNear, float zFar) noexcept :
    texturedRectMesh{std::make_unique<TexturedRectMesh>()},
    projectionMatrix{glm::perspective(glm::radians(fovy), aspect, zNear, zFar)}
{
}

RenderData::RenderData(RenderData&&) noexcept = default;

RenderData::~RenderData() = default;

RenderData& RenderData::operator=(RenderData&&) noexcept = default;

