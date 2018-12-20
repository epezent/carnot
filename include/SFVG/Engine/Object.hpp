#pragma once

#include <vector>
#include <SFVG/Engine/Engine.hpp>

//==============================================================================
// CLASS: Object
//==============================================================================

class Object : public Drawable, private NonCopyable {

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
    // Local Transformation Functions
    //==========================================================================

    /// Sets the local position of the Object
    void setPosition(float x, float y);
    /// Sets the local position of the Object
    void setPosition(const Vector2f& position);
    /// Sets the local rotation of the Object
    void setRotation(float angle);
    /// Sets the local scale of the Object
    void setScale(float factorX, float factorY);
    /// Sets the local scale of the Object
    void setScale(const Vector2f& factors);
    /// Sets the local origin of the Object
    void setOrigin(float x, float y);
    /// Sets the local origin of the Object
    void setOrigin(const Vector2f& origin);
    /// Gets the local position of the Object
    const Vector2f& getPosition() const;
    /// Gets the local rotation of the Object
    float getRotation() const;
    /// Gets the local scale of the Object
    const Vector2f& getScale() const;
    /// Gets the local origin of the Object
    const Vector2f& getOrigin() const;
    /// Moves the Object by an offset amount
    void move(float offsetX, float offsetY);
    /// Moves the Object by an offset ammount
    void move(const Vector2f& offset);
    /// Rotates the Object by an offset ammout
    void rotate(float angle);
    /// Scales the Object by factors X,Y
    void scale(float factorX, float factorY);
    /// Scales the Object by a factor
    void scale(const Vector2f& factor);
    /// Gets the local Transform of the Object
    const Transform& getTransform() const;
    /// Gets the inverse local Transform of the Object
    const Transform& getInverseTransform() const;

    //==========================================================================
    // Global Transformatioins Functions
    //==========================================================================

    /// Gets the global Transform of the Object
    Transform getGlobalTransform() const;
    /// Gets the inverse global Transform of the Object
    Transform getInverseGlobalTransform() const;
    /// Sets the global position of the Object
    void setGlobalPosition(const sf::Vector2f& position);
    /// Sets the global position of the Object
    void setGlobalPosition(float x, float y);
    /// Gets the global position of the Object
    sf::Vector2f getGlobalPosition() const;
    /// Sets the global rotation of the Object
    void setGlobalRotation(float angle);
    /// Gets the global rotation of the Object
    float getGlobalRotation() const;
    /// Sets the global scale of the Object
    void setGlobalScale(float factorX, float factorY);
    /// Sets the global scale of the Object
    void setGlobalScale(const Vector2f& factors);
    /// Gets the global scale of the Object
    const Vector2f getGlobalScale() const;

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

    Engine& getEngine() const;

    //==========================================================================
    // Virtual Functions
    //==========================================================================

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

    /// Sets the Engine pointer of the Object. Called by owning Engine.
    void setEngine(Engine* engine);

    /// Updates the m_index member variable of all children
    void updateChildIndices();

    /// Updates the Object and then recursively updates all of its children
    void updateAll();
    /// Ques the Object for rendering and the recursively ques all of its children
    void queRender(RenderQue& renderQue, RenderStates states) const;

private:

    //==========================================================================
    // Member Data
    //==========================================================================

    Id m_id;                              ///< unique Id of the Object
    bool m_enabled;                       ///< whether or not the Object is enabled
    std::size_t m_layer;                  ///< the render layer of the Object
    std::vector<Ptr<Object>> m_children;  ///< list of current child Objects
    std::vector<Ptr<Object>> m_additions; ///< list of child Objects to be added
    std::vector<Object*> m_deletions;     ///< list of child Objects to be destroyed
    Object* m_parent;                     ///< pointer to parent Object
    std::size_t m_index;                  ///< sibling index within parent Object
    Engine* m_engine;                     ///< pointer to Engine Object is managed by
    bool m_isRoot;                        ///< whether ot not the Object is the root Engine Object
    mutable bool m_iteratingChildren;     ///< true if children currently being iterated

    Vector2f          m_origin;                     ///< Origin of translation/rotation/scaling of the object
    Vector2f          m_position;                   ///< Position of the object in the 2D world
    float             m_rotation;                   ///< Orientation of the Object, in degrees
    Vector2f          m_scale;                      ///< Scale of the object

    mutable Transform m_transform;                  ///< Combined transformation of the object
    mutable Transform m_globalTransform;            ///< Global tansform of the Object
    mutable Transform m_inverseTransform;           ///< Inverse combined transformation of the object
    mutable Transform m_inverseGlobalTransform;     ///< Inverse global transform of the Object

    mutable bool      m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
    mutable bool      m_globalTransformNeedUpdate;  ///< Does the transform need to be recomputed?
    mutable bool      m_inverseTransformNeedUpdate; ///< Does the transform need to be recomputed?
    mutable bool      m_invGlobTransformNeedUpdate; ///< Does the transform need to be recomputed?
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
