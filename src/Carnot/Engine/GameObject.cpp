#pragma warning (disable : 4100)

#include <Engine/Components/Transform.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Coroutine.hpp>
#include <algorithm>
#include <cmath>

namespace carnot {

GameObject::GameObject(const Name& name) :
    Object(name),
    m_iteratingChildren(false),
    m_startCalled(false),
    m_iteratingComponents(false),
    m_parent(nullptr),
    m_index(0),
    m_isRoot(false),
    transform(*attachComponent(std::make_shared<Transform>(*this)).as<Transform>().get())
{ }

GameObject::GameObject() :
    Object(),
    m_iteratingChildren(false),
    m_startCalled(false),
    m_iteratingComponents(false),
    m_parent(nullptr),
    m_index(0),
    m_isRoot(false),
    transform(*attachComponent(std::make_shared<Transform>(*this)).as<Transform>().get())
{ }

//==============================================================================
// General
//==============================================================================

Handle<GameObject> GameObject::getHandle() const {
    if (isRoot())
        return Engine::getRoot();
    assert(hasParent());
    return m_parent->getChild(getIndex());
}

bool GameObject::isActive() const {
    if (hasParent())
        return isEnabled() && m_parent->isActive();
    else
        return isEnabled();
}

void GameObject::destroy() {
    assert(hasParent());
    m_parent->destroyChild(m_index);
}

bool GameObject::isRoot() const {
    return m_isRoot;
}

//==============================================================================
// Children
//==============================================================================

std::size_t GameObject::getChildCount() const {
    return m_children.size();
}

void GameObject::attachChild(Ptr<GameObject> gameObject) {
    gameObject->m_parent = this;
    if (!m_iteratingChildren) {
        m_children.push_back(std::move(gameObject));
        updateChildIndices();
    }
    else {
        m_childrenAdd.push_back(std::move(gameObject));
    }
}

Ptr<GameObject> GameObject::detachChild(std::size_t index) {
    assert(index < m_children.size());
    if (!m_iteratingChildren) {
        // children are not being iterated, so detach child now
        Ptr<GameObject> obj = std::move(m_children[index]);
        m_children.erase(m_children.begin() + index);
        obj->m_parent = nullptr;
        updateChildIndices();
        return obj;
    }
    else {
        // children are being iterated, so defer removal until later
        m_children[index]->m_parent = nullptr;
        destroyChild(index);
        return m_children[index];
    }
}

void GameObject::destroyChild(std::size_t index) {
    assert(index < m_children.size());
    m_childrenDel.push_back(m_children[index].get());
}

void GameObject::destroyChildren() {
    for (auto& child : m_children)
        m_childrenDel.push_back(child.get());
}

Handle<GameObject> GameObject::getChild(std::size_t index) {
    assert(index < m_children.size());
    return Handle<Object>(m_children[index]);
}

void GameObject::makeChildFirst(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<GameObject> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.insert(m_children.begin(), std::move(obj));
    updateChildIndices();
}

void GameObject::makeChildLast(std::size_t index) {
    assert(!m_iteratingChildren);
    assert(index < m_children.size());
    Ptr<GameObject> obj = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    m_children.push_back(std::move(obj));
    updateChildIndices();
}

Handle<GameObject> GameObject::findChild(Id id) {
    for (auto& child : m_children) {
        if (child->getId() == id)
            return Handle<Object>(child);
    }
    return Handle<Object>();
}

Handle<GameObject> GameObject::findChild(const Name& name) {
    Id id = ID::getId(name);
    return findChild(id);
}

bool GameObject::isParentOf(Handle<GameObject> object) {
    return object->m_parent == this;
}

//==============================================================================
// Parent
//==============================================================================

bool GameObject::hasParent() const {
    return m_parent != nullptr;
}

Handle<GameObject> GameObject::getParent() const {
    assert(hasParent());
    return m_parent->getHandle();
}

std::size_t GameObject::getIndex() const {
    return m_index;
}

bool GameObject::isChildOf(Handle<GameObject> object) {
    return m_parent == object.get();
}

//==============================================================================
// Component
//==============================================================================

void GameObject::updateComponentIndices() {
    for (std::size_t i = 0; i < m_components.size(); ++i)
        m_components[i]->m_index = i;
}

void GameObject::updateComponents() {
    m_iteratingComponents = true;
    for (const auto& comp : m_components)
        comp->updateAll();
    m_iteratingComponents = false;
}

std::size_t GameObject::getComponentCount() const {
    return m_components.size();
}

void GameObject::removeComponent(std::size_t index) {
    assert (index < m_components.size());
    if (!m_iteratingComponents) {
        auto component = m_components.begin() + index;
        auto h = Handle<Component>(*component);
        for (auto& other : m_components) {
            other->onComponentRemoved(h);
        }
        m_components.erase(component);
        updateComponentIndices();
    }
    else {
        m_componentsDel.push_back(m_components[index].get());
    }
}

Handle<Component> GameObject::attachComponent(Ptr<Component> component) {
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
// Virtual Callbacks
//==============================================================================

void GameObject::onMouseEnter() {
    // do nothing by default
}

void GameObject::onMouseExit() {
    // do nothing by default
}

void GameObject::onMouseStay() {
    // do nothing by default
}

//==============================================================================
// Private Functions
//==============================================================================

void GameObject::updateChildIndices() {
    for (std::size_t i = 0; i < m_children.size(); ++i)
        m_children[i]->m_index = i;
}

void GameObject::updateChildren() {
    m_iteratingChildren = true;
    for (const auto& child : m_children)
        child->updateAll();
    m_iteratingChildren = false;
}

void GameObject::processAdditions() {
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

void GameObject::processDeletions() {
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

void GameObject::updateAll() {
    processAdditions();
    if (isEnabled()) {
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

void GameObject::lateUpdateAll() {
    processAdditions();
    if (isEnabled()) {
        // update components
        m_iteratingComponents = true;
        for (const auto& comp : m_components)
            comp->lateUpdate();
        m_iteratingComponents = false;
        // update self
        lateUpdate();
        // update children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->lateUpdateAll();
        m_iteratingChildren = false;
    }
    processDeletions();
}

void GameObject::onRender(RenderQue& que) {
    if (isEnabled()) {
        // que components
        m_iteratingComponents = true;
        for (const auto& comp : m_components)
            comp->onRender(que);
        m_iteratingComponents = false;
        // que children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->onRender(que);
        m_iteratingChildren = false;
    }
}

void GameObject::onGizmo() {
    if (isEnabled()) {
        // que components
        m_iteratingComponents = true;
        for (const auto& comp : m_components) {
            if (comp->isEnabled() && comp->showGizmos)
                comp->onGizmo();
        }
        m_iteratingComponents = false;
        // que children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->onGizmo();
        m_iteratingChildren = false;
    }
}

void GameObject::onPhysics() {
    if (isEnabled()) {
        // que components
        m_iteratingComponents = true;
        for (const auto& comp : m_components)
            comp->onPhysics();
        m_iteratingComponents = false;
        // que children
        m_iteratingChildren = true;
        for (const auto& child : m_children)
            child->onPhysics();
        m_iteratingChildren = false;
    }
}



} // namespace carnot
