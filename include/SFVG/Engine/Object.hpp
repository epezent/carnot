#pragma once

#include <SFVG/Engine/Components/Transform.hpp>
#include <SFVG/Engine/Coroutine.hpp>
#include <SFVG/Engine/Handle.hpp>
#include <SFVG/Engine/Id.hpp>
#include <SFVG/Imports.hpp>
#include <SFVG/Print.hpp>
#include <SFVG/Random.hpp>
#include <vector>

namespace sfvg {

//==============================================================================
// Forward Declarations / Typedefs
//==============================================================================

class Engine;
class Object;

typedef std::vector<std::vector<std::pair<const Object*, RenderStates>>> RenderQue;

//==============================================================================
// CLASS: Object
//==============================================================================

class Object : private NonCopyable {

public:

    //==========================================================================
    // Constructor/Destructor
    //==========================================================================

    /// Constructs an Object with a default name (i.e. "obj0", "obj1", ...)
    Object();
    /// Constructs an Object with a defined name
    Object(const Name& name);
    /// Virtual Destructor
    virtual ~Object();

    //==========================================================================
    // General Functions
    //==========================================================================

    /// Sets the Name of an Object
    void setName(const Name& name);
    /// Gets the Name of an Object
    const Name& getName() const;
    /// Gets the Id number of an Object
    Id getId() const;
    /// Makes a Handle for this Object
    Handle<Object> getHandle() const;
    /// Enables or disables the Object for Engine processing
    void setEnabled(bool enabled);
    /// Returns true if the Object is enabled, false otherwise
    bool isEnabled() const;
    /// Returns true if the Object is active (i.e. all parents enabled)
    bool isActive() const;
    /// Ques the Object to be destroyed at the end of the parent Object's update
    void destroy();
    /// Returns ture if the Object is the root Engine object
    bool isRoot() const;

    //==========================================================================
    // Layer Functions
    //==========================================================================

    /// Set the render layer of the Object
    void setLayer(std::size_t layer);
    /// Get the render layer of the Object
    std::size_t getLayer() const;
    /// Increments render layer until Engine layer count reached
    void incrementLayer();
    /// Decrements render layer until Engine layer 0 reached
    void decrementLayer();
    /// Sets the render layer to the bottom layer
    void sendToBack();
    /// Sets the render layer to the top layer
    void sendToFront();

    //==========================================================================
    // Child Functions
    //==========================================================================

    /// Get the number of children this Object parents
    std::size_t getChildCount() const;
    /// Get the number of children of a specific type
    template <typename T> std::size_t getChildCount();

    /// Makes a child Object of a specifc type and returns a handle to it
    template <typename T, typename ...Args> Handle<T> makeChild(Args... args);
    /// Add an already existing Object as a child
    void attachChild(Ptr<Object> object);
    /// Detach a child
    Ptr<Object> detachChild(std::size_t index);

    /// Ques a child to be destroyed at the end of update
    void destroyChild(std::size_t index);
    /// Ques all children to be destroyed at the end of update
    void destroyChildren();

    /// Returns a handle to a child by index
    Handle<Object> getChild(std::size_t index);

    /// Make a child the first child
    void makeChildFirst(std::size_t index);
    /// Make a child the last child
    void makeChildLast(std::size_t index);

    /// Finds a child Object by Id and returns a handle to it
    Handle<Object> findChild(Id id);
    /// Finds a child Object by Name and returns a handle to it
    Handle<Object> findChild(const Name& name);
    /// Finds the first child Object by type and returns a handle to it
    template <typename T> Handle<T> findChild();

    /// Returns true if this Object is the parent of the passed Object
    bool isParentOf(Handle<Object> object);

    //==========================================================================
    // Parent Functions
    //==========================================================================

    /// Returns true if the Object has a parent Object, false otherwise
    bool hasParent() const;
    /// Gets a handle to the Object's parent Object
    Handle<Object> getParent() const;
    /// Gets the sibling index of the Object
    std::size_t getIndex() const;
    /// Returns true if this Object is a child of the passed Object
    bool isChildOf(Handle<Object> object);

    //==========================================================================
    // Component Functions
    //==========================================================================

    /// Adds a Component of a specific type and returns a handle to it
    template <typename T, typename ...Args> Handle<T> addComponent(Args... args);
    /// Gets the number of Components attached to this Object
    std::size_t getComponentCount() const;
    /// Gets the first Component of a specific type
    template <typename T> Handle<T> getComponent();
    /// Removes the first Component of a specific type
    void removeComponent(std::size_t index);

    //==========================================================================
    // Coroutine Functions
    //==========================================================================

