#pragma once

#include <SFVG/Engine/System.hpp>
#include <SFVG/Imports.hpp>
#include <SFVG/Graphics.hpp>
#include <sstream>

namespace sfvg {

#define DEBUG_COLOR               Greens::Chartreuse
#define DEBUG_LOCAL_BOUNDS_COLOR  Blues::Blue
#define DEBUG_WORLD_BOUNDS_COLOR  Cyans::Cyan
#define DEBUG_WIREFRAME_COLOR     Yellows::Yellow
#define DEBUG_PHYSICS_COG_COLOR   Purples::Magenta
#define DEBUG_PHYSICS_SHAPE_COLOR Purples::Magenta

class DebugSystem : public System {
public:

    /// Holds system and game information
    struct Info;

    /// Types of Widgets that can be displayed
    enum Widget {
        LocalBounds,
        WorldBounds,
        Wireframe,
        PhysicsCOG,
        PhysicsShapes,
        WidgetCount
    };

    /// Shows/hides general debug info in top right corner
    void show(bool show);

    /// Returns true if the debug info is show
    bool isShown() const;

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
    void drawLineStrip(const std::vector<Vector2f>& points,
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

public:

    std::array<bool, WidgetCount> widgets;


private:

    friend class Engine;

    /// Constructor
    DebugSystem(Engine& engine, const Name& name);

    /// Starts the DebugSystem
    void start() override;

    /// Draws debug information
    void update() override;

    /// Informs Engine if it can proceed
    bool proceed();

    /// Updates info text
    void updateInfo();

    /// Draws Widget Selector
    void updateWidgetMenu();

private:

    bool m_show;

    bool m_paused;
    bool m_advance;

    Text m_infoText;
    Text m_pauseText;
    std::array<Text, WidgetCount> m_widgetLabels;

    std::vector<Ptr<Drawable>> m_drawables;

    Ptr<Info> m_info;
    std::ostringstream m_ss;
};

} // namespace sfvg
