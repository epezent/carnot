#include <Engine/Components/RigidBody.hpp>
#include <Engine/Engine.hpp>
#include <cassert>
#include <Geometry/CircleShape.hpp>
#include <Carnot/Glue/Box2D.inl>

namespace carnot {

namespace {

static const char RBShapeTypePolygon = 0;
static const char RBShapeTypeCircle  = 1;
static const char RBShapeTypeSegment = 2;

std::size_t g_rigidBodyCount = 0;

} // namespace

RigidBody::RigidBody(GameObject& _gameObject, BodyType type) :
    Component(_gameObject),
    m_body(nullptr)
{
    // create body definition
    b2BodyDef def;
    def.position = toB2D(gameObject.transform.getPosition());
    def.angle    = toB2D(gameObject.transform.getRotation());
    def.angularDamping = 0.5f;
    def.linearDamping = 0.5f;
    switch(type) {
        case BodyType::Kinematic: {
            def.type = b2BodyType::b2_kinematicBody;
            break;
        }
        case BodyType::Static: {
            def.type = b2BodyType::b2_staticBody;
            break;
        }
        case BodyType::Dynamic:
        default: {
            def.type = b2BodyType::b2_dynamicBody;
            break;
        }
    }
    m_body = Physics::detail::world()->CreateBody(&def);
    // set initial position
    syncWithTransform();
    g_rigidBodyCount++;
}

RigidBody::~RigidBody() {
    Physics::detail::world()->DestroyBody(m_body);
    g_rigidBodyCount--;
}

void RigidBody::setBodyType(BodyType type) {
    switch(type) {
        case BodyType::Kinematic: {
            m_body->SetType(b2BodyType::b2_kinematicBody);
            break;
        }
        case BodyType::Static: {
            m_body->SetType(b2BodyType::b2_staticBody);
            break;
        }
        case BodyType::Dynamic:
        default: {
            m_body->SetType(b2BodyType::b2_dynamicBody);
            break;
        }
    }
}

RigidBody::BodyType RigidBody::getBodyType() const {
    auto type = m_body->GetType();
    switch(type) {
        case b2BodyType::b2_kinematicBody :
            return BodyType::Kinematic;
            break;
        case b2BodyType::b2_staticBody :
            return BodyType::Static;
            break;
        case b2BodyType::b2_dynamicBody :
        default:
            return BodyType::Dynamic;
            break;    
    }
}

void RigidBody::setMass(float mass) {
    b2MassData data;
    m_body->GetMassData(&data);
    data.mass = mass;
    m_body->SetMassData(&data);
} 

float RigidBody::getMass() const {
    return m_body->GetMass();
}

void RigidBody::setInertia(float inertia) {
    // TODO
}

float RigidBody::getInertia() const {
    b2MassData data;
    m_body->GetMassData(&data);
    return data.I;
}

Vector2f RigidBody::getCOG() const {
    b2MassData data;
    m_body->GetMassData(&data);
    return fromB2D(data.center);
}

void RigidBody::setLinearDamping(float damping) {
    m_body->SetLinearDamping(damping);
}

float RigidBody::getLinearDamping() const {
    return m_body->GetLinearDamping();
}

void RigidBody::setAngularDamping(float damping) {
    m_body->SetAngularDamping(damping);
}

float RigidBody::getAngularDamping() const {
    return m_body->GetAngularDamping();
}

//==============================================================================
// SHAPES
//==============================================================================

void RigidBody::addBoxShape(float width, float height, float density, float friction, float restitution) {
    // shape
    b2PolygonShape square;
    square.SetAsBox(0.5f*width*Physics::detail::scale(), 0.5f*height*Physics::detail::scale());
    // fixture def
    b2FixtureDef def;
    def.density = density;
    def.friction = friction;
    def.restitution = restitution;
    def.shape = &square;
    // create
    m_body->CreateFixture(&def);
}

void RigidBody::addCircleShape(float radius, const Vector2f& offset, float density, float friction, float restitution) {
    // shape
    b2CircleShape circle;
    circle.m_p = toB2D(offset);
    circle.m_radius = Physics::detail::scale() * radius;
    // fixture def
    b2FixtureDef def;
    def.density = density;
    def.friction = friction;
    def.restitution = restitution;
    def.shape = &circle;
    // create
    m_body->CreateFixture(&def);
}

void RigidBody::addShape(Ptr<Shape> shape, float density, float friction, float restitution) {
    Ptr<CircleShape> maybeCircle = std::dynamic_pointer_cast<CircleShape>(shape);
    if (maybeCircle)
        addCircleShape(maybeCircle->getCircleRadius(), maybeCircle->getCenter(), density, friction, restitution);
    else {
        // shape
        assert(shape->getPointCount() <= b2_maxPolygonVertices);
        assert(shape->isConvex());
        std::vector<b2Vec2> verts(shape->getPointCount());
        for (std::size_t i = 0; i < shape->getPointCount(); ++i)
            verts[i] = toB2D(shape->getPoint(i));
        b2PolygonShape polygon;    
        polygon.Set(&verts[0], (int32)verts.size());
        // fixture def
        b2FixtureDef def;
        def.density = density;
        def.friction = friction;
        def.restitution = restitution;
        def.shape = &polygon;
        // create
        m_body->CreateFixture(&def);
    }
}

std::size_t RigidBody::getShapeCount() const {
    std::size_t count = 0;
    for (auto fix = m_body->GetFixtureList(); fix; fix = fix->GetNext())
        count++;
    return count;
}


//==============================================================================
// PROPERTIES
//==============================================================================

void RigidBody::setPosition(const Vector2f& position) {
    m_body->SetTransform(toB2D(position), m_body->GetAngle());
}

void RigidBody::setPosition(float x, float y) {
    setPosition(Vector2f(x,y));
}

Vector2f RigidBody::getPosition() const {
    return fromB2D(m_body->GetPosition());
}

void RigidBody::setRotation(float angle) {
    m_body->SetTransform(m_body->GetPosition(), toB2D(angle));
}

float RigidBody::getRotation() const {
    return fromB2D(m_body->GetAngle());
}

void RigidBody::setShapeDensity(std::size_t index, float density) {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    fix.SetDensity(density);
}

void RigidBody::setShapeFriction(std::size_t index, float friction) {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    fix.SetFriction(friction);
}

void RigidBody::setShapeRestitution(std::size_t index, float restitution) {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    fix.SetRestitution(restitution);
}

float RigidBody::getShapeMass(std::size_t index) const {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    b2MassData data;
    fix.GetMassData(&data);
    return data.mass;
}

float RigidBody::getShapeInertia(std::size_t index) const {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    b2MassData data;
    fix.GetMassData(&data);
    return data.I;
}

float RigidBody::getShapeDensity(std::size_t index) const {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    return fix.GetDensity();
}

float RigidBody::getShapeFriction(std::size_t index) const {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    return fix.GetFriction();
}

float RigidBody::getShapeRestitution(std::size_t index) const {
    assert(index < getShapeCount());
    auto fix = m_body->GetFixtureList()[index];
    return fix.GetRestitution();
}

//==============================================================================
// KINEMATIC
//==============================================================================

void RigidBody::setVelocity(const Vector2f& velocity) {
    m_body->SetLinearVelocity(toB2D(velocity));
}

void RigidBody::setVelocity(float vx, float vy) {
    setVelocity(Vector2f(vx,vy));
}

Vector2f RigidBody::getVelocity() const {
    return fromB2D(m_body->GetLinearVelocity());
}

//==============================================================================
// DYNAMIC
//==============================================================================

void RigidBody::applyForceToCenter(const Vector2f& force) {
    m_body->ApplyForceToCenter(toB2D(force), true);
}

void RigidBody::applyForceToCenter(float fx, float fy) {
    applyForceToCenter(Vector2f(fx,fy));
}

void RigidBody::applyTorqueToCenter(float torque) {
    m_body->ApplyTorque(torque * Physics::detail::scale(), true);
}

std::size_t RigidBody::getRigidBodyCount() {
    return g_rigidBodyCount;
}


//==============================================================================
// OVERRIDE
//==============================================================================

void RigidBody::onPhysics() {
    auto position = getPosition();
    auto angle = getRotation();
    gameObject.transform.setPosition(position);
    gameObject.transform.setRotation(angle);
}

//==============================================================================
// PRIVATE
//==============================================================================

void RigidBody::syncWithTransform() {
    setPosition(gameObject.transform.getPosition());
    setRotation(gameObject.transform.getRotation());
}

} // namespace carnot
