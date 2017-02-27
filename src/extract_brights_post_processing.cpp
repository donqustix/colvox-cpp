#include "extract_brights_post_processing.h"
#include "framebuffer.h"
#include "texture.h"
#include "resource_container.h"
#include "render_data.h"
#include "shader_program.h"
#include "textured_rect_mesh.h"
#include "mesh_shader_data.h"

using minecpp::ExtractBrightsPostProcessing;

ExtractBrightsPostProcessing::ExtractBrightsPostProcessing(ResourceContainer& resourceContainer,
        int width, int height) noexcept :
    fbo{std::make_unique<Framebuffer>()},
    texture{std::make_unique<Texture>(GL_RGB16F, width, height, GL_RGB, GL_FLOAT, nullptr)}
{
    fbo->bindTexture(GL_COLOR_ATTACHMENT0, texture->getHandle());

    shaderProgram = resourceContainer.get<ShaderProgram>("extract_brights_shader");
}

ExtractBrightsPostProcessing::ExtractBrightsPostProcessing(ExtractBrightsPostProcessing&&) noexcept = default;

ExtractBrightsPostProcessing::~ExtractBrightsPostProcessing() = default;

ExtractBrightsPostProcessing& ExtractBrightsPostProcessing::operator=(ExtractBrightsPostProcessing&&) noexcept
    = default;

void ExtractBrightsPostProcessing::process(const RenderData& renderData, const Texture& texture) const noexcept
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, fbo->getHandle());
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glUseProgram(shaderProgram->getHandle());

    ::glActiveTexture(GL_TEXTURE0);
    ::glBindTexture(GL_TEXTURE_2D, texture.getHandle());

    ::glBindVertexArray(renderData.getTexturedRectMesh().getShaderData().getVao());
    ::glDrawArrays(GL_TRIANGLES, 0, renderData.getTexturedRectMesh().getVertexCount());
    ::glBindVertexArray(0);

    ::glBindTexture(GL_TEXTURE_2D, 0);

    ::glUseProgram(0);
    
    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

