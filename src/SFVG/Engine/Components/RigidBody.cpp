#include <SFVG/Engine/Components/RigidBody.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Common/Math.hpp>
#include <chipmunk/chipmunk_private.h>
#include <cassert>
#include <SFVG/Geometry/CircleShape.hpp>

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

static const char RBShapeTypePolygon = 0;
static const char RBShapeTypeCircle = 1;
static const char RBShapeTypeSegment = 2;

std::size_t g_rigidBodyCount = 0;

} // namespace

RigidBody::RigidBody(GameObject& _gameObject, BodyType type, float mass, float moment) :
    Component(_gameObject),
    m_body(nullptr)
{
    m_body = cpBodyNew((cpFloat)mass, (cpFloat)moment);
    setBodyType(type);
    // set initial position
    auto position = gameObject.transform.getPosition();
    auto rotation = gameObject.transform.getRotation();
    cpBodySetPosition(m_body, sf2cp(position));
    cpBodySetAngle(m_body, (cpFloat)rotation);
    cpSpaceAddBody(engine.physics.m_space, m_body);
    g_rigidBodyCount++;
}

RigidBody::~RigidBody() {
    for (auto& shape : m_shapes) {
        cpSpaceRemoveShape(engine.physics.m_space, shape);
        cpShapeFree(shape);
    }
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

Vector2f RigidBody::getCOG() const {
    return cp2sf(cpBodyLocalToWorld(m_body,cpBodyGetCenterOfGravity(m_body)));
}

//==============================================================================
// SHAPES
//==============================================================================

void RigidBody::addBoxShape(float width, float height, const Vector2f& offset, float skin) {
    cpShape* shape = cpBoxShapeNew(m_body, (cpFloat)width, (cpFloat)height, (cpFloat)skin);
    cpSpaceAddShape(engine.physics.m_space, shape);
    cpShapeSetFriction(shape, 0.5f);
    cpShapeSetMass(shape, getMass());
    m_shapes.push_back(shape);
    m_mask.push_back(RBShapeTypePolygon);
}

void RigidBody::addCircleShape(float radius, const Vector2f& offset) {
    cpShape* shape = cpCircleShapeNew(m_body, (cpFloat)radius, cpv(0,0));
    cpSpaceAddShape(engine.physics.m_space, shape);
    cpShapeSetFriction(shape, 0.5f);
    cpShapeSetMass(shape, getMass());
    m_shapes.push_back(shape);
    m_mask.push_back(RBShapeTypeCircle);
}

void RigidBody::addShape(const Shape& _shape, float skin) {
    // special case circle
    if (_shape.m_circleRadius > 0.0f && _shape.getScale() == Vector2f(1.0f,1.0f)) {
        return addCircleShape(_shape.m_circleRadius, _shape.getPosition());
    }
    // else it's a polygon
    auto verts = _shape.getVertices();
    int N = (int)verts.size();
    std::vector<cpVect> cpVerts;
    cpVerts.reserve(N);
    for (std::size_t i = 0; i < N; ++i)
        cpVerts.push_back(sf2cp(_shape.getTransform().transformPoint(verts[i])));
    cpShape* shape = cpPolyShapeNewRaw(m_body, N, &cpVerts[0], (cpFloat)skin);
    cpSpaceAddShape(engine.physics.m_space, shape);
    cpShapeSetFriction(shape, 0.5f);
    cpShapeSetMass(shape, getMass());
    m_shapes.push_back(shape);
    m_mask.push_back(RBShapeTypePolygon);
}

std::size_t RigidBody::getShapeCount() const {
    return m_shapes.size();
}


//==============================================================================
// PROPERTIES
//==============================================================================

void RigidBody::setPosition(const Vector2f& position) {
    cpBodySetPosition(m_body, sf2cp(position));
}

void RigidBody::setPosition(float x, float y) {
    setPosition(Vector2f(x,y));
}

Vector2f RigidBody::getPosition() const {
    return cp2sf(cpBodyGetPosition(m_body));
}

void RigidBody::setRotation(float angle) {
    cpBodySetAngle(m_body, (cpFloat)angle);
}

float RigidBody::getRotation() const {
    return (float)cpBodyGetAngle(m_body);
}

void RigidBody::setShapeMass(std::size_t index, float mass) {
    assert(index < m_shapes.size());
    cpShapeSetMass(m_shapes[index], (cpFloat)mass);
}

void RigidBody::setShapeDensity(std::size_t index, float density) {
    assert(index < m_shapes.size());
    cpShapeSetDensity(m_shapes[index], (cpFloat)density);
}

void RigidBody::setShapeFriction(std::size_t index, float friction) {
    assert(index < m_shapes.size());
    cpShapeSetFriction(m_shapes[index], (cpFloat)friction);
}

void RigidBody::setShapeElasticity(std::size_t index, float elasticity) {
    assert(index < m_shapes.size());
    cpShapeSetElasticity(m_shapes[index], (cpFloat)elasticity);
}

float RigidBody::getShapeMoment(std::size_t index) const {
    assert(index < m_shapes.size());
    return (float)cpShapeGetMoment(m_shapes[index]);
}

float RigidBody::getShapeMass(std::size_t index) const {
    assert(index < m_shapes.size());
    return (float)cpShapeGetMass(m_shapes[index]);
}

float RigidBody::getShapeDensity(std::size_t index) const {
    assert(index < m_shapes.size());
    return (float)cpShapeGetDensity(m_shapes[index]);
}

float RigidBody::getShapeFriction(std::size_t index) const {
    assert(index < m_shapes.size());
    return (float)cpShapeGetFriction(m_shapes[index]);
}

float RigidBody::getShapeElasticity(std::size_t index) const {
    assert(index < m_shapes.size());
    return (float)cpShapeGetElasticity(m_shapes[index]);
}

//==============================================================================
// KINEMATIC
//==============================================================================

void RigidBody::setVelocity(const Vector2f& velocity) {
    cpBodySetVelocity(m_body, sf2cp(velocity));
}

void RigidBody::setVelocity(float vx, float vy) {
    setVelocity(Vector2f(vx,vy));
}

Vector2f RigidBody::getVelocity() const {
    return cp2sf(cpBodyGetVelocity(m_body));
}

//==============================================================================
// DYNAMIC
//==============================================================================

void RigidBody::applyForceToCenter(const Vector2f& force) {
    auto pos = cpBodyGetPosition(m_body);
    cpBodyApplyForceAtWorldPoint(m_body, sf2cp(force), pos);
}

void RigidBody::applyForceToCenter(float fx, float fy) {
    applyForceToCenter(Vector2f(fx,fx));
}

void RigidBody::applyTorqueToCenter(float torque) {
    torque += (float)cpBodyGetTorque(m_body);
    cpBodySetTorque(m_body, (cpFloat)torque);
}

std::size_t RigidBody::getRigidBodyCount() {
    return g_rigidBodyCount;
}


//==============================================================================
// OVERRIDE
//==============================================================================

void RigidBody::onPhysics() {
    auto position = cpBodyGetPosition(m_body);
    auto angle = cpBodyGetAngle(m_body);
    gameObject.transform.setPosition((float) position.x, (float) position.y);
    gameObject.transform.setRotation((float) angle * RAD2DEG);
}

void RigidBody::onDebugRender() {
    // draw shapes
    if (engine.debug.widgets[DebugSystem::PhysicsShapes]) {
        for (std::size_t i = 0; i < getShapeCount(); ++i) {
            cpBody* body = cpShapeGetBody(m_shapes[i]);
            if (m_mask[i] == RBShapeTypePolygon) {
                int n = cpPolyShapeGetCount(m_shapes[i]);
                for (int j = 0; j < n; ++j) {
                    Point a = cp2sf(cpBodyLocalToWorld(body,cpPolyShapeGetVert(m_shapes[i], j)));
                    Point b = cp2sf(cpBodyLocalToWorld(body,cpPolyShapeGetVert(m_shapes[i], ((j+1)%n))));
                    engine.debug.drawLine(a, b, DEBUG_PHYSICS_SHAPE_COLOR);
                }
            }
            else if (m_mask[i] == RBShapeTypeCircle) {
                float r = (float)cpCircleShapeGetRadius(m_shapes[i]);
                Point pos = cp2sf(cpBodyLocalToWorld(body,cpCircleShapeGetOffset(m_shapes[i])));
                engine.debug.drawCircle(pos, r, DEBUG_PHYSICS_SHAPE_COLOR);
            }
            else if (m_mask[i] == RBShapeTypeSegment) {

            }
        }
    }
    // draw center of gravity
    if (engine.debug.widgets[DebugSystem::PhysicsCOG]) {
        Vector2f cog = getCOG();
        engine.debug.drawCircle(cog, 5, DEBUG_PHYSICS_COG_COLOR);
        engine.debug.drawPoint(cog + Vector2f(2,2), DEBUG_PHYSICS_COG_COLOR);
        engine.debug.drawPoint(cog - Vector2f(2,2), DEBUG_PHYSICS_COG_COLOR);

    }
}

} // namespace sfvg
