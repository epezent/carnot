#include <SFML/Window/Event.hpp>
#include <Engine/Engine.hpp>
#include <Engine/GameObject.hpp>
#include <cassert>
#include "Fonts/EngineFonts.hpp"
#include <Graphics/Components/Renderer.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include <Engine/IconsFontAwesome5.hpp>
#include <Engine/IconsFontAwesome5Brands.hpp>
#include <atomic>
#include <thread>
#include "SPSCQueue.hpp"

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#include <ShellScalingAPI.h>
#endif

using namespace rigtorp;

namespace carnot {

//==============================================================================
// GLOBAL VARIABLES / PRIVATE FUNCTIONS
//==============================================================================
namespace {   

std::atomic<bool> g_initialized = false;
std::atomic<bool> g_running     = false; 
float             g_time        = 0.0f;
float             g_delta       = 0.0f;
std::size_t       g_frame       = 0;
float             g_dpiFactor   = 1.0f;
std::vector<View> g_views       = std::vector<View>(1);
RenderQue         g_renderQue   = RenderQue(1);
Color             g_bgColor     = Color();
Clock             g_clock       = Clock();
Ptr<GameObject>   g_root;
SPSCQueue<Event>  g_eventQue(256);          

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

void loadResources() {

    Engine::window = std::make_shared<RenderWindow>();

    Engine::fonts.load(ID::makeId("Roboto"),           &Roboto_Regular_ttf,      Roboto_Regular_ttf_len);
    Engine::fonts.load(ID::makeId("RobotoBold"),       &Roboto_Bold_ttf,         Roboto_Bold_ttf_len);
    Engine::fonts.load(ID::makeId("RobotoItalic"),     &Roboto_Italic_ttf,       Roboto_Italic_ttf_len);
    Engine::fonts.load(ID::makeId("RobotoMono"),       &RobotoMono_Regular_ttf,  RobotoMono_Regular_ttf_len);
    Engine::fonts.load(ID::makeId("RobotoMonoBold"),   &RobotoMono_Bold_ttf,     RobotoMono_Bold_ttf_len);
    Engine::fonts.load(ID::makeId("RobotoMonoItalic"), &RobotoMono_Italic_ttf,   RobotoMono_Italic_ttf_len);
    Engine::fonts.load(ID::makeId("FontAwesome5"),     &fa_solid_900_ttf,        fa_solid_900_ttf_len);

    /// Built in textures
    Engine::textures.load(ID::makeId("__texture_white"), white_pixel, white_pixel_len);
}

void freeResources() {
    // free resources
    Engine::textures.unloadAll();
    Engine::fonts.unloadAll();
    // Engine::sounds.unloadAll();
    Engine::shaders.unloadAll();
    // free window
    Engine::window.reset();
}

void determineDpi() {
#ifdef _WIN32   
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    const POINT ptZero = { 0, 0 };
    auto monitor = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
    UINT dpiX, dpiY;
    auto result  = GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    g_dpiFactor = (float)dpiX / (float)USER_DEFAULT_SCREEN_DPI;
#else
    g_dpiFactor = 1.0f;
#endif
}

//==============================================================================
// WINDOWS
//==============================================================================

#ifdef _WIN32

LONG_PTR g_originalSfmlCallback = 0x0;

LRESULT CALLBACK fileDropCallback(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_DROPFILES)
    {
        HDROP hdrop = reinterpret_cast<HDROP>(wParam);
        POINT p;
        p.x = 0;
        p.y = 0;
        DragQueryPoint(hdrop, &p);
        const UINT filescount = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
        for(UINT i = 0; i < filescount; ++i)
        {
            const UINT bufsize = DragQueryFile(hdrop, i, NULL, 0);
            std::string str;
            str.resize(bufsize);
            if(DragQueryFile(hdrop, i, (&str[0]), bufsize + 1))            
                Engine::onFileDrop.emit(str, Vector2u(p.x,p.y));
        }
        DragFinish(hdrop);
    } //if WM_DROPFILES
    return CallWindowProcW(reinterpret_cast<WNDPROC>(g_originalSfmlCallback), handle, message, wParam, lParam);
}

void enableFileDrop(sf::WindowHandle handle) {
    DragAcceptFiles(handle, TRUE);
    g_originalSfmlCallback = SetWindowLongPtrW(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(fileDropCallback));
}

void setUserIcon(sf::WindowHandle handle) {
    HANDLE hIcon = LoadIconW(GetModuleHandleW(NULL), L"CARNOT_ICON");
    if (hIcon) {
        ::SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        ::SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
}

#else

void enableFileDrop(sf::WindowHandle handle) {}
void setUserIcon(sf::WindowHandle handle) {}

#endif



} // private namespace

//==============================================================================
// USER API
//==============================================================================

Ptr<RenderWindow> Engine::window;

ResourceManager<Texture>     Engine::textures = ResourceManager<Texture>();
ResourceManager<Font>        Engine::fonts    = ResourceManager<Font>();
// ResourceManager<SoundBuffer> Engine::sounds   = ResourceManager<SoundBuffer>();
ResourceManager<Shader>      Engine::shaders  = ResourceManager<Shader>();

Signal<void(const std::string&, const Vector2u&)> Engine::onFileDrop;

void Engine::init(const std::string& title) {
    init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, WindowStyle::Fullscreen, title);
}

