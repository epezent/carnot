#pragma once

#include <SFVG/Engine/System.hpp>
#include <SFVG/Imports.hpp>
#include <SFVG/Graphics.hpp>
#include <sstream>

namespace sfvg {

#define DEBUG_COLOR Greens::Chartreuse

struct DebugInfo;

class DebugSystem : public System {
public:

    /// Shows/hides general debug info in top right corner
    void show(bool show);

    /// Returns true if the debug info is show
    bool isShown() const;

    /// Draws a debug point in global coordinates
    void drawPoint(const Point& position,
                   const Color& color = DEBUG_COLOR);

    /// Draws a debug line in global coordinates
    void drawLine(const Point& start, const Point& end,
                  const Color& color = DEBUG_COLOR);

    /// Draws a debug triangle in global coordinates
    void drawTriangle(const Point& a, const Point& b, const Point& c,
                      const Color& color = DEBUG_COLOR);

    /// Draws a debug centered rectangle in global coordinates
    void drawRectangle(const Point& position, float width, float height,
                       const Color& color = DEBUG_COLOR);

    void drawCircle(const Point& position, float radius,
                    const Color& color = DEBUG_COLOR);


    /// Draws a debug text label in global coordinates
    void drawText(const std::string& text,
                  const Point& position,
                  const Color& color = DEBUG_COLOR);

private:

    friend class Engine;

    /// Constructor
    DebugSystem(Engine& engine, const Name& name);

    /// Starts the DebugSystem
    void start() override;

    /// Draws debug information
    void update() override;

    /// Updates info text
    void updateInfo();

    /// Informs Engine if it can proceed
    bool proceed();

private:

    bool m_show;
    bool m_paused;
    bool m_advance;

    Text m_infoText;
    Text m_pauseText;

    std::vector<Ptr<Drawable>> m_drawables;

    Ptr<DebugInfo> m_info;
    std::ostringstream m_ss;
};

} // namespace sfvg
