#include <SFVG/Engine/Components/BoxCollider.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Components/RigidBody.hpp>
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
}

BoxCollider::BoxCollider(GameObject& _gameObject, float width, float height, float radius) :
    Collider(_gameObject)
{
    cpBody* body;
    if ((body = getBodyPointer())) {
        m_shape = cpBoxShapeNew(body, (cpFloat)width, (cpFloat)height, (cpFloat)radius);
        addShape(m_shape);
        cpShapeSetElasticity(m_shape, 0.1f);
        cpShapeSetFriction(m_shape, 0.1f);
        cpShapeSetMass(m_shape, cpBodyGetMass(body));
    }
    else {

    }
}

void BoxCollider::update() {
    int n = cpPolyShapeGetCount(m_shape);
    cpBody* body = getBodyPointer();
    for (int i = 0; i < n; ++i) {
        Point a = cp2sf(cpBodyLocalToWorld(body,cpPolyShapeGetVert(m_shape, i)));
        Point b = cp2sf(cpBodyLocalToWorld(body,cpPolyShapeGetVert(m_shape, ((i+1)%n))));
        engine.debug.drawLine(a, b, Purples::Magenta);
    }
}

} // namespace sfvg
