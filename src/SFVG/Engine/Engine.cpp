#include <SFML/Window/Event.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/ProcessInfo.hpp>
#include <SFVG/Graphics.hpp>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Fonts/EngineFonts.hpp"
#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/Components/RigidBody.hpp>

namespace sfvg {

//==============================================================================
// PUBLIC MEMBER VARIABLES
//==============================================================================

namespace {
bool g_engineLoaded = false;
} // private namespace

//==============================================================================
// GENERAL
//==============================================================================

Engine::Engine(unsigned int width, unsigned int height, unsigned int style) :
    window(),
    input(*this,"__input__"),
    physics(*this, "__physics__"),
    m_views(1),
    m_renderQue(1),
    m_infoText(),
    m_showInfo(false),
    m_timeValue(0.0f),
    m_deltaTimeValue(0.0f),
    m_paused(false)
{
    assert(!g_engineLoaded);

    // load resources
    sfvgInit();

    fonts.load("Roboto",           &Roboto_Regular_ttf,     Roboto_Regular_ttf_len);
    fonts.load("RobotoBold",       &Roboto_Bold_ttf,        Roboto_Bold_ttf_len);
    fonts.load("RobotoItalic",     &Roboto_Italic_ttf,      Roboto_Italic_ttf_len);
    fonts.load("RobotoMono",       &RobotoMono_Regular_ttf, RobotoMono_Regular_ttf_len);
    fonts.load("RobotoMonoBold",   &RobotoMono_Bold_ttf,    RobotoMono_Bold_ttf_len);
    fonts.load("RobotoMonoItalic", &RobotoMono_Italic_ttf,  RobotoMono_Italic_ttf_len);

    m_infoText.setFont(fonts.get("RobotoMonoBold"));
    m_infoText.setPosition(5, 5);
    m_infoText.setCharacterSize(20);
    m_infoText.setFillColor(Color::Magenta);
    m_infoText.scale(0.5f, 0.5f);
    m_pauseText = m_infoText;
    m_pauseText.setString("PAUSED");
    alignTopCenter(m_pauseText);

    // create Window and set settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(width, height), "", style, settings);
    window.setFramerateLimit(60);
    window.requestFocus();
    // set Window view
    m_views[0] = window.getDefaultView();
    m_views[0].setCenter(width * 0.5f, height * 0.5f);
    // loaded
    g_engineLoaded = true;
}

Engine::~Engine() {
    g_engineLoaded = false;
    sfvgFree();
}

void Engine::run() {
    assert(m_root != nullptr);
    m_clock.restart();
    while (window.isOpen()) {
        // input update
        input.update();
        processEvents();
        // internal input checks
        if (input.getKeyDown(Key::Escape))
            window.close();
        if (input.getKeyDown(Key::F1))
            m_showInfo = !m_showInfo;
        if (input.getKeyDown(Key::F2))
            m_paused = !m_paused;
        if (input.getKeyDown(Key::F3))
            m_advance = true;
        // update delta time
        m_deltaTimeValue = m_clock.restart().asSeconds();
        // update stats
        updateStats();
        if (!m_paused || m_advance) {
            // reset advance flag
            m_advance = false;
            // update continous time
            m_timeValue += m_deltaTimeValue;
            // physics update
            physics.update();
            m_root->onPhysics();
            // update all objects
            m_root->updateAll();
        }
        // clear window
        window.clear(m_backgroundColor);
        // render
        render();
        // display window
        window.display();
    }
}

void Engine::showInfo(bool show) {
    m_showInfo = show;
}

float Engine::time() {
    return m_timeValue;
}

float Engine::deltaTime() {
    return m_deltaTimeValue;
}

//==============================================================================
// RENDERING
//==============================================================================


View& Engine::getView(std::size_t index) {
    return m_views[index];
}

void Engine::addView() {
    View view = window.getDefaultView();
    m_views.push_back(view);
}


sf::Vector2f Engine::getWorldSize() const {
    return window.mapPixelToCoords(sf::Vector2i(window.getSize()), m_views[0]);
}

void Engine::setBackgroundColor(const Color &color) {
    m_backgroundColor = color;
}

void Engine::setLayerCount(std::size_t count) {
    assert(count > 0);
    m_renderQue.resize(count);
}

std::size_t Engine::getLayerCount() const {
    return m_renderQue.size();
}

//==============================================================================
// ROOT OBJECT
//==============================================================================

void Engine::setRoot(Ptr<GameObject> root) {
    m_root = root;
    m_root->m_isRoot = true;
}

Handle<GameObject> Engine::getRoot() const {
    return Handle<GameObject>(m_root);
}

void Engine::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        input.processEvent(event);
        switch (event.type) {
            case Event::Closed: {
                window.close();
                break;
            }
            case Event::Resized: {
                FloatRect size(0.0f, 0.0f, (float)event.size.width, (float)event.size.height);
                break;
            }
            default:
                break;
        }
    }
}

void Engine::render() {
    // clear each layer in the RenderQue and reserve capacity for max number of Objects
    for (auto& layer : m_renderQue) {
        layer.clear();
        layer.reserve(Renderer::getRendererCount());
    }
    // que Objects for rendering
    m_root->onRender(m_renderQue);
    // iterate over views
    for (auto& view : m_views) {
        // set view
        window.setView(view);
        // iterate over layers and draw
        for (auto& layer : m_renderQue) {
            for (auto& renderer : layer) {
                renderer->render(window);
            }
        }
    }
    // render stats
    if (m_showInfo) {
        window.setView(window.getDefaultView());
        window.draw(m_infoText);
    }
    if (m_paused) {
        window.setView(window.getDefaultView());
        float width = (float)window.getSize().x;
        m_pauseText.setPosition(width*0.5f, 5);
        window.draw(m_pauseText);
    }
}

void Engine::updateStats() {
    static float elapsedTime = 0.0f;
    static std::ostringstream ss;

    static std::size_t frames = 0;
    static std::size_t framesDisplay = 0;

    static double cpuSum = 0.0;
    static double cpuDisplay = 0.0;

    static std::size_t ramSum = 0;
    static std::size_t ramDisplay = 0;

    frames++;
    cpuSum      += cpu_usage_process();
    ramSum      += ram_used_process() / 1000000;
    elapsedTime += deltaTime();

    // update every second
    if (elapsedTime >= 1.0) {
        // calculate averages
        framesDisplay = frames;
        cpuDisplay = cpuSum / frames;
        ramDisplay = ramSum / frames;
        // reset
        frames = 0;
        cpuSum = 0.0;
        ramSum = 0;
        elapsedTime = 0.0f;
    }

    // form string
    ss.str(std::string());
    ss << "CLK:  " << (int)m_timeValue << " s\n";
    ss << "FPS:  " << framesDisplay << "\n";
    ss << "CPU:  " << std::setprecision(3) << cpuDisplay << "%\n";
    ss << "RAM:  " << ramDisplay << " MB\n";
    ss << "PIX:  " << input.getRawMousePosition().x << "," << input.getRawMousePosition().y << " px\n";
    ss << "X,Y:  " << std::fixed << input.getMousePosition().x << "," << input.getMousePosition().y << "\n";
    ss << "OBJ:  " << Object::getObjectCount() << "\n";
    ss << "RND:  " << Renderer::getRendererCount() << "\n";
    ss << "BDY:  " << RigidBody::getRigidBodyCount() << "\n";
    m_infoText.setString(ss.str()) ;
}

// How does Unity's GetComponent() work?
// https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work

} // namespace sfvg
