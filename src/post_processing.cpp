#include "post_processing.h"
#include "hdr_post_processing.h"
#include "extract_brights_post_processing.h"
#include "gaussian_blur_post_processing.h"

using minecpp::PostProcessing;
using minecpp::Texture;

PostProcessing::PostProcessing(ResourceContainer& resourceContainer, int width, int height) noexcept :
    hdrPostProcessing{std::make_unique<HdrPostProcessing>(resourceContainer, width, height)},
    extractBrightsPostProcessing{std::make_unique<ExtractBrightsPostProcessing>(resourceContainer, width, height)},
    gaussianBlurPostProcessing{std::make_unique<GaussianBlurPostProcessing>(resourceContainer, width, height)}
{
}

PostProcessing::PostProcessing(PostProcessing&&) noexcept = default;

PostProcessing::~PostProcessing() = default;

PostProcessing& PostProcessing::operator=(PostProcessing&&) noexcept = default;

void PostProcessing::process(const RenderData& renderData, const Texture& texture) const noexcept
{
    extractBrightsPostProcessing->process(renderData, texture);
    gaussianBlurPostProcessing->process(renderData, extractBrightsPostProcessing->getTexture());
    hdrPostProcessing->process(renderData, texture, gaussianBlurPostProcessing->getTexture());
}

const Texture& PostProcessing::getTexture() const noexcept
{
    return hdrPostProcessing->getTexture();
}

