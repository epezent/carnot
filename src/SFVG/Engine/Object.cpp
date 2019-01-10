#pragma warning (disable : 4100)

#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Coroutine.hpp>
#include <SFVG/Graphics.hpp>
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
    m_enabled(true),
    m_layer(0),
    m_iteratingChildren(false),
    m_startCalled(false),
    m_iteratingComponents(false),
    m_parent(nullptr),
    m_index(0),
    m_engine(nullptr),
    m_isRoot(false),
    transform(*attachComponent(std::make_shared<Transform>()).as<Transform>().get())
{
    g_objectCount++;
}

Object::Object() :
    Object("obj" + std::to_string(g_nameIndex++))
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

Handle<Object> Object::getHandle() const {
    if (isRoot())
        return m_engine->getRoot();
    assert(hasParent());
    return m_parent->getChild(getIndex());
}

void Object::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Object::isEnabled() const {
    return m_enabled;
}

bool Object::isActive() const {
    return m_enabled && getParent()->isActive();
}

void Object::destroy() {
    assert(hasParent());
    m_parent->destroyChild(m_index);
}

bool Object::isRoot() const {
    return m_isRoot;
}

//==============================================================================
// Layer Functions
//==============================================================================

void Object::setLayer(std::size_t layer) {
    assert(layer < m_engine->getLayerCount());
    m_layer = layer;
}

std::size_t Object::getLayer() const {
    return m_layer;
}

void Object::incrementLayer() {
    if (m_layer < (m_engine->getLayerCount() - 1))
        m_layer++;
}

void Object::decrementLayer() {
    if (m_layer > 0)
        m_layer--;
}

void Object::sendToBack() {
    m_layer = 0;
}

void Object::sendToFront() {
    m_layer = m_engine->getLayerCount() - 1;
}

//==============================================================================
// Children
//==============================================================================

std::size_t Object::getChildCount() const {
    return m_children.size();
}

void Object::attachChild(Ptr<Object> gameObject) {
    gameObject->m_parent = this;
    gameObject->setEngine(m_engine);
    if (!m_iteratingChildren) {
        m_children.push_back(std::move(gameObject));
        updateChildIndices();
    }
    else {
        m_childrenAdd.push_back(std::move(gameObject));
    }
}

Ptr<Object> Object::detachChild(std::size_t index) {
    assert(index < m_children.size());
    if (!m_iteratingChildren) {
        // children are not being iterated, so detach child now
        Ptr<Object> obj = std::move(m_children[index]);
        m_children.erase(m_children.begin() + index);
        obj->m_parent = nullptr;
        obj->setEngine(nullptr);
        updateChildIndices();
        return obj;
    }
    else {
        // children are being iterated, so defer removal until later
        m_children[index]->m_parent = nullptr;
        m_children[index]->setEngine(nullptr);
        destroyChild(index);
        return m_children[index];
    }
}

void Object::destroyChild(std::size_t index) {
    assert(index < m_children.size());
    m_childrenDel.push_back(m_children[index].get());
}

void Object::destroyChildren() {
    for (auto& child : m_children)
        m_childrenDel.push_back(child.get());
}

Handle<Object> Object::getChild(std::size_t index) {
    assert(index < m_children.size());
    return Handle<Object>(m_children[index]);
}

void Object::makeChildFirst(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<Object> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.insert(m_children.begin(), std::move(obj));
    updateChildIndices();
}

void Object::makeChildLast(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<Object> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.push_back(std::move(obj));
    updateChildIndices();
}

Handle<Object> Object::findChild(Id id) {
    for (auto& child : m_children) {
        if (child->m_id == id)
            return Handle<Object>(child);
    }
    return Handle<Object>();
}

Handle<Object> Object::findChild(const Name& name) {
    Id id = ID::getId(name);
    return findChild(id);
}

bool Object::isParentOf(Handle<Object> object) {
    return object->m_parent == this;
}

//==============================================================================
// Parent
//==============================================================================

bool Object::hasParent() const {
    return m_parent != nullptr;
}

Handle<Object> Object::getParent() const {
    assert(hasParent());
    return m_parent->getHandle();
}

std::size_t Object::getIndex() const {
    return m_index;
}

bool Object::isChildOf(Handle<Object> object) {
    return m_parent == object.get();
}

//==============================================================================
// Component
//==============================================================================

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

std::size_t Object::getComponentCount() const {
    return m_components.size();
}

void Object::removeComponent(std::size_t index) {
    assert (index < m_componentsDel.size());
    if (!m_iteratingComponents) {
        m_components.erase(m_components.begin() + index);
        updateComponentIndices();
    }
    else {
        m_componentsDel.push_back(m_components[index].get());
    }
}

Handle<Component> Object::attachComponent(Ptr<Component> component) {
    component->m_object = this;
    component->setEngine(m_engine);
    auto h = Handle<Component>(component);
    if (!m_iteratingComponents) {
        m_components.push_back(std::move(component));
        updateComponentIndices();
        for (auto& other : m_components)
            other->onComponentAdded(h);
    }
    else {
        m_componentsAdd.push_back(std::move(component));
    }
    return h;
}


//==============================================================================
// Public Static Functions
//==============================================================================

std::size_t Object::getObjectCount() {
    return g_objectCount;
}

//==============================================================================
// Protected Functions
//==============================================================================

Engine& Object::engine() const {
    return *m_engine;
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

void Object::draw(RenderTarget& target, RenderStates states) const {
    // do nothing by default
}

void Object::onRenamed(const Name& newName) {
    // do nothing by default
}

//==============================================================================
// Private Functions
//==============================================================================

void Object::setEngine(Engine* engine) {
    m_engine = engine;
    for (const auto& child : m_children)
        child->setEngine(m_engine);
    for (const auto& comp : m_components)
        comp->setEngine(m_engine);
}

void Object::updateChildIndices() {
    for (std::size_t i = 0; i < m_children.size(); ++i)
        m_children[i]->m_index = i;
}

void Object::updateChildren() {
    m_iteratingChildren = true;
    for (const auto& child : m_children)
        child->updateAll();
    m_iteratingChildren = false;
}

void Object::processAdditions() {
    // process children
    while (!m_childrenAdd.empty()) {
        attachChild(std::move(m_childrenAdd.back()));
        m_childrenAdd.pop_back();
    }
    // process Components
    while (!m_componentsAdd.empty()) {
        attachComponent(std::move(m_componentsAdd.back()));
        m_componentsAdd.pop_back();
    }
}

void Object::processDeletions() {
    // process children
    while (!m_childrenDel.empty()) {
        detachChild(m_childrenDel.back()->getIndex());
        m_childrenDel.pop_back();
    }
    // process Components
    while (!m_componentsDel.empty()) {
        removeComponent(m_componentsDel.back()->getIndex());
        m_componentsDel.pop_back();
    }
}

void Object::updateAll() {
    processAdditions();
    if (m_enabled) {
        if (!m_startCalled) {
            start();
            m_startCalled = true;
        }
        updateComponents();
        update();
        if (hasCoroutines())
            resumeCoroutines();
        updateChildren();
    }
    processDeletions();
}

void Object::queRender(RenderQue& renderQue, RenderStates states) const {
    if (m_enabled) {
        // set states
        states.transform *= transform.getLocalMatrix();
        // que this Object
        renderQue[m_layer].emplace_back(this, states);
        // que children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->queRender(renderQue, states);
        m_iteratingChildren = false;
    }
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
