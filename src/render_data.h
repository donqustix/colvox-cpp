#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <glm/mat4x4.hpp>

#include <memory>

namespace minecpp
{
    class TexturedRectMesh;

    class RenderData
    {
        std::unique_ptr<TexturedRectMesh> texturedRectMesh;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

    public:
        RenderData(float fovy, float aspect, float zNear, float zFar) noexcept;
        RenderData(RenderData&&) noexcept;
        ~RenderData();

        RenderData& operator=(RenderData&&) noexcept;

        void setViewMatrix(const glm::mat4& viewMatrix) noexcept {this->viewMatrix = viewMatrix;}

        const auto& getTexturedRectMesh() const noexcept {return *texturedRectMesh;}
        const auto& getProjectionMatrix() const noexcept {return projectionMatrix;}
        const auto& getViewMatrix() const noexcept {return viewMatrix;}
    };
}

#endif
