#include <SFML/Window/Event.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/Object.hpp>
#include <SFVG/Graphics.hpp>
#include "RobotoMonoBold.hpp"
#include <cassert>


namespace sfvg {

//==============================================================================
// PUBLIC MEMBER VARIABLES
//==============================================================================

namespace {

bool g_engineLoaded = false;
sf::Clock g_timeClock      = sf::Clock();
sf::Clock g_deltaTimeClock = sf::Clock();
float g_timeValue           = 0.0f;
float g_deltaTimeValue      = 0.0f;

} // private namespace

//==============================================================================
// GENERAL
//==============================================================================

Engine::Engine(unsigned int width, unsigned int height, unsigned int style) :
    window(),
    m_views(1),
    m_renderQue(1),
    m_showTitleBar(true),
    m_infoText(),
    m_showInfo(false)
{
    assert(!g_engineLoaded);
    sfvgInit();
    m_font.loadFromMemory(&RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len);
    m_infoText.setFont(m_font);
    m_infoText.setPosition(5, 5);
    m_infoText.setCharacterSize(20);
    m_infoText.setFillColor(Color::Magenta);

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
    g_timeClock.restart();
    g_deltaTimeClock.restart();
    while (window.isOpen()) {
        g_deltaTimeValue = g_deltaTimeClock.restart().asSeconds();
        g_timeValue      = g_timeClock.getElapsedTime().asSeconds();
        processEvents();
        update();
        updateStats();
        render();
    }
}

void Engine::showInfo(bool show) {
    m_showInfo = show;
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

void Engine::setRoot(Ptr<Object> root) {
    m_root = root;
    m_root->setEngine(this);
    m_root->m_isRoot = true;
}

Handle<Object> Engine::getRoot() const {
    return Handle<Object>(m_root);
}

void Engine::processEvents() {
    Input::clearState();
    auto mousePosition = sf::Mouse::getPosition(window);
    Input::updatePositions(mousePosition, window.mapPixelToCoords(mousePosition, m_views[0]));
    Event event;
    while (window.pollEvent(event)) {
        Input::processEvent(event);
        switch (event.type) {
            case Event::Closed: {
                window.close();
                break;
            }
            case Event::Resized: {
                FloatRect size(0.0f, 0.0f, (float)event.size.width, (float)event.size.height);
                // View view(size);
                // m_view = view;
                break;
            }
            default:
                break;
        }
    }
}

void Engine::update() {
    if (Input::getKeyDown(Key::Escape))
        window.close();
    if (Input::getKeyDown(Key::Tilde) && Input::getKey(Key::LControl))
        m_showInfo = !m_showInfo;
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
    window.clear(m_backgroundColor);
    // iterate over views
    for (auto& view : m_views) {
        // set view
        window.setView(view);
        // iterate over layers and draw
        for (auto& layer : m_renderQue) {
            for (auto& pair : layer) {
                pair.first->draw(window, pair.second);
            }
        }
    }
    // render stats
    if (m_showInfo) {
        window.setView(window.getDefaultView());
        window.draw(m_infoText);
    }
    // display window
    window.display();
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
    ss << "CPU:  " << std::setprecision(3) << cpuDisplay << "%%\n";
    ss << "RAM:  " << ramDisplay << " MB\n";
    ss << "PIX:  " << Input::getRawMousePosition().x << "," << Input::getRawMousePosition().y << " px\n";
    ss << "X,Y:  " << std::fixed << Input::getMousePosition().x << "," << Input::getMousePosition().y << "\n";
    ss << "OBJ:  " << Object::getObjectCount() << "\n";
    ss << "CMP:  " << Component::getComponentCount() << "\n";
    m_infoText.setString(ss.str()) ;
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
