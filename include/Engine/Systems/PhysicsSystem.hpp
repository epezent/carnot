#pragma once

#include <Common/Types.hpp>
#include <Common/Handle.hpp>


struct cpSpace;
class b2World;

typedef b2World PhysicsWorld;

namespace carnot {
namespace Physics {

/// Set PhysicsSystem step time (default = 1/60);
void setDeltaTime(float dt);

/// Sets the global gravity vector (default = <0, 1000>)
void setGravity(const Vector2f& g);
/// Gets the global gravity vector
Vector2f getGravity();

// Implementation details [internal use only]
namespace detail {
void init();
void update();
void shutdown();
PhysicsWorld* world();
float scale();
float invScale();
} // namespace detail
} // namespace Physics
} // namespace carnot
