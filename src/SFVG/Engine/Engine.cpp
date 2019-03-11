#include <SFML/Window/Event.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <cassert>
#include "Fonts/EngineFonts.hpp"
#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/ImGui/imgui.h>
#include <SFVG/Engine/ImGui/imgui-SFML.h>
#include <SFVG/Engine/FontAwesome5.hpp>
#include <windows.h>
#include <winuser.h>
#include <ShellScalingAPI.h>

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

Engine::Engine() :
    Engine(sf::VideoMode::getDesktopMode().width,
           sf::VideoMode::getDesktopMode().height,
           WindowStyle::Fullscreen)
{

}

Engine::Engine(unsigned int width, unsigned int height, unsigned int style) :
    window(),
    input(*this,"__input__"),
    physics(*this, "__physics__"),
    debug(*this, "__debug__"),
    m_running(false),
    m_views(1),
    m_renderQue(1),
    m_dpiFactor(1.0f),
    m_timeValue(0.0f),
    m_deltaTimeValue(0.0f),
    m_frame(0)
{
    assert(!g_engineLoaded);
    // load resources
    loadBuiltInResources();

#ifdef _WIN32   
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    const POINT ptZero = { 0, 0 };
    auto monitor = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
    UINT dpiX, dpiY;
    auto result  = GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    m_dpiFactor = (float)dpiX / (float)USER_DEFAULT_SCREEN_DPI;
#else
    m_dpiFactor = 1.0f;
#endif


    // create Window and set settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode((unsigned int)(width * m_dpiFactor), (unsigned int)(height * m_dpiFactor)), "", style, settings);
    window.setFramerateLimit(60);

    // set Window view
    m_views[0] = window.getDefaultView();
    m_views[0].setCenter(m_views[0].getCenter() / m_dpiFactor);
    m_views[0].setSize(m_views[0].getSize() / m_dpiFactor);

    // initialize imgui
    ImGui::SFML::Init(window, m_dpiFactor);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    unsigned char* fontCopy1 = new unsigned char[RobotoMono_Bold_ttf_len];
    std::memcpy(fontCopy1, &RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len);
    io.Fonts->AddFontFromMemoryTTF(fontCopy1, RobotoMono_Bold_ttf_len, 15.0f * m_dpiFactor);

    // merge in icons from font awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 14.0f;
    unsigned char* fontCopy2 = new unsigned char[fa_solid_900_ttf_len];
    std::memcpy(fontCopy2, &fa_solid_900_ttf, fa_solid_900_ttf_len);
    io.Fonts->AddFontFromMemoryTTF(fontCopy2, fa_solid_900_ttf_len, 10.0f * m_dpiFactor, &icons_config, icons_ranges );
    io.FontGlobalScale = 1.0f / m_dpiFactor;  
    ImGui::SFML::UpdateFontTexture();
    // loaded
    g_engineLoaded = true;
    window.requestFocus();
}

Engine::~Engine() {
    g_engineLoaded = false;
    sfvgFree();
    ImGui::SFML::Shutdown();
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
        auto deltaTimeTime = m_clock.restart();
        m_deltaTimeValue = deltaTimeTime.asSeconds();
        // upate imgui
        ImGui::SFML::Update(window, deltaTimeTime);
        // game update
        if (debug.proceed()) {
            // update continous time
            m_timeValue += m_deltaTimeValue;
            // physics update
            physics.update();
            m_root->onPhysics();
            // update all objects
            m_root->updateAll();
            // late update all object
            m_root->lateUpdateAll();
            // increment frame
            m_frame++;
        }

        // clear window
        window.clear(m_backgroundColor);
        // render
        render();
        // update debug
        if (debug.isShown())
            m_root->onDebugRender();
        debug.update();
        window.setView(window.getDefaultView());
        // draw imgui
        ImGui::SFML::Render(window);
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

float Engine::getDpiFactor() const {
    return m_dpiFactor;
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


//==============================================================================
// PRIVATE
//==============================================================================



void Engine::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        input.processEvent(event);
        ImGui::SFML::ProcessEvent(event);
        switch (event.type) {
            case Event::Closed: {
                window.close();
                break;
            }
            case Event::Resized: {
                // FloatRect size(0.0f, 0.0f, (float)event.size.width, (float)event.size.height);
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

//==============================================================================
// RESOURCES
//==============================================================================

namespace {
    const std::string g_solidShaderGlsl = \
        "uniform vec4 u_color;" \
        "uniform sampler2D u_texture;" \
        "void main() {" \
        "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
        "    gl_FragColor = u_color * pixel;" \
        "}";

    const std::string g_gradientShaderGlsl = \
        "uniform vec4 u_color1;" \
        "uniform vec4 u_color2;" \
        "uniform float u_angle;" \
        "uniform sampler2D u_texture;" \
        "void main() {" \
        "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
        "    vec2 center = gl_TexCoord[0].xy - 0.5;" \
        "    float radians = -0.0174532925199433 * u_angle;" \
        "    float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;" \
        "    gl_FragColor = mix(u_color1, u_color2, t) * pixel;" \
        "}";

        unsigned char white_pixel[] = {
          0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
          0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
          0x01, 0x03, 0x00, 0x00, 0x00, 0x25, 0xdb, 0x56, 0xca, 0x00, 0x00, 0x00,
          0x03, 0x50, 0x4c, 0x54, 0x45, 0xff, 0xff, 0xff, 0xa7, 0xc4, 0x1b, 0xc8,
          0x00, 0x00, 0x00, 0x0a, 0x49, 0x44, 0x41, 0x54, 0x08, 0xd7, 0x63, 0x60,
          0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0xe2, 0x21, 0xbc, 0x33, 0x00, 0x00,
          0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
        };
        unsigned int white_pixel_len = 82;

} // namespace name

void Engine::loadBuiltInResources() {
    // fonts
    fonts.load("Roboto",           &Roboto_Regular_ttf,      Roboto_Regular_ttf_len);
    fonts.load("RobotoBold",       &Roboto_Bold_ttf,         Roboto_Bold_ttf_len);
    fonts.load("RobotoItalic",     &Roboto_Italic_ttf,       Roboto_Italic_ttf_len);
    fonts.load("RobotoMono",       &RobotoMono_Regular_ttf,  RobotoMono_Regular_ttf_len);
    fonts.load("RobotoMonoBold",   &RobotoMono_Bold_ttf,     RobotoMono_Bold_ttf_len);
    fonts.load("RobotoMonoItalic", &RobotoMono_Italic_ttf,   RobotoMono_Italic_ttf_len);
    fonts.load("FontAwesome5",     &fa_solid_900_ttf,        fa_solid_900_ttf_len);
    // shaders
    shaders.load("Solid"   ,g_solidShaderGlsl,    sf::Shader::Fragment);
    shaders.load("Gradient",g_gradientShaderGlsl, sf::Shader::Fragment);
    // textures
    textures.load("__white__", white_pixel, white_pixel_len);


    sfvgInit();

}


} // namespace sfvg
