#include <SFVG/Engine/Systems/DebugSystem.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Common/Alignment.hpp>
#include <SFVG/Engine/ProcessInfo.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/Components/RigidBody.hpp>
#include <SFVG/Common/Math.hpp>

namespace sfvg {

namespace {
    static const std::array<std::string, DebugSystem::WidgetCount> g_widgetNames {
        "Local Bounds",
        "World Bounds",
        "Wireframe",
        "Physics COG",
        "Physics Shapes"
    };

    static std::array<Color, DebugSystem::WidgetCount> g_widgetColors;

} // namespace name

struct DebugSystem::Info {
    float       elapsedTime = 0.0f;
    std::size_t frames = 0;
    std::size_t fpsDisplay = 0;
    std::size_t framesDisplay = 0;
    double cpuSum = 0.0;
    double cpuDisplay = 0.0;
    std::size_t ramSum = 0;
    std::size_t ramDisplay = 0;
};

DebugSystem::DebugSystem(Engine& engine, const Name& name) :
    System(engine, name),
    m_show(false),
    m_paused(false),
    m_advance(false),
    m_info(new DebugSystem::Info())
{
    widgets.fill(false);
    g_widgetColors[0] =  DEBUG_LOCAL_BOUNDS_COLOR;
    g_widgetColors[1] =  DEBUG_WORLD_BOUNDS_COLOR;
    g_widgetColors[2] =  DEBUG_WIREFRAME_COLOR;
    g_widgetColors[3] =  DEBUG_PHYSICS_COG_COLOR;
    g_widgetColors[4] =  DEBUG_PHYSICS_SHAPE_COLOR;

}

void DebugSystem::show(bool _show) {
    m_show = _show;
}

bool DebugSystem::isShown() const {
    return m_show;
}

void DebugSystem::drawPoint(const Vector2f& position, const Color& color) {
    auto rect = std::make_shared<VertexArray>(sf::TriangleStrip);
    rect->resize(4);
    (*rect)[0].position = position + Vector2f( 2.0f, -2.0f);
    (*rect)[1].position = position + Vector2f(-2.0f, -2.0f);
    (*rect)[2].position = position + Vector2f( 2.0f,  2.0f);
    (*rect)[3].position = position + Vector2f(-2.0f,  2.0f);
    (*rect)[0].color = color;
    (*rect)[1].color = color;
    (*rect)[2].color = color;
    (*rect)[3].color = color;
    m_drawables.push_back(rect);
}

void DebugSystem::drawLine(const Vector2f& start,
              const Vector2f& end,
              const Color& color)
{
    auto line = std::make_shared<VertexArray>(sf::Lines);
    line->resize(2);
    (*line)[0].position = start;
    (*line)[0].color = color;
    (*line)[1].position = end;
    (*line)[1].color = color;
    m_drawables.push_back(line);
}

void DebugSystem::drawLines(const std::vector<Vector2f> &points, const Color& color) {
    auto lines = std::make_shared<VertexArray>(sf::Lines);
    lines->resize(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        (*lines)[i].position = points[i];
        (*lines)[i].color    = color;
    }
    m_drawables.push_back(lines);
}

void DebugSystem::drawLineStrip(const std::vector<Vector2f> &points, const Color& color) {
    auto lines = std::make_shared<VertexArray>(sf::LineStrip);
    lines->resize(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        (*lines)[i].position = points[i];
        (*lines)[i].color    = color;
    }
    m_drawables.push_back(lines);
}

void DebugSystem::drawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c, const Color& color) {
    auto tri = std::make_shared<VertexArray>(sf::LineStrip);
    tri->resize(4);
    (*tri)[0].position = a;
    (*tri)[0].color = color;
    (*tri)[1].position = b;
    (*tri)[1].color = color;
    (*tri)[2].position = c;
    (*tri)[2].color = color;
    (*tri)[3].position = a;
    (*tri)[3].color = color;
    m_drawables.push_back(tri);
}

void DebugSystem::drawRectangle(const Vector2f& position, float width, float height,  const Color& color) {
    auto rect = std::make_shared<VertexArray>(sf::LineStrip);
    rect->resize(5);
    (*rect)[0].position = position + Vector2f(-width, -height) * 0.5f;
    (*rect)[1].position = position + Vector2f( width, -height) * 0.5f;
    (*rect)[2].position = position + Vector2f( width,  height) * 0.5f;
    (*rect)[3].position = position + Vector2f(-width,  height) * 0.5f;
    (*rect)[4].position = position + Vector2f(-width, -height) * 0.5f;
    (*rect)[0].color = color;
    (*rect)[1].color = color;
    (*rect)[2].color = color;
    (*rect)[3].color = color;
    (*rect)[4].color = color;
    m_drawables.push_back(rect);
}

void DebugSystem::drawCircle(const Vector2f &position, float radius, const Color& color) {
    std::size_t smoothness = 36;
    auto circle = std::make_shared<VertexArray>(sf::LineStrip);
    circle->resize(smoothness + 1);
    float angleIncrement = 2.0f * PI / smoothness;
    for (std::size_t i = 0; i < smoothness + 1; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        (*circle)[i].position = position + Vector2f(std::cos(angle) * radius, std::sin(angle) * radius);
        (*circle)[i].color = color;
    }
    m_drawables.push_back(circle);
}

void DebugSystem::drawText(const std::string& _text,
               const Vector2f& position,
               const Color& color)
{
    auto text = std::make_shared<Text>();
    text->setFont(engine.fonts.get("RobotoMonoBold"));
    text->setPosition(position);
    text->setCharacterSize(20);
    text->setScale(0.5f, 0.5f);
    text->setFillColor(color);
    text->setString(_text);
    alignCenter(*text.get());
    m_drawables.push_back(text);
}


