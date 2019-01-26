#pragma once

#include <SFVG/Engine/Component.hpp>

struct cpBody;
struct cpShape;

namespace sfvg {

class Shape;
class CircleShape;

class RigidBody : public Component {
public:

    /// RigidBody body type
    enum BodyType {
        Dynamic,
        Kinematic,
        Static
    };

public:

    /// Constructor
    RigidBody(GameObject& gameObject,
              BodyType type = Dynamic,
              float mass    = 1.0f,
              float moment  = 1.0f);
    /// Destructor
    ~RigidBody();

    //==============================================================================
    // PROPERTIES
    //==============================================================================

    /// Sets the BodyType
    void setBodyType(BodyType type);
    /// Gets the BodyType
    BodyType getBodyType() const;

    /// Set RigidBody mass
    void setMass(float mass);
    /// Get RigidBody mass
    float getMass() const;

    /// Set RigidBody moment
    void setMoment(float moment);
    /// Get RigidBody moment
    float getMoment() const;

    //==============================================================================
    // SHAPES
    //==============================================================================

    /// Adds a centered box shape to the RigidBody
    void addBoxShape(float width, float height, float radius = 0.0f);
    /// Adds a cenetered circle shape to the RigidBody
    void addCircleShape(float radius);
    /// Add a generic shape
    void addShape(const Shape& shape);
    /// Adds a CircleShape
    void addShape(const CircleShape& shape);

    /// Gets the number of shapes attached to RigidBody
    std::size_t getShapeCount() const;

    /// Set a shape's mass
    void setShapeMass(std::size_t index, float mass);
    /// Set a shape's friction
    void setShapeFriction(std::size_t index, float friction);
    /// Set a shape's elasticity
    void setShapeElasticity(std::size_t index, float elasticity);

    /// Get a shape's mass
    float getShapeMass(std::size_t index) const;
    /// Get a shape's moment
    float getShapeMoment(std::size_t index) const;
    /// Get a shape's friction
    float getShapeFriction(std::size_t index) const;
    /// Get a shape's elasticity
    float getShapeElasticity(std::size_t index) const;

    //==============================================================================
    // KINEMATIC
    //==============================================================================

    /// Set RigidBody velocity
    void setVelocity(const Vector2f& velocity);
    /// Get RigidBody velocity
    Vector2f getVelocity() const;

    //==============================================================================
    // DYNAMIC
    //==============================================================================

    /// Apply force to RigidBody center
    void applyForceToCenter(const Vector2f& force);
    /// Apply torque to RigidBody center
    void applyTorqueToCenter(float torque);

public:

    /// Gets the number of RigidBodys currently in the Engine
    static std::size_t getRigidBodyCount();

protected:

    /// Updates GameObject transform
    void onPhysics() override;
    /// Draws Shapes and RigidBody info
    void onDebugRender() override;

private:

    friend class Collider;

    cpBody* m_body;                 ///< Chipmunk body
    std::vector<cpShape*> m_shapes; ///< Chipmunk shapes
    std::vector<char>     m_mask;   ///< Shape type mask
};


} // namespace sfvg
