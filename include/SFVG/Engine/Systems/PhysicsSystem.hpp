#pragma once

#include <SFVG/Engine/System.hpp>

struct cpSpace;

namespace sfvg {

class RigidBody;

class PhysicsSystem : private System {
public:

    /// Constructor
    PhysicsSystem(Engine& engine, const Name& name);
    /// Destructor
    ~PhysicsSystem();

    /// Set PhysicsSystem step time (default = 1/60);
    void setDeltaTime(float dt);

    /// Sets the global gravity vector (default = <0, 1000>)
    void setGravity(const Vector2f& g);
    /// Gets the global gravity vector
    Vector2f getGravity() const;

    /// Set global damping for 0 to 1 (default = 0.9)
    void setDamping(float damping);
    /// Get global damping
    float getDamping() const;

private:

    friend class Engine;
    friend class RigidBody;

    /// Steps the PhysicsSystem
    void update() override;

private:

    cpSpace* m_space;  /// Chipmunk space
    double m_dt;       /// delta time

};


} // namespace sfvg
