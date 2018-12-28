#include <SFML/Window/Event.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Graphics.hpp>
#include "RobotoMonoBold.hpp"
#include <cassert>

#define REF_WIDTH  3840.0f

namespace sfvg {

//==============================================================================
// PUBLIC MEMBER VARIABLES
//==============================================================================

namespace {

static bool g_engineLoaded = false;
sf::Clock g_timeClock      = sf::Clock();
sf::Clock g_deltaTimeClock = sf::Clock();
float g_timeValue           = 0.0f;
float g_deltaTimeValue      = 0.0f;

} // private namespace

Engine::Engine() :
    m_renderQue(1),
    m_window(),
    m_windowSize(500,500),
    m_appName("SFVG Application"),
    m_debugText(),
    m_showDebug(false),
    m_scaleFactor((float)sf::VideoMode::getDesktopMode().width / REF_WIDTH)
{
    sfvgInit();
    assert(!g_engineLoaded);
    m_font.loadFromMemory(&RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len);
    m_debugText.setFont(m_font);
    m_debugText.setPosition(5, 5);
    m_debugText.setCharacterSize(20);
    m_debugText.setFillColor(Color::Magenta);
    g_engineLoaded = true;
}

Engine::~Engine() {
    sfvgFree();
}

void Engine::setRoot(Ptr<Object> root) {
    m_root = root;
    m_root->setEngine(this);
    m_root->m_isRoot = true;
}


Handle<Object> Engine::getRoot() const {
    return Handle<Object>(m_root);
}

void Engine::setWindowSize(const Vector2f& windowSize) {
    m_windowSize = windowSize;
}

void Engine::setBackgroundColor(const Color &color) {
    m_backgroundColor = color;
}

void Engine::setScaleFactor(float scaleFactor) {
    m_scaleFactor = scaleFactor;
}

sf::Vector2f Engine::getWindowSize() const {
    return Vector2f(m_window.getSize());
}

sf::Vector2f Engine::getGlobalSize() const {
    return m_window.mapPixelToCoords(sf::Vector2i(m_window.getSize()), m_view);
}

void Engine::setLayerCount(std::size_t count) {
    assert(count > 0);
    m_renderQue.resize(count);
}

std::size_t Engine::getLayerCount() const {
    return m_renderQue.size();
}

void Engine::run() {
    assert(m_root != nullptr);
    initWindow();
    g_timeClock.restart();
    g_deltaTimeClock.restart();
    while (m_window.isOpen()) {
        g_deltaTimeValue = g_deltaTimeClock.restart().asSeconds();
        g_timeValue      = g_timeClock.getElapsedTime().asSeconds();
        processEvents();
        update();
        updateStats();
        render();
    }
}

void Engine::initWindow() {
    // create Window and set settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    m_window.create(sf::VideoMode(m_windowSize.x * m_scaleFactor, m_windowSize.y * m_scaleFactor), m_appName, sf::Style::Default, settings);
    m_window.setFramerateLimit(60);
    m_window.requestFocus();

    // set Window view
    m_view = m_window.getDefaultView();
    m_view.setSize(m_windowSize.x, m_windowSize.y);
    m_view.setCenter(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f);
}

void Engine::processEvents() {
    Input::clearState();
    auto mousePosition = sf::Mouse::getPosition(m_window);
    Input::updatePositions(mousePosition, m_window.mapPixelToCoords(mousePosition, m_view));
    Event event;
    while (m_window.pollEvent(event)) {
        Input::processEvent(event);
        switch (event.type) {
            case Event::Closed: {
                m_window.close();
                break;
            }
            case Event::Resized: {
                FloatRect size(0, 0, event.size.width / m_scaleFactor, event.size.height / m_scaleFactor);
                View view(size);
                m_view = view;
                break;
            }
            default:
                break;
        }
    }
}

void Engine::update() {
    if (Input::getKeyDown(Key::Escape))
        m_window.close();
    if (Input::getKeyDown(Key::Tilde) && Input::getKey(Key::LControl))
        m_showDebug = !m_showDebug;

    m_root->updateAll();
}

void Engine::render() {
    // clear each layer in the RenderQue and reserve capacity for max number of Objects
    for (auto& layer : m_renderQue) {
        layer.clear();
        layer.reserve(Object::getObjectCount());
    }
    // que Objects for rendering
    m_root->queRender(m_renderQue, sf::RenderStates::Default);
    // clear window
    m_window.clear(m_backgroundColor);
    // set view for resolution independance
    m_window.setView(m_view);
    // iterate over layers and draw
    for (auto& layer : m_renderQue) {
        for (auto& pair : layer) {
            pair.first->draw(m_window, pair.second);
        }
    }

    // m_root->draw(m_window, sf::RenderStates::Default);

    // render stats
    if (m_showDebug) {
        m_window.draw(m_debugText);
    }
    // display window
    m_window.display();
}

void Engine::showDebugInfo(bool show) {
    m_showDebug = show;
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
    cpuSum += cpu_usage_process();
    ramSum += ram_used_process() / 1000000;
    elapsedTime += g_deltaTimeValue;

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
    ss << "CLK:  " << (int)g_timeValue << " s\n";
    ss << "FPS:  " << framesDisplay << "\n";
    ss << "CPU:  " << std::setprecision(3) << cpuDisplay << "\%\n";
    ss << "RAM:  " << ramDisplay << " MB\n";
    ss << "PIX:  " << Input::getRawMousePosition().x << "," << Input::getRawMousePosition().y << " px\n";
    ss << "X,Y:  " << std::fixed << Input::getMousePosition().x << "," << Input::getMousePosition().y << "\n";
    ss << "OBJ:  " << Object::getObjectCount() << "\n";
    m_debugText.setString(ss.str()) ;
}

void Engine::setWindowTitle(const std::string& name) {
    if (m_window.isOpen())
        m_window.setTitle(name);
    m_appName = name;
}

void Engine::showMouseCursor(bool show) {
   m_window.setMouseCursorVisible(show);
}

float Engine::time() {
    return g_timeValue;
}

float Engine::deltaTime() {
    return g_deltaTimeValue;
}

// How does Unity's GetComponent() work?
// https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work

} // namespace sfvg
