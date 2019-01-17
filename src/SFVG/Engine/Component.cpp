#include <SFVG/Engine/Component.hpp>
#include <SFVG/Engine/Object.hpp>

namespace sfvg {

//==========================================================================
// Constructor/Destructor
//==========================================================================
namespace {
    static std::size_t g_componentCount = 0;
} // namespace

Component::Component() :
    m_enabled(true),
    m_startCalled(false),
    m_object(nullptr),
    m_index(0),
    m_engine(nullptr),
    m_coroutines()
{
    g_componentCount++;
}

Component::~Component() {
    stopAllCoroutines();
    g_componentCount--;
}

//==========================================================================
// General Functions
//==========================================================================

void Component::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Component::isEnabled() const {
    return m_enabled;
}

bool Component::isActive() const {
    return m_enabled && object()->isActive();
}

Handle<Object> Component::object() const {
    assert(m_object != nullptr);
    return m_object->getHandle();
}

void Component::destroy() {
    object()->removeComponent(m_index);
}

void Component::setEngine(Engine* engine) {
    m_engine = engine;
}

Engine& Component::engine() const {
    return *m_engine;
}

std::size_t Component::getIndex() const {
    return m_index;
}

void Component::start() {
    // do nothing by default
}

void Component::update() {
    // do nothing by default
}

void Component::onComponentAdded(Handle<Component> component) {
    // do nothing by default
}

void Component::updateAll() {
    if (m_enabled) {
        if (!m_startCalled) {
            start();
            m_startCalled = true;
        }
        update();
        if (hasCoroutines())
            resumeCoroutines();
    }
}

std::size_t Component::getComponentCount() {
    return g_componentCount;
}

//=============================================================================
// Coroutines
//=============================================================================

Handle<Coroutine> Component::startCoroutine(Enumerator&& e) {
    auto h = e.getCoroutine();
    m_coroutines.push_back(std::move(e));
    return h;
}

void Component::stopCoroutine(Handle<Coroutine> routine) {
    if (routine)
        routine->stop();
}

void Component::stopAllCoroutines() {
    m_coroutines.clear();
}

bool Component::hasCoroutines() const {
    return !m_coroutines.empty();
}

std::size_t Component::getCoroutineCount() const {
    return m_coroutines.size();
}

void Component::resumeCoroutines() {
    std::vector<Enumerator> coroutinesCopy;
    coroutinesCopy.swap(m_coroutines);
    for (auto& coro : coroutinesCopy) {
        if (coro.moveNext())
            m_coroutines.push_back(std::move(coro));
    }
}

} // namespace sfvg
