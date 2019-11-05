#include <Engine/Components/Trigger.hpp>
#include <Engine/Systems/InputSystem.hpp>
#include <Engine/GameObject.hpp>
#include <Utility/Math.hpp>

namespace carnot {

Trigger::Trigger(GameObject& _gameObject) :
    Component(_gameObject),
    shape(new Shape()),
    mode(Points),
    m_inside(false)
{
}

Trigger::Trigger(GameObject& _gameObject, Ptr<Shape> _shape, Mode mode) :
    Trigger(_gameObject)
{
    shape = std::move(_shape);
}

void Trigger::updateState(bool inside) {
    if (!m_inside && inside) {
        onMouseEnter.emit();
        // gameObject.onMouseEnter();
    }
    else if (m_inside && inside) {
        onMouseStay.emit();
        // gameObject.onMouseStay();
    }
    else if (m_inside && !inside) {
        onMouseExit.emit();
        // gameObject.onMouseExit();
    }
    m_inside = inside;
}

void Trigger::update() {
    /// check shape
    if (!shape)
        return;
    // get mouse position in shape's local coordinates
    auto localPos = gameObject.transform.worldToLocal(Input::getMousePosition());
    // test shape
    bool insideBounds = Math::inBounds(localPos, shape->getBounds());
    if (!insideBounds || mode == Bounds)
        updateState(insideBounds);
    else if (mode == Points) {
        bool insidePoints = shape->isInside(localPos, Shape::Points);
        updateState(insidePoints);
    }
    else if (mode == Vertices) {
        bool insideVertices = shape->isInside(localPos, Shape::Vertices);
        updateState(insideVertices);
    }
}

void Trigger::onGizmo() {

}

} // namespace carnot