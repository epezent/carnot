#pragma once

#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Components/Transform.hpp>

namespace sfvg {

class GameObject : public Object {
public:

    //==========================================================================
    // Constructor/Destructor
    //==========================================================================

    /// Constructs an GameObject with a default name (i.e. "obj0", "obj1", ...)
    GameObject();
    /// Constructs an GameObject with a defined name
    GameObject(const Name& name);

    //==========================================================================
    // General Functions
    //==========================================================================

    /// Ques the GameObject to be destroyed at the end of the parent GameObject's update
    void destroy();
    /// Returns ture if the GameObject is the root Engine object
    bool isRoot() const;
    /// Returns true if the GameObject is active (i.e. all parents enabled)
    bool isActive() const;
    /// Makes a Handle for this GameObject
    Handle<GameObject> getHandle() const;

    //==========================================================================
    // Child Functions
    //==========================================================================

    /// Get the number of children this GameObject parents
    std::size_t getChildCount() const;
    /// Get the number of children of a specific type
    template <typename T> std::size_t getChildCount();

    /// Makes a child GameObject of a specifc type and returns a handle to it
    template <typename T, typename ...Args> Handle<T> makeChild(Args... args);
    /// Add an already existing GameObject as a child
    void attachChild(Ptr<GameObject> object);
    /// Detach a child
    Ptr<GameObject> detachChild(std::size_t index);

    /// Ques a child to be destroyed at the end of update
    void destroyChild(std::size_t index);
    /// Ques all children to be destroyed at the end of update
    void destroyChildren();

    /// Returns a handle to a child by index
    Handle<GameObject> getChild(std::size_t index);

    /// Make a child the first child
    void makeChildFirst(std::size_t index);
    /// Make a child the last child
    void makeChildLast(std::size_t index);

    /// Finds a child GameObject by Id and returns a handle to it
    Handle<GameObject> findChild(Id id);
    /// Finds a child GameObject by Name and returns a handle to it
    Handle<GameObject> findChild(const Name& name);
    /// Finds the first child GameObject by type and returns a handle to it
    template <typename T> Handle<T> findChild();

    /// Returns true if this GameObject is the parent of the passed GameObject
    bool isParentOf(Handle<GameObject> object);

    //==========================================================================
    // Parent Functions
    //==========================================================================

    /// Returns true if the GameObject has a parent GameObject, false otherwise
    bool hasParent() const;
    /// Gets a handle to the GameObject's parent GameObject
    Handle<GameObject> getParent() const;
    /// Gets the sibling index of the GameObject
    std::size_t getIndex() const;
    /// Returns true if this GameObject is a child of the passed GameObject
    bool isChildOf(Handle<GameObject> object);

    //==========================================================================
    // Component Functions
    //==========================================================================

    /// Adds a Component of a specific type and returns a handle to it
    template <typename T, typename ...Args> Handle<T> addComponent(Args... args);
    /// Gets the number of Components attached to this GameObject
    std::size_t getComponentCount() const;
    /// Gets the first Component of a specific type
    template <typename T> Handle<T> getComponent();
    /// Removes the first Component of a specific type
    void removeComponent(std::size_t index);

private:

    friend class Engine;
    friend class Transform;

    /// Attaches a Component to this GameObject
    Handle<Component> attachComponent(Ptr<Component> component);
    /// Sets the Engine pointer of the GameObject. Called by owning Engine.
    // void setEngine(Engine* engine);
    /// Updates the m_index member variable of all children
    void updateChildIndices();
    /// Updates the m_index member variable of all components
    void updateComponentIndices();
    /// Updates child GameObjects
    void updateChildren();
    /// Updates Components
    void updateComponents();
    /// Process added GameObjects/Components
    void processAdditions();
    /// Processs deleted GameObjects/Components
    void processDeletions();

    /// Updates the GameObject and then recursively updates all of its children
    void updateAll();
    /// Updates the GameObject and then recursively updates all of its children
    void lateUpdateAll();
    /// Ques the GameObject for rendering and the recursively ques all of its children
    void onRender(RenderQue& que) final;
    /// Calls onDebugRender for all cildren and components
    void onDebugRender() final;
    /// Updates all Physics children and compoents
    void onPhysics() final;

private:

    std::vector<Ptr<GameObject>> m_children;    ///< list of current child GameObjects
    std::vector<Ptr<GameObject>> m_childrenAdd; ///< list of child GameObjects to be added
    std::vector<GameObject*>     m_childrenDel; ///< list of child GameObjects to be destroyed
    mutable bool m_iteratingChildren;       ///< true if children currently being iterated
    mutable bool m_startCalled;             ///< Has start() been called?

    std::vector<Ptr<Component>> m_components;
    std::vector<Ptr<Component>> m_componentsAdd;
    std::vector<Component*>     m_componentsDel;
    mutable bool m_iteratingComponents;

    GameObject* m_parent;                 ///< pointer to parent GameObject
    std::size_t m_index;                  ///< sibling index within parent GameObject
    bool m_isRoot;

 public:

    Transform& transform;  ///< reference to Transform Component

};

//==============================================================================
// Template / Inline Function Implementations
//==============================================================================

template <typename T>
std::size_t GameObject::getChildCount() {
    std::size_t count = 0;
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<T>(m_children[i]);
        if (ptr != nullptr)
            count++;
    }
    return count;
}

template <typename T, typename ...Args>
Handle<T> GameObject::makeChild(Args... args) {
    auto child = std::make_shared<T>(std::forward<Args>(args)...);
    attachChild(child);
    return Handle<T>(child);
}

template <typename T>
Handle<T> GameObject::findChild() {
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<T>(m_children[i]);
        if (ptr != nullptr)
            return Handle<T>(ptr);
    }
    return Handle<T>();
}

template <typename T, typename ...Args>
Handle<T> GameObject::addComponent(Args... args) {
    auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
    auto handle = attachComponent(std::move(component));
    return handle.as<T>();
}

template <typename T>
Handle<T> GameObject::getComponent() {
    for (auto& comp : m_components) {
        auto cast = std::dynamic_pointer_cast<T>(comp);
        if (cast)
            return Handle<T>(cast);
    }
    return Handle<T>();
}

}