void Engine::init(unsigned int width, unsigned int height, const std::string& title) {
    init(width, height, WindowStyle::Default, title);
}

void Engine::init(unsigned int width, unsigned int height, unsigned int style, const std::string& title) {
    // asserts
    assert(!g_initialized);
    assert(!g_running);    
    // load Engine resources
    loadResources();
    // determine DPI
    determineDpi();
    // create context settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    // create VideoMode
    sf::VideoMode vMode = sf::VideoMode((unsigned int)(width * g_dpiFactor), (unsigned int)(height * g_dpiFactor));
    if (style & WindowStyle::Fullscreen)
        vMode = sf::VideoMode::getDesktopMode();
    // create window context
    window->create(vMode, title, style, settings);
    // set frame limit
    window->setFramerateLimit(60);
    // set user icon from RC file
    setUserIcon(window->getSystemHandle());
    // enable file drop
    enableFileDrop(window->getSystemHandle());
    // set default Window view
    g_views[0] = window->getDefaultView();
    g_views[0].setCenter(g_views[0].getCenter() / g_dpiFactor);
    g_views[0].setSize(g_views[0].getSize() / g_dpiFactor);
    // initialize imgui
    ImGui::SFML::Init(*window, true);
    ImGui::GetIO().DisplayFramebufferScale = ImVec2(g_dpiFactor,g_dpiFactor);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->Clear();
    unsigned char* fontCopy1 = new unsigned char[RobotoMono_Bold_ttf_len];
    std::memcpy(fontCopy1, &RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len);
    io.Fonts->AddFontFromMemoryTTF(fontCopy1, RobotoMono_Bold_ttf_len, 15.0f * g_dpiFactor);

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 14.0f;
    icons_config.GlyphOffset = ImVec2(0,1);

    // merge in icons from font awesome 5
    static const ImWchar fa_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    unsigned char* fontCopy2 = new unsigned char[fa_solid_900_ttf_len];
    std::memcpy(fontCopy2, &fa_solid_900_ttf, fa_solid_900_ttf_len);
    io.Fonts->AddFontFromMemoryTTF(fontCopy2, fa_solid_900_ttf_len, 14.0f * g_dpiFactor, &icons_config, fa_ranges );

    // merge in icons from font awesome 5 brands
    static const ImWchar fab_ranges[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };
    unsigned char* fontCopy3 = new unsigned char[fa_brands_400_ttf_len];
    std::memcpy(fontCopy3, &fa_brands_400_ttf, fa_brands_400_ttf_len);
    io.Fonts->AddFontFromMemoryTTF(fontCopy3, fa_brands_400_ttf_len, 14 * g_dpiFactor, &icons_config, fab_ranges);
    
    io.FontGlobalScale = 1.0f / g_dpiFactor;  
    io.IniFilename = 0;
    ImGui::SFML::UpdateFontTexture();

    // stype ImGui
    ImGuiStyle * imStyle = &ImGui::GetStyle();

    // Main
    imStyle->WindowPadding    = ImVec2(8,8);
    imStyle->FramePadding     = ImVec2(2,2);
    imStyle->ItemSpacing      = ImVec2(4,4);
    imStyle->ItemInnerSpacing = ImVec2(4,4);
    imStyle->IndentSpacing    = 20.0f;
    imStyle->ScrollbarSize    = 15.0f;
    imStyle->GrabMinSize = 5.0f;

    // Rounding
    imStyle->WindowRounding = 2.0f;
    imStyle->ChildRounding = 2.0f;
    imStyle->FrameRounding = 2.0f;
    imStyle->PopupRounding = 2.0f;
    imStyle->ScrollbarRounding = 10.0f;
    imStyle->GrabRounding = 2.0f;
    imStyle->TabRounding = 2.0f;

    // Alignment
    imStyle->WindowMenuButtonPosition = ImGuiDir_Right;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.05f, 0.07f, 0.95f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.07f, 0.07f, 0.09f, 0.95f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.07f, 0.07f, 0.09f, 0.95f);
    colors[ImGuiCol_Border]                 = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);   

    // start systems
    Input::detail::init();
    Debug::detail::init(); 
    Physics::detail::init();

    // loaded
    g_initialized = true;
    window->requestFocus();
}

