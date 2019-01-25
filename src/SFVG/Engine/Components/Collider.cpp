#include <SFVG/Engine/Components/Collider.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Components/RigidBody.hpp>
#include <chipmunk.h>

namespace sfvg {

Collider::Collider(GameObject& _gameObject) :
    Component(_gameObject),
    m_shape(nullptr),
    m_dummyBody(nullptr)
{
    // check to see if we have a RigidBody
    m_rb = gameObject.getComponent<RigidBody>();
    if (m_rb)
        print(getName(), "Collider: RigidBody found on GameObject",gameObject.getName());
    else
        print(getName(), "Collider: No RigidBody found");
}

Collider::~Collider() {
    if (m_shape) {
        cpSpaceRemoveShape(gameObject.engine.physics.m_space, m_shape);
        cpShapeFree(m_shape);
    }
    if (m_dummyBody) {
        cpSpaceRemoveBody(gameObject.engine.physics.m_space, m_dummyBody);
        cpBodyFree(m_dummyBody);
    }
}

void Collider::addShape(cpShape* shape) {
    cpSpaceAddShape(engine.physics.m_space, shape);
}

cpBody* Collider::getBodyPointer() {
    if (m_rb)
        return m_rb->m_body;
    return nullptr;
}

void Collider::onRigidBodyConnected(cpBody* body) {
    // do nothing by default
}

void Collider::onRigidBodyDisconnected() {
    // do nothing by default
}

void Collider::onComponentAdded(Handle<Component> component) {
    auto candidate = component.as<RigidBody>();
    if (candidate) {
        print(getName(), "Collider: RigidBody added to GameObject",gameObject.getName());
        m_rb = candidate;
        onRigidBodyConnected(m_rb->m_body);
    }
}

void Collider::onComponentRemoved(Handle<Component> component) {
    if (component == m_rb) {
        print(getName(), "Collider: RigidBody removed from GameObject",gameObject.getName());
        onRigidBodyDisconnected();
    }
}

} // namespace sfvg
