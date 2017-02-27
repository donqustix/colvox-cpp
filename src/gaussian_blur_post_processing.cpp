#include "gaussian_blur_post_processing.h"
#include "framebuffer.h"
#include "texture.h"
#include "shader_program.h"
#include "resource_container.h"
#include "render_data.h"
#include "textured_rect_mesh.h"
#include "mesh_shader_data.h"

using minecpp::GaussianBlurPostProcessing;

GaussianBlurPostProcessing::GaussianBlurPostProcessing(ResourceContainer& resourceContainer,
        int width, int height) noexcept :
    fbos{std::make_unique<Framebuffer>(), std::make_unique<Framebuffer>()},
    textures{std::make_unique<Texture>(GL_RGB16F, width, height, GL_RGB, GL_FLOAT, nullptr),
             std::make_unique<Texture>(GL_RGB16F, width, height, GL_RGB, GL_FLOAT, nullptr)}
{
    fbos[0]->bindTexture(GL_COLOR_ATTACHMENT0, textures[0]->getHandle());
    fbos[1]->bindTexture(GL_COLOR_ATTACHMENT0, textures[1]->getHandle());

    shaderProgram = resourceContainer.get<ShaderProgram>("gaussian_blur_shader");
}

GaussianBlurPostProcessing::GaussianBlurPostProcessing(GaussianBlurPostProcessing&&) noexcept = default;

GaussianBlurPostProcessing::~GaussianBlurPostProcessing() = default;

GaussianBlurPostProcessing& GaussianBlurPostProcessing::operator=(GaussianBlurPostProcessing&&) noexcept
    = default;

void GaussianBlurPostProcessing::process(const RenderData& renderData, const Texture& texture) const noexcept
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]->getHandle());
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]->getHandle());
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glUseProgram(shaderProgram->getHandle());
    ::glActiveTexture(GL_TEXTURE0);
    ::glBindVertexArray(renderData.getTexturedRectMesh().getShaderData().getVao());

    bool first = true, horizontal = false;

    for (unsigned i = 0; i < 10; ++i)
    {
        shaderProgram->uniform(1, horizontal);
        ::glBindFramebuffer(GL_FRAMEBUFFER, fbos[horizontal]->getHandle());
        ::glBindTexture(GL_TEXTURE_2D, first ? texture.getHandle() : textures[!horizontal]->getHandle());
        ::glDrawArrays(GL_TRIANGLES, 0, renderData.getTexturedRectMesh().getVertexCount());
        horizontal = !horizontal;
        if (first)
            first = false;
    }
    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ::glBindVertexArray(0);
    ::glUseProgram(0);
}

