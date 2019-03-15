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
#include <SFVG/Engine/ImGui/imgui.h>
#include <SFVG/Engine/ImGui/imgui-SFML.h>
#include <SFVG/Engine/FontAwesome5.hpp>
#include <sstream>
#include <array>
#include <map>

#define DEBUG_COLOR               Greens::Chartreuse
#define DEBUG_XAXIS_COLOR         Reds::Red
#define DEBUG_YAXIS_COLOR         Greens::Chartreuse
#define DEBUG_TRANSFORM_COLOR     Whites::White
#define DEBUG_LOCAL_BOUNDS_COLOR  Blues::Blue
#define DEBUG_WORLD_BOUNDS_COLOR  Cyans::Cyan
#define DEBUG_WIREFRAME_COLOR     Yellows::Yellow
#define DEBUG_PHYSICS_COG_COLOR   Purples::Magenta
#define DEBUG_PHYSICS_SHAPE_COLOR Purples::Magenta

namespace sfvg {
    
//==============================================================================
// GLOBALS
//==============================================================================

namespace {

    std::vector<Id>           g_gizmoIds;
    std::map<Id, std::string> g_gizmoNames;
    std::map<Id, Color>       g_gizmoColors;
    std::map<Id, bool>        g_gizmoActives;

    bool g_show;
    bool g_paused;
    bool g_advance;
    bool g_gizmoFrameActive;
    bool g_panTool;

    std::vector<Vertex> g_triangles;
    std::vector<Vertex> g_lines;
    std::vector<Text>   g_text;

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

