#ifndef EXTRACTBRIGHTSPOSTPROCESSING_H
#define EXTRACTBRIGHTSPOSTPROCESSING_H

#include <memory>

namespace minecpp
{
    class Framebuffer;
    class Texture;
    class ShaderProgram;
    class ResourceContainer;
    class RenderData;

    class ExtractBrightsPostProcessing
    {
        std::unique_ptr<Framebuffer> fbo;
        std::unique_ptr<Texture> texture;
        ShaderProgram* shaderProgram;

    public:
        ExtractBrightsPostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept;
        ExtractBrightsPostProcessing(ExtractBrightsPostProcessing&&) noexcept;
        ~ExtractBrightsPostProcessing();

        ExtractBrightsPostProcessing& operator=(ExtractBrightsPostProcessing&&) noexcept;

        void process(const RenderData& renderData, const Texture& texture) const noexcept;

        const auto& getTexture() const noexcept {return *texture;}
    };
}

#endif
