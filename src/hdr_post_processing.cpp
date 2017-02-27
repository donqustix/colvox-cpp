#include "hdr_post_processing.h"
#include "framebuffer.h"
#include "texture.h"
#include "resource_container.h"
#include "shader_program.h"
#include "mesh_shader_data.h"
#include "render_data.h"
#include "textured_rect_mesh.h"

#include <SDL2/SDL_keyboard.h>

using minecpp::HdrPostProcessing;

HdrPostProcessing::HdrPostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept :
    fbo{std::make_unique<Framebuffer>()},
    texture{std::make_unique<Texture>(GL_RGB16F, width, height, GL_RGB, GL_FLOAT, nullptr)}
{
    fbo->bindTexture(GL_COLOR_ATTACHMENT0, texture->getHandle());

    shaderProgram = resourceContainer.get<ShaderProgram>("hdr_shader");
}

HdrPostProcessing::HdrPostProcessing(HdrPostProcessing&&) noexcept = default;

HdrPostProcessing::~HdrPostProcessing() = default;

HdrPostProcessing& HdrPostProcessing::operator=(HdrPostProcessing&&) noexcept = default;

void HdrPostProcessing::process(const RenderData& renderData,
        const Texture& colorTexture, const Texture& blurTexture) const noexcept
{
    static float power = 1.0F;

    const Uint8* const keyboardState = ::SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_1])
    {
        power += 0.05F;
    }
    else if (keyboardState[SDL_SCANCODE_2])
    {
        power -= 0.05F;
    }
    if (power < 1.0F)
        power = 1.0F;

    ::glBindFramebuffer(GL_FRAMEBUFFER, fbo->getHandle());
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glUseProgram(shaderProgram->getHandle());

    ::glUniform1f(0, power);

    ::glActiveTexture(GL_TEXTURE0);
    ::glBindTexture(GL_TEXTURE_2D, colorTexture.getHandle());

    ::glActiveTexture(GL_TEXTURE1);
    ::glBindTexture(GL_TEXTURE_2D, blurTexture.getHandle());

    ::glBindVertexArray(renderData.getTexturedRectMesh().getShaderData().getVao());
    ::glDrawArrays(GL_TRIANGLES, 0, renderData.getTexturedRectMesh().getVertexCount());
    ::glBindVertexArray(0);

    ::glBindTexture(GL_TEXTURE_2D, 0);

    ::glUseProgram(0);

    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

