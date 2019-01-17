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
    void destroy();
    /// Gets the sibling index of the Component
    std::size_t getIndex() const;
    /// Returns true if this Component is attached to the passed Object
    // bool isComponentOf(Handle<Object> object);

    //==========================================================================
    // Coroutine Functions
    //==========================================================================

    /// Starts a new coroutine
    Handle<Coroutine> startCoroutine(Enumerator&& routine);
    /// Stop a running coroutine
    void stopCoroutine(Handle<Coroutine> routine);
    /// Stops all running coroutines
    void stopAllCoroutines();
    /// Returns true if the Component has running coroutines
    bool hasCoroutines() const;
    /// Returns the number of coroutines currently running
    std::size_t getCoroutineCount() const;

public:

    //==========================================================================
    // Public Static Functions
    //==========================================================================

    /// Returns the total number of Objects active in the Engine
    static std::size_t getComponentCount();

protected:

    //==========================================================================
    // Protected Functions
    //==========================================================================

    Engine& engine() const;

    //==========================================================================
    // Virtual Functions
    //==========================================================================

    /// Called once during the lifetime of the Component before any calls to update
    virtual void start();
    /// Called once per frame when the Component updates
    virtual void update();
    /// Called when a new Component is added to the Object
    virtual void onComponentAdded(Handle<Component> component);

private:

    //==========================================================================
    // Private Functions
    //==========================================================================

    friend class Engine;
    friend class Object;

    /// Sets the Engine pointer of the Object. Called by owning Engine.
    void setEngine(Engine* engine);

    /// Calls update and resumes coroutines
    void updateAll();

    /// Resumes all active coroutines
    void resumeCoroutines();

private:

    bool m_enabled;                           ///< whether or not the Component is enabled
    bool m_startCalled;                       ///< true if start() has been called
    Object* m_object;                         ///< pointer to Component's Object
    std::size_t m_index;                      ///< Component sibling index within Object
    Engine* m_engine;                         ///< pointer to Engine Component is managed by
    std::vector<Enumerator> m_coroutines;     ///< Coroutines


};

} // namspace sfvg