    DebugInfo g_info;

} // private namespace

//==============================================================================
// USER API
//==============================================================================

namespace Debug {

void show(bool _show) {
    g_show = _show;
}

bool isShown() {
    return g_show;
}

void addGizmo(const std::string& name, const Color& color) {
    Name intern = "__" + name + "__";
    Id id = ID::makeId(intern);
    g_gizmoIds.push_back(id);
    g_gizmoNames[id] = name;
    g_gizmoColors[id] = color;
    g_gizmoActives[id] = false;
}

bool gizmoActive(Id id) {
    return g_gizmoActives[id];
}

void setGizmoActive(Id id, bool active) {
    g_gizmoActives[id] = active;
}

Id gizmoId(const std::string& name) {
    Name intern = "__" + name + "__";
    return ID::getId(intern);
}

const Color& gizmoColor(Id id) {
    return g_gizmoColors[id];
}

void drawPoint(const Vector2f& position, const Color& color) {
    g_triangles.emplace_back(position + Vector2f( 2.0f, -2.0f), color);
    g_triangles.emplace_back(position + Vector2f(-2.0f, -2.0f), color);
    g_triangles.emplace_back(position + Vector2f( 2.0f,  2.0f), color);
    g_triangles.emplace_back(position + Vector2f(-2.0f, -2.0f), color);
    g_triangles.emplace_back(position + Vector2f( 2.0f,  2.0f), color);
    g_triangles.emplace_back(position + Vector2f(-2.0f,  2.0f), color);
}

void drawLine(const Vector2f& start, const Vector2f& end, const Color& color) {
    g_lines.emplace_back(start, color);
    g_lines.emplace_back(end, color);
}

void drawLines(const std::vector<Vector2f> &points, const Color& color) {
    if (isEven((int)points.size())) {
        for (std::size_t i = 0; i < points.size(); ++i)
            g_lines.emplace_back(points[i], color);
    }
    else {
        for (std::size_t i = 0; i < points.size()-1; ++i)
            g_lines.emplace_back(points[i], color);
    }
}

void drawPolyline(const std::vector<Vector2f> &points, const Color& color) {
    for (std::size_t i = 0; i < points.size()-1; ++i)
        drawLine(points[i],points[i+1],color);
}

void drawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c, const Color& color) {
    drawLine(a,b,color);
    drawLine(b,c,color);
    drawLine(c,a,color);
}

void drawRectangle(const Vector2f& position, float width, float height,  const Color& color) {
    auto a = position + Vector2f(-width, -height) * 0.5f;
    auto b = position + Vector2f( width, -height) * 0.5f;
    auto c = position + Vector2f( width,  height) * 0.5f;
    auto d = position + Vector2f(-width,  height) * 0.5f;
    drawLine(a,b,color);
    drawLine(b,c,color);
    drawLine(c,d,color);
    drawLine(d,a,color);
}

void drawCircle(const Vector2f &position, float radius, const Color& color) {
    std::size_t smoothness = 36;
    float angleIncrement = 2.0f * PI / smoothness;
    std::vector<Vector2f> polyline(smoothness + 1);
    for (std::size_t i = 0; i < smoothness + 1; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        polyline[i] = position + Vector2f(std::cos(angle) * radius, std::sin(angle) * radius);
    }
    drawPolyline(polyline,color);
}

void drawText(const std::string& _text,
               const Vector2f& position,
               const Color& color)
{
    Text text;
    text.setFont(Engine::fonts.get("RobotoMonoBold"));
    text.setPosition(position);
    text.setCharacterSize(10 * Engine::getDpiFactor());
    text.setScale(1.0f / Engine::getDpiFactor(), 1.0f / Engine::getDpiFactor());
    text.setFillColor(color);
    text.setString(_text);
    alignCenter(text);
    g_text.push_back(text);
}

namespace detail {

namespace {

void updateInfo() {
    g_info.frames++;
    g_info.cpuSum       += cpu_usage_process();
    g_info.ramSum       += ram_used_process() / 1000000;
    g_info.elapsedTime  += Engine::deltaTime();
    g_info.framesDisplay = Engine::frame();

    // updated every second
    if (g_info.elapsedTime >= 1.0) {
        // calculate averages
        g_info.fpsDisplay = g_info.frames;
        g_info.cpuDisplay = g_info.cpuSum / g_info.frames;
        g_info.ramDisplay = g_info.ramSum / g_info.frames;
        // reset
        g_info.frames = 0;
        g_info.cpuSum = 0.0;
        g_info.ramSum = 0;
        g_info.elapsedTime = 0.0f;
    }
}

void showContextMenu(int& corner) {
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
        if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
        if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
        if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
        if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
        ImGui::EndPopup();
    }
}

void infoMenu() {
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_piv = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    if (corner != -1)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_piv);
    ImGui::SetNextWindowBgAlpha(0.9f); // Transparent background
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin("Info", &g_gizmoFrameActive, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_AlwaysAutoResize*/ | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("CLK: %.2f s", Engine::time());
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Current time");

        ImGui::Text("FPS: %u", (int)g_info.fpsDisplay);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Frames per second");

        ImGui::Text("FRM: %u", (int)g_info.framesDisplay);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Current frame");

        ImGui::Separator();
        ImGui::Text("CPU: %.2f",   g_info.cpuDisplay);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Percent CPU utilized");

        ImGui::Text("RAM: %u MB", (int)g_info.ramDisplay);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Amount of RAM used");

        ImGui::Separator();
        ImGui::Text("PIX: %d,%d px", Input::getRawMousePosition().x, Input::getRawMousePosition().y);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mouse pixel position");


        ImGui::Text("X,Y: %.2f, %.2f", Input::getMousePosition().x, Input::getMousePosition().y);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mouse world position");

        ImGui::Separator();
        ImGui::Text("OBJ: %i", (int)Object::getObjectCount());
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Total Object count");

        ImGui::Text("RND: %i", (int)Renderer::getRendererCount());
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Total Renderer count");

        ImGui::Text("BDY: %i", (int)RigidBody::getRigidBodyCount());
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Total RigidBody count");

        showContextMenu(corner);
    }
    ImGui::End();
}

void gizmoMenu() {
    const float DISTANCE = 10.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_piv = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    if (corner != -1)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_piv);
    ImGui::SetNextWindowBgAlpha(0.9f); // Transparent background
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin("Gizmos", &g_gizmoFrameActive, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        for (auto& id : g_gizmoIds) {
            auto color = g_gizmoColors[id];
            color.a /= 2;
            ImGui::PushStyleColor(ImGuiCol_Header, color);
            ImGui::Selectable(g_gizmoNames[id].c_str(), &g_gizmoActives[id]);
            ImGui::PopStyleColor();
        }
        showContextMenu(corner);
    }
    ImGui::End();
}

