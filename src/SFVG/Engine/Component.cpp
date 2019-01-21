#include <SFVG/Engine/Component.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/GameObject.hpp>

namespace sfvg {

//==========================================================================
// Constructor/Destructor
//==========================================================================
namespace {
    static std::size_t g_componentCount = 0;
} // namespace

Component::Component(GameObject& _gameObject) :
    Object(_gameObject.engine),
    gameObject(_gameObject),
    m_startCalled(false),
    m_index(0)
{
    g_componentCount++;
}

Component::~Component() {
    g_componentCount--;
}

//==========================================================================
// General Functions
//==========================================================================

bool Component::isActive() const {
    return isEnabled() && gameObject.isActive();
}

void Component::destroy() {
    gameObject.removeComponent(m_index);
}


std::size_t Component::getIndex() const {
    return m_index;
}

void Component::onComponentAdded(Handle<Component> component) {
    // do nothing by default
}

void Component::updateAll() {
    if (isEnabled()) {
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

} // namespace sfvg
