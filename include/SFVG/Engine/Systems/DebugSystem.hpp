#pragma once

#include <SFVG/Engine/System.hpp>
#include <SFVG/Imports.hpp>
#include <sstream>

namespace sfvg {

struct DebugInfo;

class DebugSystem : public System {
public:

    /// Constructor
    DebugSystem(Engine& engine, const Name& name);

    /// Shows/hides general debug info in top right corner
    void show(bool show);

    /// Draws a debug line in global coordinates
    void drawLine(const Vector2f& start,
                  const Vector2f& end,
                  const Color& color = Color::Magenta);

    /// Draws a debug text label in global coordinates
    void drawLabel(const std::string& text,
                   const Vector2f& position,
                   const Color& color = Color::Magenta);

private:

    friend class Engine;

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
