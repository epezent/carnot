#include <SFML/Window/Event.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Graphics.hpp>
#include <cassert>
#include "Fonts/EngineFonts.hpp"
#include <SFVG/Engine/Components/Renderer.hpp>

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
    debug(*this, "__debug__"),
    m_running(false),
    m_views(1),
    m_renderQue(1),
    m_timeValue(0.0f),
    m_deltaTimeValue(0.0f),
    m_frame(0)
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
    assert(!m_running);
    m_running = true;
    // start systems
    debug.start();
    m_clock.restart();
    // run game
    while (window.isOpen() && m_running) {
        // input update
        input.update();
        processEvents();
        // update delta time
        m_deltaTimeValue = m_clock.restart().asSeconds();
        if (debug.proceed()) {
            // update continous time
            m_timeValue += m_deltaTimeValue;
            // physics update
            physics.update();
            m_root->onPhysics();
            // update all objects
            m_root->updateAll();
            m_frame++;
        }
        // clear window
        window.clear(m_backgroundColor);
        // render
        render();
        // update debug
        debug.update();
        // display window
        window.display();
    }
}

void Engine::stop() {
    m_running = false;
}

float Engine::time() const {
    return m_timeValue;
}

float Engine::deltaTime() const {
    return m_deltaTimeValue;
}

std::size_t Engine::frame() const {
    return m_frame;
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
}

} // namespace sfvg