    /// Starts a new coroutine
    Handle<Coroutine> startCoroutine(Enumerator&& routine);
    /// Stop a running coroutine
    void stopCoroutine(Handle<Coroutine> routine);
    /// Stops all running coroutines
    void stopAllCoroutines();
    /// Returns true if the Object has running coroutines
    bool hasCoroutines() const;
    /// Returns the number of coroutines currently running
    std::size_t getCoroutineCount() const;

public:

    //==========================================================================
    // Public Static Functions
    //==========================================================================

    /// Returns the total number of Objects active in the Engine
    static std::size_t getObjectCount();

    /// Makes a new unparented Object
    template <typename T, typename ...Args> static Ptr<T> make(Args ...);

protected:

    //==========================================================================
    // Protected Functions
    //==========================================================================

    Engine& engine() const;

    //==========================================================================
    // Virtual Functions
    //==========================================================================

    /// Called once during the lifetime of the Object before any calls to update
    virtual void start();
    /// Called once per frame when the Object updates
    virtual void update();
    /// Called once per frame when the Object is drawn
    virtual void draw(sf::RenderTarget& target, RenderStates states) const;
    /// Called when the Object is renamed
    virtual void onRenamed(const Name& newName);

private:

    //==========================================================================
    // Private Functions
    //==========================================================================

    friend class Engine;
    friend class Transform;

    /// Attaches a Component to this Object
    Handle<Component> attachComponent(Ptr<Component> component);
    /// Sets the Engine pointer of the Object. Called by owning Engine.
    void setEngine(Engine* engine);
    /// Updates the m_index member variable of all children
    void updateChildIndices();
    /// Updates the m_index member variable of all components
    void updateComponentIndices();
    /// Updates child Objects
    void updateChildren();
    /// Updates Components
    void updateComponents();
    /// Process added Objects/Components
    void processAdditions();
    /// Processs deleted Objects/Components
    void processDeletions();

    /// Updates the Object and then recursively updates all of its children
    void updateAll();
    /// Ques the Object for rendering and the recursively ques all of its children
    void queRender(RenderQue& renderQue, RenderStates states) const;

    /// Resumes all active coroutines
    void resumeCoroutines();

private:

    //==========================================================================
    // Member Data
    //==========================================================================

    Id m_id;                              ///< unique Id of the Object
    bool m_enabled;                       ///< whether or not the Object is enabled
    std::size_t m_layer;                  ///< the render layer of the Object

    std::vector<Ptr<Object>> m_children;    ///< list of current child Objects
    std::vector<Ptr<Object>> m_childrenAdd; ///< list of child Objects to be added
    std::vector<Object*>     m_childrenDel; ///< list of child Objects to be destroyed
    mutable bool m_iteratingChildren;       ///< true if children currently being iterated
    mutable bool m_startCalled;             ///< Has start() been called?

    std::vector<Ptr<Component>> m_components;
    std::vector<Ptr<Component>> m_componentsAdd;
    std::vector<Component*>     m_componentsDel;
    mutable bool m_iteratingComponents;

    Object* m_parent;                     ///< pointer to parent Object
    std::size_t m_index;                  ///< sibling index within parent Object
    Engine* m_engine;                     ///< pointer to Engine Object is managed by
    bool m_isRoot;                        ///< whether ot not the Object is the root Engine Object

    std::vector<Enumerator> m_coroutines;           ///< Coroutines

public:

    Transform& transform;
};

//==============================================================================
// Template / Inline Function Implementations
//==============================================================================

template <typename T>
std::size_t Object::getChildCount() {
    std::size_t count = 0;
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<T>(m_children[i]);
        if (ptr != nullptr)
            count++;
    }
    return count;
}

template <typename T, typename ...Args>
Handle<T> Object::makeChild(Args... args) {
    auto child = std::make_shared<T>(args...);
    attachChild(child);
    return Handle<T>(child);
}

template <typename T>
Handle<T> Object::findChild() {
    for (std::size_t i = 0; i < m_children.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<T>(m_children[i]);
        if (ptr != nullptr)
            return Handle<T>(ptr);
    }
    return Handle<T>();
}

template <typename T, typename ...Args>
Ptr<T> Object::make(Args... args) {
    return std::make_shared<T>(args...);
}

template <typename T, typename ...Args>
Handle<T> Object::addComponent(Args... args) {
    auto component = std::make_shared<T>(args...);
    return attachComponent(component).as<T>();
}

template <typename T>
Handle<T> Object::getComponent() {
    for (auto& comp : m_components) {
        auto cast = std::dynamic_pointer_cast<T>(comp);
        if (cast)
            return Handle<T>(cast);
    }
    return Handle<T>();
}

} // namespace sfvg
