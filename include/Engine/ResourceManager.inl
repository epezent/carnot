namespace carnot {

template <typename Resource>
void ResourceManager<Resource>::load(Id id, const std::string& filename)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceManager::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource>
template <typename Parameter>
void ResourceManager<Resource>::load(Id id, const std::string& filename, const Parameter& secondParam)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromMemory(filename, secondParam))
        throw std::runtime_error("ResourceManager::load - Failed to load " + filename);

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource>
void ResourceManager<Resource>::load(Id id, const void* data, std::size_t size)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromMemory(data, size))
        throw std::runtime_error("ResourceManager::load - Failed to load");

    // If loading successful, insert resource to map
    insertResource(id, std::move(resource));
}

template <typename Resource>
Resource& ResourceManager<Resource>::get(Id id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource>
const Resource& ResourceManager<Resource>::get(Id id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource>
void ResourceManager<Resource>::unload(Id id) {
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    mResourceMap.erase(found);
}

template <typename Resource>
void ResourceManager<Resource>::unloadAll() {
    mResourceMap.clear();
}

template <typename Resource>
void ResourceManager<Resource>::insertResource(Id id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    assert(!mResourceMap.count(id));
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

} // namespace carnot
