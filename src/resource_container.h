#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include <cassert>

namespace minecpp
{
    struct Resource;

    class ResourceContainer
    {
        std::unordered_map<std::string, std::unique_ptr<Resource>> resources;

    public:
        ResourceContainer() noexcept;
        ResourceContainer(ResourceContainer&&) noexcept;
        ~ResourceContainer();

        ResourceContainer& operator=(ResourceContainer&&) noexcept;

        template<typename T, typename... Args>
        T* load(const std::string& name, Args&&... args)
        {
            auto pair = resources.emplace(name, std::make_unique<T>(std::forward<Args>(args)...));
            assert(pair.second);
            return dynamic_cast<T*>(pair.first->second.get());
        }

        template<typename T>
        T* get(const std::string& name) noexcept
        {
            auto iter = resources.find(name);
            return iter == resources.cend() ? nullptr : dynamic_cast<T*>(iter->second.get());
        }
    };
}

#endif
