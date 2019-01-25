#pragma once

#include <SFVG/Engine/Object.hpp>

namespace sfvg {

class GameObject;

class Component : public Object {
public:

    //==========================================================================
    // Constructor/Destructor
    //==========================================================================

    /// Constructor
    Component(GameObject& gameObject);
    /// Destructor
    virtual ~Component();

    //==========================================================================
    // General Functions
    //==========================================================================

    /// Returns true if the Component is active (i.e. all parents enabled)
    bool isActive() const;
    /// Ques the Component to be destroyed at the end of the parent Object's update
    void destroy();
    /// Gets the sibling index of the Component
    std::size_t getIndex() const;

public:

    //==========================================================================
    // Public Static Functions
    //==========================================================================

    /// Returns the total number of Objects active in the Engine
    static std::size_t getComponentCount();

protected:

    //==========================================================================
    // Virtual Functions
    //==========================================================================

    /// Called when a new Component is added to the Object
    virtual void onComponentAdded(Handle<Component> component);
    /// Called when a Component was about to be removed
    virtual void onComponentRemoved(Handle<Component> component);

private:

    //==========================================================================
    // Private Functions
    //==========================================================================

    friend class Engine;
    friend class GameObject;

    /// Calls update and resumes coroutines
    void updateAll();

public:

    GameObject& gameObject;

private:

    bool m_startCalled;                       ///< true if start() has been called
    std::size_t m_index;                      ///< Component sibling index within Object

};

} // namspace sfvg
