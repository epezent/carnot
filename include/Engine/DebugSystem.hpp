#pragma once

#include <Utility/Types.hpp>
#include <Engine/Id.hpp>

namespace carnot {
namespace Debug {

struct DebugInfo {
    float fps;
    float cpu;
    float ram;
};

/// Shows/hides debug info and Gizmos
void show(bool show);

/// Returns true if the debug info is shown
bool isShown();

/// Enables/Disables F Keys
void setFunctionKeysEnabled(bool enabled);

/// Get FPS, CPU, and RAM usage (updated every second)
DebugInfo getInfo();

/// Adds a new Gizmo option and associated color
void addGizmo(const std::string& name, const Color& color);

/// Returns true if the user as selected a Gizmo to be active
bool gizmoActive(Id id);

/// Sets a Gizmo active state
void setGizmoActive(Id id, bool active);

/// Returns Id associated with Gizmo
Id gizmoId(const std::string& name);

/// Sets an existing Gizmo color
void setGizmoColor(Id id, const Color& color);

/// Return the color associated with a Gizmo
const Color& getGizmoColor(Id id);

/// Pauses or unpaues the application
void setPaused(bool pause);

/// Returns true if the application is currently paused
bool isPaused();

/// Draws a point in global coordinates
void drawPoint(const Vector2f& position, const Color& color);

/// Draws a line in global coordinates
void drawLine(const Vector2f& start, const Vector2f& end, const Color& color);

/// Draws unconnected lines between every two points in global coordinates
void drawLines(const std::vector<Vector2f>& points, const Color& color);

/// Draws a connected polyline between a series of points in global coordinates
void drawPolyline(const std::vector<Vector2f>& points, const Color& color);

/// Draws a triangle in global coordinates
void drawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c, const Color& color, bool fill = false);

/// Draws a centered rectangle in global coordinates
void drawRectangle(const Vector2f& position, float width, float height, const Color& color, bool fill = false);

/// Draws a polygon composed of vertices in global coordinates
void drawPolygon(const std::vector<Vector2f>& vertices, const Color& color, bool fill = false);

/// Draws a circle in global coordinates
void drawCircle(const Vector2f& position, float radius, const Color& color, bool fill = false);

/// Draws a debug text label in global coordinates
void drawText(const std::string& text, const Vector2f& position, const Color& color);

// Implementation details [internal use only]
namespace detail {
void init();
void shutdown();
void update();
bool proceed();
} // namespace detail
} // namespace Debug
} // namespace carnot
