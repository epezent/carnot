#pragma once

#include <Engine/Component.hpp>

class b2Body;
class b2Fixture;

namespace carnot {

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
    RigidBody(GameObject& gameObject, BodyType type = Dynamic);
    /// Destructor
    ~RigidBody();

    //==============================================================================
    // PROPERTIES
    //==============================================================================

    /// Sets the RigidBody position (world coordinates)
    void setPosition(const Vector2f& position);
    /// Sets the RigidBody position (world coordinates)
    void setPosition(float x, float y);
    /// Gets the RigidBody position (world coordinates)
    Vector2f getPosition() const;

    /// Sets the global rotation of the Object
    void setRotation(float angle);
    /// Gets the global rotation of the Object
    float getRotation() const;

    /// Sets the BodyType
    void setBodyType(BodyType type);
    /// Gets the BodyType
    BodyType getBodyType() const;

    /// Set RigidBody mass
    void setMass(float mass);
    /// Get RigidBody mass
    float getMass() const;

    /// Set RigidBody moment
    void setInertia(float inertia);
    /// Get RigidBody moment
    float getInertia() const;

    /// Get RigidBody center of gravity
    Vector2f getCOG() const;

    /// Set RigidBody linear damping
    void setLinearDamping(float damping);
    /// Gets RigidBody linear damping
    float getLinearDamping() const;

    /// Set RigidBody angular damping
    void setAngularDamping(float damping);
    /// Gets RigidBody angular damping
    float getAngularDamping() const;
    
    //==============================================================================
    // SHAPES
    //==============================================================================

    /// Add a generic shape (must be convex)
    void addShape(Ptr<Shape> shape, float density = 1.0f, float friction = 0.1f, float resitution = 0.0f);
    /// Adds a centered box shape to the RigidBody
    void addBoxShape(float width, float height, float density = 1.0f, float friction = 0.1f, float resitution = 0.0f);
    /// Adds a cenetered circle shape to the RigidBody
    void addCircleShape(float radius, const Vector2f& offset = Vector2f(), float density = 1.0f, float friction = 0.1f, float resitution = 0.0f);

    /// Gets the number of shapes attached to RigidBody
    std::size_t getShapeCount() const;

    /// Set a shape's density
    void setShapeDensity(std::size_t index, float density);
    /// Set a shape's friction
    void setShapeFriction(std::size_t index, float friction);
    /// Set a shape's elasticity
    void setShapeRestitution(std::size_t index, float restitution);

    /// Get a shape's mass
    float getShapeMass(std::size_t index) const;
    /// Get a shape's inertia
    float getShapeInertia(std::size_t index) const;
    /// Get a shape's density
    float getShapeDensity(std::size_t index) const;
    /// Get a shape's friction
    float getShapeFriction(std::size_t index) const;
    /// Get a shape's elasticity
    float getShapeRestitution(std::size_t index) const;

    //==============================================================================
    // KINEMATIC
    //==============================================================================

    /// Set RigidBody linear velocity
    void setVelocity(const Vector2f& velocity);
    /// Set RigidBody linear velocity
    void setVelocity(float vx, float vy);
    /// Get RigidBody linear velocity
    Vector2f getVelocity() const;

    //==============================================================================
    // DYNAMIC
    //==============================================================================

    /// Apply force to RigidBody center
    void applyForceToCenter(const Vector2f& force);
    /// Apply force to RigidBody center
    void applyForceToCenter(float fx, float fy);
    /// Apply torque to RigidBody center
    void applyTorqueToCenter(float torque);

public:

    /// Gets the number of RigidBodys currently in the Engine
    static std::size_t getRigidBodyCount();

private:

    /// Syncs RigidBody position/rotation with Transform
    void syncWithTransform();
    /// Updates GameObject transform
    void onPhysics() override;

private:

    friend class Collider;

    b2Body* m_body;                 ///< Physics system body
    std::vector<char>     m_mask;   ///< Shape type mask
};


} // namespace carnot
