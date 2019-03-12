#pragma once

#include <SFVG/Common/Imports.hpp>
#include <SFVG/Graphics/Color.hpp>

namespace sfvg {

#define DEBUG_COLOR               Greens::Chartreuse
#define DEBUG_XAXIS_COLOR         Reds::Red
#define DEBUG_YAXIS_COLOR         Greens::Chartreuse
#define DEBUG_TRANSFORM_COLOR     Whites::White
#define DEBUG_LOCAL_BOUNDS_COLOR  Blues::Blue
#define DEBUG_WORLD_BOUNDS_COLOR  Cyans::Cyan
#define DEBUG_WIREFRAME_COLOR     Yellows::Yellow
#define DEBUG_PHYSICS_COG_COLOR   Purples::Magenta
#define DEBUG_PHYSICS_SHAPE_COLOR Purples::Magenta

namespace Debug {

/// Types of Gizmo that can be displayed
enum Gizmo {
    Transform,
    LocalBounds,
    WorldBounds,
    Wireframe,
    PhysicsCOG,
    PhysicsShapes,
    GizmoCount
};

/// Shows/hides general debug info in top right corner
void show(bool show);

/// Returns true if the debug info is show
bool isShown();

/// Draws a point in global coordinates
void drawPoint(const Vector2f& position,
                const Color& color = DEBUG_COLOR);

/// Draws a line in global coordinates
void drawLine(const Vector2f& start, const Vector2f& end,
                const Color& color = DEBUG_COLOR);

/// Draws unconnected lines between every two points in global coordinates
void drawLines(const std::vector<Vector2f>& points,
                const Color& color = DEBUG_COLOR);

/// Draws a connected polyline between a series of poits in global coordinates
void drawPolyline(const std::vector<Vector2f>& points,
                    const Color& color = DEBUG_COLOR);

/// Draws a triangle in global coordinates
void drawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c,
                    const Color& color = DEBUG_COLOR);

/// Draws a centered rectangle in global coordinates
void drawRectangle(const Vector2f& position, float width, float height,
                    const Color& color = DEBUG_COLOR);

/// Draws a circle in global coordinates
void drawCircle(const Vector2f& position, float radius,
                const Color& color = DEBUG_COLOR);

/// Draws a debug text label in global coordinates
void drawText(const std::string& text,
                const Vector2f& position,
                const Color& color = DEBUG_COLOR);

// Implementation details [internal use only]
namespace detail {
void init();
void update();
bool proceed();
bool gizmoActive(Gizmo gizmo);
} // namespace detail
} // namespace Debug
} // namespace sfvg
