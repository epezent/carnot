#pragma once

#include <SFVG/Engine/Component.hpp>
#include <functional>

namespace sfvg {

class Transform : public Component {
public:

    /// Constructor
    Transform(GameObject& gameObject);

    //==========================================================================
    // World Absolute Transformation Functions
    //==========================================================================

    /// Sets the global position of the Object
    void setPosition(const Vector2f& position);
    /// Sets the global position of the Object
    void setPosition(float x, float y);
    /// Gets the global position of the Object
    Vector2f getPosition() const;
    /// Sets the global rotation of the Object
    void setRotation(float angle);
    /// Gets the global rotation of the Object
    float getRotation() const;
    /// Sets the global scale of the Object
    void setScale(float factorX, float factorY);
    /// Sets the global scale of the Object
    void setScale(const Vector2f& factors);
    /// Gets the global scale of the Object
    const Vector2f getScale() const;
    /// Gets the global Transform of the Object
    Matrix3x3 getWorldMatrix() const;
    /// Gets the inverse global Transform of the Object
    Matrix3x3 getInverseWorldMatrix() const;

    //==========================================================================
    // Local Absolute Transformation Functions
    //==========================================================================

    /// Sets the local position of the Object
    void setLocalPosition(float x, float y);
    /// Sets the local position of the Object
    void setLocalPosition(const Vector2f& position);
    /// Sets the local rotation of the Object
    void setLocalRotation(float angle);
    /// Sets the local scale of the Object
    void setLocalScale(float factorX, float factorY);
    /// Sets the local scale of the Object
    void setLocalScale(const Vector2f& factors);
    /// Sets the local origin of the Object
    void setLocalOrigin(float x, float y);
    /// Sets the local origin of the Object
    void setLocalOrigin(const Vector2f& origin);
    /// Gets the local position of the Object
    const Vector2f& getLocalPosition() const;
    /// Gets the local rotation of the Object
    float getLocalRotation() const;
    /// Gets the local scale of the Object
    const Vector2f& getLocalScale() const;
    /// Gets the local origin of the Object
    const Vector2f& getLocalOrigin() const;
    /// Gets the local Transform of the Object in Local coordinates
    const Matrix3x3& getLocalMatrix() const;
    /// Gets the inverse local Transform of the Object in Local coordinates
    const Matrix3x3& getInverseLocalMatrix() const;

    //==========================================================================
    // Local Relative Transformation Functions
    //==========================================================================

    /// Moves the Object by an offset amount in Local coordinates
    void move(float offsetX, float offsetY);
    /// Moves the Object by an offset ammount in Local coordinates
    void move(const Vector2f& offset);
    /// Rotates the Object by an offset ammout in Local coordinates
    void rotate(float angle);
    /// Scales the Object by factors X,Y in Local coordinates
    void scale(float factorX, float factorY);
    /// Scales the Object by a factor in Local coordinates
    void scale(const Vector2f& factor);

    //==========================================================================
    // World to Local Conversion Functions
    //==========================================================================

    /// Transforms local point to world point
    Vector2f localToWorld(const Vector2f& point);
    /// Transform local rect to world rect
    FloatRect localToWorld(const FloatRect& rect);
    /// Transforms world point to local point
    Vector2f worldToLocal(const Vector2f& point);
    /// Transform local rect to world rect
    FloatRect worldToLocal(const FloatRect& rect);

    //==========================================================================
    // Callbacks
    //==========================================================================

    /// Register a callback to be called when the Transform changes
    void registerCallback(std::function<void(void)> callback);

private:

    /// Makes the transform dirty
    void makeDirty();

    /// Calls all callbacks
    void callCallbacks();

    /// Renders the Transform axis handles during Debug mode
    void onDebugRender() override;

private:

    Vector2f          m_origin;                 ///< Local Origin of translation/rotation/scaling of the object
    Vector2f          m_position;               ///< Local Position of the object in the 2D world
    float             m_rotation;               ///< Local Orientation of the Object, in degrees
    Vector2f          m_scale;                  ///< Local Scale of the object

    mutable Matrix3x3 m_localTransform;         ///< Combined transformation of the object
    mutable Matrix3x3 m_worldTransform;         ///< Global tansform of the Object
    mutable Matrix3x3 m_invLocalTransform;      ///< Inverse combined transformation of the object
    mutable Matrix3x3 m_invWorldTransform;      ///< Inverse global transform of the Object

    mutable bool      m_localTransformDirty;    ///< Does the transform need to be recomputed?
    mutable bool      m_worldTransformDirty;    ///< Does the transform need to be recomputed?
    mutable bool      m_invLocalTransformDirty; ///< Does the transform need to be recomputed?
    mutable bool      m_invWorldTransformDirty; ///< Does the transform need to be recomputed?

    std::vector<std::function<void(void)>> m_callbacks;  ///< callbacks called when Transform changes

};

} // namespace sfvg
