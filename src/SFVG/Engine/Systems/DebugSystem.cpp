#include <SFVG/Engine/Systems/DebugSystem.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Graphics/Alignment.hpp>
#include <SFVG/Engine/ProcessInfo.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/Components/RigidBody.hpp>

namespace sfvg {

struct DebugInfo {
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
    m_info(new DebugInfo())
{ }

void DebugSystem::show(bool _show) {
    m_show = _show;
}

bool DebugSystem::isShown() const {
    return m_show;
}

void DebugSystem::drawPoint(const Point& position, const Color& color) {
    // auto sqr = std::make_shared<SquareShape>(3);
    // sqr->setPosition(position);
    // sqr->setColor(color);
    // m_drawables.push_back(sqr);

    auto rect = std::make_shared<VertexArray>(sf::TriangleStrip);
    rect->resize(4);
    (*rect)[0].position = position + Point( 2.0f, -2.0f);
    (*rect)[1].position = position + Point(-2.0f, -2.0f);
    (*rect)[2].position = position + Point( 2.0f,  2.0f);
    (*rect)[3].position = position + Point(-2.0f,  2.0f);
    (*rect)[0].color = color;
    (*rect)[1].color = color;
    (*rect)[2].color = color;
    (*rect)[3].color = color;
    m_drawables.push_back(rect);
}

void DebugSystem::drawLine(const Point& start,
              const Point& end,
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

void DebugSystem::drawTriangle(const Point& a, const Point& b, const Point& c, const Color& color) {
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

void DebugSystem::drawRectangle(const Point& position, float width, float height,  const Color& color) {
    auto rect = std::make_shared<VertexArray>(sf::LineStrip);
    rect->resize(5);
    (*rect)[0].position = position + Point(-width, -height) * 0.5f;
    (*rect)[1].position = position + Point( width, -height) * 0.5f;
    (*rect)[2].position = position + Point( width,  height) * 0.5f;
    (*rect)[3].position = position + Point(-width,  height) * 0.5f;
    (*rect)[4].position = position + Point(-width, -height) * 0.5f;
    (*rect)[0].color = color;
    (*rect)[1].color = color;
    (*rect)[2].color = color;
    (*rect)[3].color = color;
    (*rect)[4].color = color;
    m_drawables.push_back(rect);
}

void DebugSystem::drawCircle(const Point &position, float radius, const Color& color) {
    std::size_t smoothness = 100;
    auto circle = std::make_shared<VertexArray>(sf::LineStrip);
    circle->resize(smoothness + 1);
    float angleIncrement = 2.0f * PI / smoothness;
    for (std::size_t i = 0; i < smoothness + 1; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        (*circle)[i].position = position + Point(std::cos(angle) * radius, std::sin(angle) * radius);
        (*circle)[i].color = color;
    }
    m_drawables.push_back(circle);
}

void DebugSystem::drawText(const std::string& _text,
               const Point& position,
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
    m_pauseText.setString("PAUSED");
    alignTopCenter(m_pauseText);
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
    m_infoText.setString(m_ss.str()) ;
}


} // namespace sfvg
