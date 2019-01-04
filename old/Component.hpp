#pragma once

#include <SFVG/Imports.hpp>
#include <SFVG/Engine/Handle.hpp>
#include <SFVG/Engine/Id.hpp>
#include <SFVG/Engine/Coroutine.hpp>

namespace sfvg {

class Object;
class Engine;

class Component : private NonCopyable {
public:

    //==========================================================================
    // Constructor/Destructor
    //==========================================================================

    /// Constructor
    Component();
    /// Destructor
    virtual ~Component();

    //==========================================================================
    // General Functions
    //==========================================================================

    /// Enables or disables the Component for updating processing
    void setEnabled(bool enabled);
    /// Returns true if the Component is enabled, false otherwise
    bool isEnabled() const;
    /// Returns true if the Component is active (i.e. all parents enabled)
    bool isActive() const;
    /// Returns the Object the component is attached to
    Handle<Object> object() const;
    /// Ques the Component to be destroyed at the end of the parent Object's update
    // void destroy();
    /// Gets the sibling index of the Component
    std::size_t getIndex() const;
    /// Returns true if this Component is attached to the passed Object
    // bool isComponentOf(Handle<Object> object);

    //==========================================================================
    // Coroutine Functions
    //==========================================================================

    /// Starts a new coroutine
    // Handle<Coroutine> startCoroutine(Enumerator&& routine);
    /// Stop a running coroutine
    // void stopCoroutine(Handle<Coroutine> routine);
    /// Stops all running coroutines
    // void stopAllCoroutines();
    /// Returns true if the Component has running coroutines
    // bool hasCoroutines() const;
    /// Returns the number of coroutines currently running
    // std::size_t getCoroutineCount() const;

protected:

    //==========================================================================
    // Protected Functions
    //==========================================================================

    Engine& getEngine() const;

    //==========================================================================
    // Virtual Functions
    //==========================================================================

    /// Called once per frame when the Component updates
    virtual void update();

private:

    friend class Engine;
    friend class Object;

    /// Sets the Engine pointer of the Object. Called by owning Engine.
    void setEngine(Engine* engine);

    void updateAll();

    /// Resumes all active coroutines
    // void resumeCoroutines();

private:

    bool m_enabled;                           ///< whether or not the Component is enabled
    Object* m_object;                         ///< pointer to Component's Object
    std::size_t m_index;                      ///< Component sibling index within Object
    Engine* m_engine;                         ///< pointer to Engine Component is managed by
    std::vector<Enumerator> m_coroutines;     ///< Coroutines

};

} // namspace sfvg

//

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
template <typename T> void removeComponent();

template <typename T> bool hasComponent();

template <typename T, typename ...Args>
Handle<T> Object::addComponent(Args... args) {
    auto component = std::make_shared<T>(args...);
    component->m_object = this;
    component->setEngine(m_engine);
    if (!m_iteratingComponents) {
        m_components.push_back(std::move(component));
        updateComponentIndices();
    }
    else {
        m_componentAdd.push_back(std::move(component));
    }
    return Handle<T>(component);
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