void Engine::eventThread() {
    Event event;
    while (g_running) {
        while (window->pollEvent(event))
            g_eventQue.try_push(event);
        sleep(milliseconds(2));
    }
}

Vector2u Engine::getWindowSize() {
#ifdef _WIN32
    RECT rect;
    GetClientRect(window->getSystemHandle(), &rect);  
    Vector2u out;
    out.x = rect.right - rect.left;
    out.y = rect.bottom - rect.top;
    return out;
#else
    return window->getSize();
#endif
}

void Engine::renderThread() {
    window->setActive(true);
    while (window->isOpen() && g_running) {
        // input update
        Input::detail::update();
        processEvents();
        // update delta time
        auto deltaTime = g_clock.restart();
        g_delta = deltaTime.asSeconds();
        // get realtime window size
        auto windowSize = getWindowSize();
        // print(windowSize);
        g_views[0].setSize(windowSize.x / g_dpiFactor, windowSize.y / g_dpiFactor);
        // upate imgui
        // ImGui::SFML::Update(*window, *window, windowSize, deltaTime);
        ImGui::SFML::Update(*window, deltaTime);

        // game update
        if (Debug::detail::proceed()) {
            // update continous time
            g_time += g_delta;
            // physics update
            Physics::detail::update();
            g_root->onPhysics();
            // update all objects
            g_root->updateAll();
            // late update all object
            g_root->lateUpdateAll();
            // increment frame
            g_frame++;
        }
        // clear window
        window->clear(g_bgColor);
        // render
        render();
        // update debug
        if (Debug::isShown())
            g_root->onGizmo();
        Debug::detail::update();
        window->setView(window->getDefaultView());
        // draw imgui
        ImGui::SFML::Render(*window);
        // display window
        window->display();
    }
    g_running = false;
}

void Engine::run() {
    assert(g_root != nullptr);
    assert(!g_running);
    g_running = true;
    g_clock.restart();
    // run game
    window->setActive(false);
    std::thread rt(renderThread);
    eventThread();
    rt.join();
    // free resources
    freeResources();
    // delete root GameObject
    g_root.reset();
    // shutdown systems
    ImGui::SFML::Shutdown();
    Physics::detail::shutdown();
    Debug::detail::shutdown();
    g_initialized = false;   
}

void Engine::stop() {
    assert(g_running);
    g_running = false;
}

float Engine::time() {
    return g_time;
}

float Engine::deltaTime() {
    return g_delta;
}

std::size_t Engine::frame() {
    return g_frame;
}

//==============================================================================
// RENDERING
//==============================================================================


View& Engine::getView(std::size_t index) {
    return g_views[index];
}

void Engine::addView() {
    View view = window->getDefaultView();
    g_views.push_back(view);
}


Vector2f Engine::getWorldSize() {
    return window->mapPixelToCoords(sf::Vector2i(window->getSize()), g_views[0]);
}

void Engine::setBackgroundColor(const Color &color) {
    g_bgColor = color;
}

void Engine::setLayerCount(std::size_t count) {
    assert(count > 0);
    g_renderQue.resize(count);
}

std::size_t Engine::getLayerCount() {
    return g_renderQue.size();
}

float Engine::getDpiFactor() {
    return g_dpiFactor;
}

void Engine::setRoot(Ptr<GameObject> root) {
    g_root = root;
    g_root->m_isRoot = true;
}

Handle<GameObject> Engine::getRoot() {
    return Handle<GameObject>(g_root);
}

void Engine::render() {
    // clear each layer in the RenderQue and reserve capacity for max number of Objects
    for (auto& layer : g_renderQue) {
        layer.clear();
        layer.reserve(Renderer::getRendererCount());
    }
    // que Objects for rendering
    g_root->onRender(g_renderQue);
    // iterate over views
    for (auto& view : g_views) {
        // set view
        window->setView(view);
        // iterate over layers and draw
        for (auto& layer : g_renderQue) {
            for (auto& renderer : layer) {
                renderer->render(*window);
            }
        }
    }
}

void Engine::processEvents() {
    Event event;
    while (g_eventQue.front()) {
        event = *g_eventQue.front();
        Input::detail::processEvent(event);
        ImGui::SFML::ProcessEvent(event);
        switch (event.type) {
            case Event::Closed: {
                window->close();
                break;
            }
            case Event::Resized: {     
                g_views[0].setSize((float)event.size.width / g_dpiFactor, (float)event.size.height / g_dpiFactor);
                break;
            }
            default:
                break;
        }
        g_eventQue.pop();
    }
}

} // namespace carnot
