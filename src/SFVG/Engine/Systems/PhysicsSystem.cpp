#include <SFVG/Engine/Systems/PhysicsSystem.hpp>
#include <chipmunk/chipmunk.h>

namespace sfvg {

//==============================================================================
// GLOBALS
//==============================================================================

namespace {

inline cpVect sf2cp(const Vector2f& in) {
    cpVect out;
    out.x = static_cast<cpFloat>(in.x);
    out.y = static_cast<cpFloat>(in.y);
    return out;
}

inline Vector2f cp2sf(const cpVect& in) {
    Vector2f out;
    out.x = static_cast<float>(in.x);
    out.y = static_cast<float>(in.y);
    return out;
}

cpSpace* g_space;  /// Chipmunk space
double    g_dt;

} // namespace

//==============================================================================
// USER API
//==============================================================================

namespace Physics {

void setDeltaTime(float dt) {
    g_dt = static_cast<double>(dt);
}

void setGravity(const Vector2f &g) {
    cpSpaceSetGravity(g_space, sf2cp(g));
}

Vector2f getGravity() {
    return cp2sf(cpSpaceGetGravity(g_space));
}

void setDamping(float damping) {
    cpSpaceSetDamping(g_space, (cpFloat)damping);
}

float getDamping() {
    return (float)cpSpaceGetDamping(g_space);
}

//==============================================================================
// DETAIL
//==============================================================================

namespace detail {

void init()
{
    g_dt = 1.0 / 60.0;
    g_space = cpSpaceNew();
    cpSpaceSetIterations(g_space, 10);
    // cpSpaceSetSleepTimeThreshold(g_space, 0.5f);
    // cpSpaceSetCollisionSlop(g_space, 0.5f);
    setGravity(Vector2f(0,1000));
    setDamping(0.9f);
}

void update() {
    cpSpaceStep(g_space, g_dt);
}

void shutdown() {
    cpSpaceFree(g_space);
}

cpSpace* space() {
    return g_space;
}

} // namespace detail
} // namespace Physics
} // namespace sfvg
