#include <SFVG/Engine/Components/RigidBody.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Math.hpp>
#include <chipmunk.h>
#include <cassert>

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

std::size_t g_rigidBodyCount = 0;

} // namespace

RigidBody::RigidBody(GameObject& _gameObject, BodyType type, float mass, float moment) :
    Component(_gameObject),
    m_body(nullptr)
{
    m_body = cpBodyNew((cpFloat)mass, (cpFloat)moment);


    switch(type) {
        case BodyType::Kinematic: {
            m_body = cpBodyNewKinematic();
            break;
        }
        case BodyType::Static: {
            m_body = cpBodyNewStatic();
            break;
        }
        case BodyType::Dynamic:
        default: {
            m_body = cpBodyNew(mass, moment);
            break;
        }
    }


    // set initial position
    auto position = gameObject.transform.getGlobalPosition();
    auto rotation = gameObject.transform.getGlobalRotation();
    cpBodySetPosition(m_body, sf2cp(position));
    cpBodySetAngle(m_body, (cpFloat)rotation);
    cpSpaceAddBody(gameObject.engine.physics.m_space, m_body);
    g_rigidBodyCount++;
}

RigidBody::~RigidBody() {
    cpSpaceRemoveBody(gameObject.engine.physics.m_space, m_body);
    cpBodyFree(m_body);
    g_rigidBodyCount--;
}

void RigidBody::setBodyType(BodyType type) {
    switch(type) {
        case BodyType::Kinematic: {
            cpBodySetType(m_body, CP_BODY_TYPE_KINEMATIC);
            break;
        }
        case BodyType::Static: {
            cpBodySetType(m_body, CP_BODY_TYPE_STATIC);
            break;
        }
        case BodyType::Dynamic:
        default: {
            cpBodySetType(m_body, CP_BODY_TYPE_DYNAMIC);
            break;
        }
    }
}

RigidBody::BodyType RigidBody::getBodyType() const {
    cpBodyType type = cpBodyGetType(m_body);
    switch (type) {
        case CP_BODY_TYPE_KINEMATIC:
            return BodyType::Kinematic;
        case CP_BODY_TYPE_STATIC:
            return BodyType::Static;
        case CP_BODY_TYPE_DYNAMIC:
        default:
            return BodyType::Dynamic;
    }
}

void RigidBody::setMass(float mass) {
    cpBodySetMass(m_body, (cpFloat)mass);
}

float RigidBody::getMass() const {
    return (float)cpBodyGetMass(m_body);
}

void RigidBody::setMoment(float moment) {
    cpBodySetMoment(m_body, (cpFloat)moment);
}

float RigidBody::getMoment() const {
    return (float)cpBodyGetMoment(m_body);
}

void RigidBody::setVelocity(const Vector2f& velocity) {
    cpBodySetVelocity(m_body, sf2cp(velocity));
}

Vector2f RigidBody::getVelocity() const {
    return cp2sf(cpBodyGetVelocity(m_body));
}

void RigidBody::applyForceToCenter(const Vector2f& force) {
    auto pos = cpBodyGetPosition(m_body);
    cpBodyApplyForceAtWorldPoint(m_body, sf2cp(force), cpv(pos.x, pos.y));
}

void RigidBody::applyTorqueToCenter(float torque) {
    torque += (float)cpBodyGetTorque(m_body);
    cpBodySetTorque(m_body, (cpFloat)torque);
}

std::size_t RigidBody::getRigidBodyCount() {
    return g_rigidBodyCount;
}

void RigidBody::onPhysics() {
    auto position = cpBodyGetPosition(m_body);
    auto angle = cpBodyGetAngle(m_body);
    gameObject.transform.setGlobalPosition((float) position.x, (float) position.y);
    gameObject.transform.setGlobalRotation((float) angle * RAD2DEG);
}

} // namespace sfvg
