#pragma once

#include <SFVG/Engine/Component.hpp>

namespace sfvg {

class Transform : public Component {
public:

    /// Constructor
    Transform();

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
    const Matrix3x3& getLocalMatrix() const;
    /// Gets the inverse local Transform of the Object
    const Matrix3x3& getInverseLocalMatrix() const;

    //==========================================================================
    // Global Transformatioins Functions
    //==========================================================================

    /// Gets the global Transform of the Object
    Matrix3x3 getGlobalMatrix() const;
    /// Gets the inverse global Transform of the Object
    Matrix3x3 getInverseGlobalMatrix() const;
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

public:

    Vector2f          m_origin;                     ///< Origin of translation/rotation/scaling of the object
    Vector2f          m_position;                   ///< Position of the object in the 2D world
    float             m_rotation;                   ///< Orientation of the Object, in degrees
    Vector2f          m_scale;                      ///< Scale of the object

    mutable Matrix3x3 m_transform;                  ///< Combined transformation of the object
    mutable Matrix3x3 m_globalTransform;            ///< Global tansform of the Object
    mutable Matrix3x3 m_inverseTransform;           ///< Inverse combined transformation of the object
    mutable Matrix3x3 m_inverseGlobalTransform;     ///< Inverse global transform of the Object

    mutable bool      m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
    mutable bool      m_globalTransformNeedUpdate;  ///< Does the transform need to be recomputed?
    mutable bool      m_inverseTransformNeedUpdate; ///< Does the transform need to be recomputed?
    mutable bool      m_invGlobTransformNeedUpdate; ///< Does the transform need to be recomputed?

};

} // namespace sfvg
