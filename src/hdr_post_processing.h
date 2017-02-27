#ifndef HDRPOSTPROCESSING_H
#define HDRPOSTPROCESSING_H

#include <memory>

namespace minecpp
{
    class ResourceContainer;
    class Texture;
    class Framebuffer;
    class ShaderProgram;
    class RenderData;

    class HdrPostProcessing
    {
        std::unique_ptr<Framebuffer> fbo;
        std::unique_ptr<Texture> texture;
        ShaderProgram* shaderProgram;

    public:
        HdrPostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept;
        HdrPostProcessing(HdrPostProcessing&&) noexcept;
        ~HdrPostProcessing();

        HdrPostProcessing& operator=(HdrPostProcessing&&) noexcept;

        void process(const RenderData& renderData,
                const Texture& sceneTexture, const Texture& blurTexture) const noexcept;

        const auto& getTexture() const noexcept {return *texture;}
    };
}

#endif
