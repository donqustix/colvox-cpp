#ifndef GAUSSIANBLURPOSTPROCESSING_H
#define GAUSSIANBLURPOSTPROCESSING_H

#include <memory>

namespace minecpp
{
    class Framebuffer;
    class Texture;
    class ShaderProgram;
    class ResourceContainer;
    class RenderData;

    class GaussianBlurPostProcessing
    {
        std::unique_ptr<Framebuffer> fbos[2];
        std::unique_ptr<Texture> textures[2];
        ShaderProgram* shaderProgram;

    public:
        GaussianBlurPostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept;
        GaussianBlurPostProcessing(GaussianBlurPostProcessing&&) noexcept;
        ~GaussianBlurPostProcessing();

        GaussianBlurPostProcessing& operator=(GaussianBlurPostProcessing&&) noexcept;

        void process(const RenderData& renderData, const Texture& texture) const noexcept;

        const auto& getTexture() const noexcept {return *textures[0];}
    };
}

#endif