void toolbarMenu() {
    const float DISTANCE = 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2(io.DisplaySize.x * 0.5f, DISTANCE);
    ImVec2 window_piv = ImVec2(0.5f, 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_piv);
    ImGui::SetNextWindowBgAlpha(0.9f); // Transparent background
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin("Toolbar", &g_gizmoFrameActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_AlwaysAutoResize*/ | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {

        if (!g_paused) {
            if (ImGui::Button(ICON_FA_PAUSE))
                g_paused = true;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Pause (F2)");
        }
        else {
            if (ImGui::Button(ICON_FA_PLAY))
                g_paused = false;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Play (F2)");
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STEP_FORWARD)) {
            if (!g_paused)
                g_paused = !g_paused;
            else
                g_advance = true;
        }            
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Step (F3)");
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP)) {
            Engine::stop();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Stop (F4)");
        ImGui::SameLine();
        if (g_panTool)
            ImGui::PushStyleColor(ImGuiCol_Text, Blues::LightSkyBlue);
        if (ImGui::Button(ICON_FA_ARROWS_ALT)) {
            if (g_panTool)
                ImGui::PopStyleColor();
            g_panTool = !g_panTool;
        }
        else {
            if (g_panTool)
                ImGui::PopStyleColor();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Pan Tool (F5)");
    }
    ImGui::End();
}

void clearDrawables() {
    g_text.clear();
    g_lines.clear();
    g_triangles.clear();
}

} // private namespace

void init()
{
    g_show = false;
    g_paused = false;
    g_advance = false;
    g_info = DebugInfo();
    g_panTool = false;

    g_gizmoIds.clear();
    g_gizmoNames.clear();
    g_gizmoColors.clear();
    g_gizmoActives.clear();
    g_triangles.clear();
    g_lines.clear();
    g_text.clear();    

    addGizmo("Transform", DEBUG_TRANSFORM_COLOR);
    addGizmo("Local Bounds", DEBUG_LOCAL_BOUNDS_COLOR);
    addGizmo("World Bounds", DEBUG_WORLD_BOUNDS_COLOR);
    addGizmo("Wireframe", DEBUG_WIREFRAME_COLOR);
    addGizmo("Physics COG", DEBUG_PHYSICS_COG_COLOR);
    addGizmo("Physics Shape", DEBUG_PHYSICS_SHAPE_COLOR);
}

void update() {
    // check for user input
    if (Input::getKeyDown(Key::F1))
        g_show = !g_show;
    if (Input::getKeyDown(Key::F2))
        g_paused = !g_paused;
    if (Input::getKeyDown(Key::F3)) {
        if (!g_paused)
            g_paused = !g_paused;
        else
            g_advance = true;
    }
    if (Input::getKeyDown(Key::F4))
        Engine::stop();
    if (Input::getKeyDown(Key::F5))
        g_panTool = !g_panTool;
    // pan tool
    if (g_panTool) {
        float scroll = Input::getScroll();
        if (scroll > 0) {
            Engine::getView(0).zoom(1.1f);
        }
        else if (Input::getScroll() < 0) {
            Engine::getView(0).zoom(1.0f/1.1f);
        }
        auto drag = Input::dragDeltaRaw(MouseButton::Left);
        Engine::getView(0).move(-drag.x, -drag.y); // TODO: doesn't work when paused
    }    
    // udate info
    updateInfo();
    // draw debug
    if (g_show) {
        // imgui
        infoMenu();
        gizmoMenu();
        toolbarMenu();
        // draw drawables
        if (g_triangles.size() > 0)
            Engine::window->draw(&g_triangles[0], g_triangles.size(), sf::Triangles);
        if (g_lines.size() > 0)
            Engine::window->draw(&g_lines[0], g_lines.size(), sf::Lines);
        for (auto& text : g_text)
            Engine::window->draw(text);
        // clear drawbales if we are going to advance frames
        if (!g_paused || g_advance)
            clearDrawables();
    }
    else
        clearDrawables();
}

bool proceed() {
    bool ret = !g_paused || g_advance;
    g_advance = false;
    return ret;
}

} // namespace detail
} // namespace Debug
} // namespace sfvg
