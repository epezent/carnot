#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <Utility/Types.hpp>

namespace carnot {

/// Resource memory manager 
template <typename Resource>
class ResourceManager : private NonCopyable {
public:

    /// Loads resource from file
    void load(Id id, const std::string& filename);

    /// Load resource from file with second parameter
    template <typename Parameter>
    void load(Id id, const std::string& filename, const Parameter& secondParam);

    /// Load resource from memory
    void load(Id id, const void* data, std::size_t size);

    /// Unload a resource by Id
    void unload(Id id);

    /// Unload all resources
    void unloadAll();

    /// Get non-const reference to resource by Id
    Resource& get(Id id);

    /// Get const reference to resource by Id
    const Resource& get(Id id) const;

private:

    /// Inserts a resource into the resource map
    void insertResource(Id id, std::unique_ptr<Resource> resource);

private:
    std::map<Id, std::unique_ptr<Resource>> mResourceMap; ///< resource map
};

} // namespace carnot

#include <Engine/ResourceManager.inl>
