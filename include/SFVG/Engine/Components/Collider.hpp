#pragma once

#include <SFVG/Engine/Component.hpp>

struct cpShape;
struct cpBody;

namespace sfvg {

class RigidBody;

class Collider : public Component {
public:

    /// Constructor
    Collider(GameObject& gameObject);
    /// Destructor
    ~Collider();

protected:

    /// Gets Chipmunk body if it exits (nullptr otherwise)
    cpBody* getBodyPointer();
    /// Adds shape
    void addShape(cpShape* shape);
    /// Override for behavior when RigidBody added to GamObject
    virtual void onRigidBodyConnected(cpBody* body);
    /// Override for behavior when RigidBody removed from GameObject
    virtual void onRigidBodyDisconnected();

protected:

    cpShape* m_shape;
    cpBody*  m_dummyBody;

private:

    /// Handles addition of RigidBody components
    void onComponentAdded(Handle<Component> component) final;
    /// Handles removal of RigidBody components
    void onComponentRemoved(Handle<Component> component) final;

private:

    Handle<RigidBody> m_rb;  ///< RigidBody component
};

} // namespace sfvg
