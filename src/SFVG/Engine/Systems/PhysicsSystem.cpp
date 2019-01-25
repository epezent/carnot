#include <SFVG/Engine/Systems/PhysicsSystem.hpp>
#include <chipmunk.h>

namespace sfvg {

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

} // namespace

PhysicsSystem::PhysicsSystem(Engine& engine, const Name& name) :
    System(engine, name),
    m_dt(1.0/120.0)
{
    m_space = cpSpaceNew();
    cpSpaceSetIterations(m_space, 10);
    // cpSpaceSetSleepTimeThreshold(m_space, 0.5f);
    // cpSpaceSetCollisionSlop(m_space, 0.5f);
    setGravity(Vector2f(0,1000));
    setDamping(0.9f);
}

PhysicsSystem::~PhysicsSystem() {
    cpSpaceFree(m_space);
}

void PhysicsSystem::setDeltaTime(float dt) {
    m_dt = static_cast<double>(dt);
}

void PhysicsSystem::setGravity(const Vector2f &g) {
    cpSpaceSetGravity(m_space, sf2cp(g));
}

void PhysicsSystem::setDamping(float damping) {
    cpSpaceSetDamping(m_space, (cpFloat)damping);
}

float PhysicsSystem::getDamping() const {
    return (float)cpSpaceGetDamping(m_space);
}

Vector2f PhysicsSystem::getGravity() const {
    return cp2sf(cpSpaceGetGravity(m_space));
}

void PhysicsSystem::update() {
    cpSpaceStep(m_space, m_dt);
}


} // namespace sfvg
