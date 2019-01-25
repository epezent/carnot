#pragma once

#include <SFVG/Engine/Component.hpp>

struct cpBody;

namespace sfvg {

class Collider;

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

private:

    friend class Collider;

    cpBody* m_body;  ///< Chipmunk body
};


} // namespace sfvg
