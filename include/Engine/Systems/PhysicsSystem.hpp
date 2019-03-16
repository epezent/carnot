#pragma once

#include <Common/Imports.hpp>

struct cpSpace;

namespace carnot {
namespace Physics {

/// Set PhysicsSystem step time (default = 1/60);
void setDeltaTime(float dt);

/// Sets the global gravity vector (default = <0, 1000>)
void setGravity(const Vector2f& g);
/// Gets the global gravity vector
Vector2f getGravity();

/// Set global damping for 0 to 1 (default = 0.9)
void setDamping(float damping);
/// Get global damping
float getDamping();

// Implementation details [internal use only]
namespace detail {
void init();
void update();
void shutdown();
cpSpace* space();
} // namespace detail
} // namespace Physics
} // namespace carnot