void DebugSystem::start() {
    m_infoText.setFont(engine.fonts.get("RobotoMonoBold"));
    m_infoText.setPosition(5, 5);
    m_infoText.setCharacterSize(20);
    m_infoText.setFillColor(DEBUG_COLOR);
    m_infoText.scale(0.5f, 0.5f);
    m_pauseText = m_infoText;
    m_pauseText.setString("PAUSED (F2 = RESUME | F3 = STEP)");
    alignTopCenter(m_pauseText);

    // setup widget labels
    for (std::size_t i = 0; i < WidgetCount; ++i) {
        m_widgetLabels[i].setFont(engine.fonts.get("RobotoMonoBold"));
        m_widgetLabels[i].setString(g_widgetNames[i]);
        m_widgetLabels[i].setFillColor(g_widgetColors[i]);
        m_widgetLabels[i].setCharacterSize(20);
        m_widgetLabels[i].scale(0.5f, 0.5f);
        alignTopLeft(m_widgetLabels[i]);
    }
}

void DebugSystem::update() {
    // check for user input
    if (input.getKeyDown(Key::Escape))
        engine.stop();
    if (input.getKeyDown(Key::F1))
        m_show = !m_show;
    if (input.getKeyDown(Key::F2))
        m_paused = !m_paused;
    if (input.getKeyDown(Key::F3)) {
        if (!m_paused)
            m_paused = !m_paused;
        else
            m_advance = true;
    }
    // udate info
    updateInfo();
    // draw debug
    if (m_show) {
        // draw drawables
        for (auto& drawable : m_drawables)
            engine.window.draw(*drawable.get());
        // clear drawbales if we are going to advance frames
        if (!m_paused || m_advance)
            m_drawables.clear();
        // draw info
        engine.window.setView(engine.window.getDefaultView());
        engine.window.draw(m_infoText);
        updateWidgetMenu();
    }
    else {
        // clear drawables if we aren't drawing
        m_drawables.clear();
    }
    // draw paused label
    if (m_paused) {
        engine.window.setView(engine.window.getDefaultView());
        float width = (float)engine.window.getSize().x;
        m_pauseText.setPosition(width*0.5f, 5);
        engine.window.draw(m_pauseText);
    }
}

bool DebugSystem::proceed() {
    bool ret = !m_paused || m_advance;
    m_advance = false;
    return ret;
}

void DebugSystem::updateInfo() {
    m_info->frames++;
    m_info->cpuSum      += cpu_usage_process();
    m_info->ramSum      += ram_used_process() / 1000000;
    m_info->elapsedTime += engine.deltaTime();

    // update every second
    if (m_info->elapsedTime >= 1.0) {
        // calculate averages
        m_info->fpsDisplay = m_info->frames;
        m_info->cpuDisplay = m_info->cpuSum / m_info->frames;
        m_info->ramDisplay = m_info->ramSum / m_info->frames;
        m_info->framesDisplay = engine.frame();
        // reset
        m_info->frames = 0;
        m_info->cpuSum = 0.0;
        m_info->ramSum = 0;
        m_info->elapsedTime = 0.0f;
    }

    // form string
    m_ss.str(std::string());
    m_ss << "CLK:  " << (int)engine.time() << " s\n";
    m_ss << "FPS:  " << m_info->fpsDisplay << "\n";
    m_ss << "FRM:  " << m_info->framesDisplay << "\n";
    m_ss << "CPU:  " << std::setprecision(3) << m_info->cpuDisplay << "%\n";
    m_ss << "RAM:  " << m_info->ramDisplay << " MB\n";
    m_ss << "PIX:  " << input.getRawMousePosition().x << "," << input.getRawMousePosition().y << " px\n";
    m_ss << "X,Y:  " << std::fixed << input.getMousePosition().x << "," << input.getMousePosition().y << "\n";
    m_ss << "OBJ:  " << Object::getObjectCount() << "\n";
    m_ss << "RND:  " << Renderer::getRendererCount() << "\n";
    m_ss << "BDY:  " << RigidBody::getRigidBodyCount() << "\n";
    m_infoText.setString(m_ss.str());
}

void DebugSystem::updateWidgetMenu() {
    // setup widget labels
    float maxWidth = 0.0f;
    float maxHeight = 0.0f;
    for (std::size_t i = 0; i < WidgetCount; ++i) {
        if (m_widgetLabels[i].getGlobalBounds().width > maxWidth)
            maxWidth = m_widgetLabels[i].getGlobalBounds().width;
        if (m_widgetLabels[i].getGlobalBounds().height > maxHeight)
            maxHeight = m_widgetLabels[i].getGlobalBounds().height;
    }
    float width = (float)engine.window.getSize().x;
    for (std::size_t i = 0; i < WidgetCount; ++i) {
        m_widgetLabels[i].setPosition(width - 5 - maxWidth, 5 + (maxHeight * 1.4f) * i);
        if (inBounds(engine.input.getMousePosition(engine.window.getDefaultView()), m_widgetLabels[i].getGlobalBounds())) {
            m_widgetLabels[i].setFillColor(Whites::White);
            if (input.getMouseDown(MouseButton::Left)) {
                widgets[i] = !widgets[i];
            }
        }
        else {
            if (widgets[i])
                m_widgetLabels[i].setFillColor(g_widgetColors[i]);
            else
                m_widgetLabels[i].setFillColor(Grays::Gray50);
        }
        engine.window.draw(m_widgetLabels[i]);
    }
}


} // namespace sfvg
