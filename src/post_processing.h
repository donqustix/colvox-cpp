#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <memory>

namespace minecpp
{
    class ResourceContainer;
    class Texture;
    class RenderData;
    class HdrPostProcessing;
    class ExtractBrightsPostProcessing;
    class GaussianBlurPostProcessing;

    class PostProcessing
    {
        std::unique_ptr<HdrPostProcessing> hdrPostProcessing;
        std::unique_ptr<ExtractBrightsPostProcessing> extractBrightsPostProcessing;
        std::unique_ptr<GaussianBlurPostProcessing> gaussianBlurPostProcessing;

    public:
        PostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept;
        PostProcessing(PostProcessing&&) noexcept;
        ~PostProcessing();

        PostProcessing& operator=(PostProcessing&&) noexcept;

        void process(const RenderData& renderData, const Texture& texture) const noexcept;

        const Texture& getTexture() const noexcept;
    };
}

#endif
