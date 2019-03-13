#pragma warning (disable : 4100)

#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Coroutine.hpp>
#include <algorithm>
#include <cmath>

namespace sfvg {

//==============================================================================
// Constructor/Destructor
//==============================================================================

namespace {
std::size_t g_nameIndex = 0;
std::size_t g_objectCount = 0;
}

Object::Object(const Name& name) :
    m_id(ID::makeId(name)),
    m_enabled(true)
{
    g_objectCount++;
}

Object::Object() :
    Object("Obj" + std::to_string(g_nameIndex++))
{ }

Object::~Object() {
    stopAllCoroutines();
    g_objectCount--;
    ID::freeId(m_id);
}

//==============================================================================
// General
//==============================================================================

void Object::setName(const Name& name) {
    ID::freeId(m_id);
    m_id = ID::makeId(name);
    onRenamed(name);
}

const Name& Object::getName() const {
    return ID::getName(m_id);
}

Id Object::getId() const {
    return m_id;
}

void Object::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Object::isEnabled() const {
    return m_enabled;
}

//==============================================================================
// Public Static Functions
//==============================================================================

std::size_t Object::getObjectCount() {
    return g_objectCount;
}

//==============================================================================
// Virtual Functions
//==============================================================================

void Object::start() {
    // do nothing by default
}

void Object::update() {
    // do nothing by default
}

void Object::lateUpdate() {
    // do nothing by default
}

void Object::onRenamed(const Name& newName) {
    // do nothing by default
}

void Object::onPhysics() {

}

void Object::onRender(RenderQue& que) {
    // do nothing by default
}

void Object::onGizmo() {
    // do nothing by default
}

//=============================================================================
// Coroutines
//=============================================================================

Handle<Coroutine> Object::startCoroutine(Enumerator&& e) {
    auto h = e.getCoroutine();
    m_coroutines.push_back(std::move(e));
    return h;
}

void Object::stopCoroutine(Handle<Coroutine> routine) {
    if (routine)
        routine->stop();
}

void Object::stopAllCoroutines() {
    m_coroutines.clear();
}

bool Object::hasCoroutines() const {
    return !m_coroutines.empty();
}

std::size_t Object::getCoroutineCount() const {
    return m_coroutines.size();
}

void Object::resumeCoroutines() {
    std::vector<Enumerator> coroutinesCopy;
    coroutinesCopy.swap(m_coroutines);
    for (auto& coro : coroutinesCopy) {
        if (coro.moveNext())
            m_coroutines.push_back(std::move(coro));
    }
}

} // namespace sfvg
