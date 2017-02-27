#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

namespace minecpp
{
    class Config;
    class ResourceContainer;

    class ResourceLoader
    {
        ResourceContainer* resourceContainer;

    public:
        explicit ResourceLoader(ResourceContainer& resourceContainer) noexcept;

        void load(const Config& config);
    };
}

#endif
