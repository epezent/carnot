#pragma once
\
#include <Engine/Coroutine.hpp>
#include <Utility/Handle.hpp>
#include <Utility/Signal.hpp>
#include <Engine/Id.hpp>
#include <Utility/Types.hpp>
#include <Utility/Print.hpp>
#include <Utility/Random.hpp>
#include <vector>
#include <utility>


namespace carnot {

//==============================================================================
// Forward Declarations / Typedefs
//==============================================================================

class Engine;
class Object;
class Renderer;

typedef std::vector<std::vector<const Renderer*>> RenderQue;

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
    /// Enables or disables the Object for Engine processing
    void setEnabled(bool enabled);
    /// Returns true if the Object is enabled, false otherwise
    bool isEnabled() const;

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

    bool showGizmos;  ///< whether the Object's gizmos are shown

public:

    //==========================================================================
    // Public Static Functions
    //==========================================================================

    /// Returns the total number of Objects active in the Engine
    static std::size_t getObjectCount();
    

protected:

    friend class Engine;

    //==========================================================================
    // Virtual Callbacks
    //==========================================================================

    /// Called once during the lifetime of the Object before any calls to update
    virtual void start();
    /// Called once per frame
    virtual void update();
    /// Called once per frame
    virtual void lateUpdate();

    /// Called when the Object is renamed
    virtual void onRenamed(const Name& newName);
    /// Called when the Engine updates physics
    virtual void onPhysics();
    /// Called when the Engine renders the game
    virtual void onRender(RenderQue& que);
    /// Called when the Engine renders Gizmos
    virtual void onGizmo();
    /// Called when the Object is enabled
    virtual void onEnable();
    /// Called when the Obejct is disabled
    virtual void onDisable();

    //==========================================================================
    // Protected Functions
    //==========================================================================

    /// Resumes all active coroutines
    void resumeCoroutines();

private:

    //==========================================================================
    // Member Data
    //==========================================================================

    Id m_id;                              ///< unique Id of the Object
    bool m_enabled;                       ///< whether or not the Object is enabled
    std::vector<Enumerator> m_coroutines; ///< Coroutines

};

} // namespace carnot
