#pragma once

#include <Engine/Component.hpp>
#include <Geometry/Shape.hpp>

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