#include <SFVG/Engine/Component.hpp>
#include <SFVG/Engine/Object.hpp>

namespace sfvg {

//==========================================================================
// Constructor/Destructor
//==========================================================================

Component::Component() :
    m_enabled(true),
    m_object(nullptr),
    m_index(0),
    m_engine(nullptr),
    m_coroutines()
{

}

Component::~Component() {
    // stopAllCoroutines();
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

void Component::setEngine(Engine* engine) {
    m_engine = engine;
}

Engine& Component::getEngine() const {
    return *m_engine;
}

std::size_t Component::getIndex() const {
    return m_index;
}

void Component::update() {
    // do nothing by default
}

void Component::updateAll() {
    if (m_enabled) {
        update();
        // if (hasCoroutines())
        //     resumeCoroutines();
    }
}


} // namespace sfvg

//

void Object::updateComponentIndices() {
    for (std::size_t i = 0; i < m_components.size(); ++i)
        m_components[i]->m_index = i;
}

void Object::updateComponents() {
    m_iteratingComponents = true;
    for (const auto& comp : m_components)
        comp->updateAll();
    m_iteratingComponents = false;
}
