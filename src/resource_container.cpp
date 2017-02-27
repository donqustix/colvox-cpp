#include "resource_container.h"
#include "resource.h"

using minecpp::ResourceContainer;

ResourceContainer::ResourceContainer() noexcept = default;

ResourceContainer::ResourceContainer(ResourceContainer&&) noexcept = default;

ResourceContainer::~ResourceContainer() = default;

ResourceContainer& ResourceContainer::operator=(ResourceContainer&&) noexcept = default;

