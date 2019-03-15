#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
# include <EE/Common/Imports.hpp>

namespace ee {

template <typename Resource, typename Identifier>
class ResourceManager : private NonCopyable {
public:
    void load(Identifier id, const std::string& filename);

    template <typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);
    void load(Identifier id, const void* data, std::size_t size);

    void unload(Identifier id);
    void unloadAll();

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

} // namespace ee

# include <EE/Engine/ResourceManager.inl>
