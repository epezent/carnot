#pragma once

#include <Engine/Component.hpp>
#include <Geometry/Shape.hpp>
#include <Utility/Signal.hpp>

namespace carnot {

/// Component which queries Shape for trigger events
class Trigger : public Component {
public:

    /// Triggering Query Mode
    enum Mode {
        Bounds,   ///< shape queried against points bounding box 
        Points,   ///< shape queried against points (e.g no rounded corneers)
        Vertices  ///< shape queried against vertices (e.g. rounded corners)
    };
    
    /// Constuctor
    Trigger(GameObject& gameObject);

    /// Constructor which takes a Shape
    Trigger(GameObject& gameObject, Ptr<Shape> shape, Mode mode = Points);
  
public:

    ProtectedSignal<void(void), Trigger> onMouseEnter; ///< emitted when the mouse enters the trigger
    ProtectedSignal<void(void), Trigger> onMouseStay;  ///< emitted when the mouse stays in the trigger
    ProtectedSignal<void(void), Trigger> onMouseExit;  ///< emitted when the mouse exits the trigger

    Mode mode;         ///< Trigger query mode
    Ptr<Shape> shape;  ///< Shape to be quried

private:

    virtual void update() override;
    void updateState(bool inside);
    virtual void onGizmo() override;

private:

    bool m_inside;

};

} // namespace carnot