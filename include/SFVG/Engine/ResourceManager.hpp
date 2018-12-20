#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

template <typename Resource, typename Identifier>
class ResourceManager {
public:
    void load(Identifier id, const std::string& filename);

    template <typename Parameter>
    void load(Identifier id,
              const std::string& filename,
              const Parameter& secondParam);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include <SFVG/Engine/ResourceManager.inl>
