#include <SFVG/Engine/Id.hpp>
#include <unordered_map>
#include <cassert>

static std::hash<std::string> g_hasher;
static std::unordered_map<Id, std::string> g_idMap;

namespace ID {

Id makeId(const Name& name) {
    // hash name
    Id id = g_hasher(name);
    // assert that Id isn't already in map
    assert(g_idMap.count(id) == 0);
    // add Id to map
    g_idMap[id] = name;
    // return Id
    return id;
}

Id getId(const Name& name) {
    // hash name
    Id id = g_hasher(name);
    // assert that Id is already in map
    assert(g_idMap.count(id) > 0);
    // return Id
    return id;
}

void freeId(Id id) {
    g_idMap.erase(id);
}

const Name& getName(Id id) {
    auto it = g_idMap.find(id);
    assert(it != g_idMap.end());
    return it->second;
}

}
