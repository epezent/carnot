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

void DebugSystem::drawLine(const Vector2f& start,
              const Vector2f& end,
              const Color& color)
{
    auto ptr = std::make_shared<VertexArray>(sf::Lines);
    ptr->resize(2);
    ptr->operator[](0).position = start;
    ptr->operator[](0).color = color;
    ptr->operator[](1).position = end;
    ptr->operator[](1).color = color;
    m_drawables.push_back(ptr);
}

void DebugSystem::drawText(const std::string& text,
               const Vector2f& position,
               const Color& color)
{
    auto ptr = std::make_shared<Text>();
    ptr->setFont(engine.fonts.get("RobotoMonoBold"));
    ptr->setPosition(position);
    ptr->setCharacterSize(20);
    ptr->setScale(0.5f, 0.5f);
    ptr->setFillColor(color);
    ptr->setString(text);
    alignCenter(*ptr.get());
    m_drawables.push_back(ptr);
}


void DebugSystem::start() {
    m_infoText.setFont(engine.fonts.get("RobotoMonoBold"));
    m_infoText.setPosition(5, 5);
    m_infoText.setCharacterSize(20);
    m_infoText.setFillColor(Color::Magenta);
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
