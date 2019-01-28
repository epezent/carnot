namespace sfvg {

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceManager::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromMemory(filename, secondParam))
        throw std::runtime_error("ResourceManager::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const void* data, std::size_t size)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromMemory(data, size))
        throw std::runtime_error("ResourceManager::load - Failed to load");

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

} // namespace sfvg
