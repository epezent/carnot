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

namespace sfvg {

namespace {
    static const std::array<std::string, DebugSystem::WidgetCount> g_widgetNames {
        "Transform",
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
    g_widgetColors[Widget::Transform] = DEBUG_TRANSFORM_COLOR;
    g_widgetColors[Widget::LocalBounds] =  DEBUG_LOCAL_BOUNDS_COLOR;
    g_widgetColors[Widget::WorldBounds] =  DEBUG_WORLD_BOUNDS_COLOR;
    g_widgetColors[Widget::Wireframe] =  DEBUG_WIREFRAME_COLOR;
    g_widgetColors[Widget::PhysicsCOG] =  DEBUG_PHYSICS_COG_COLOR;
    g_widgetColors[Widget::PhysicsShapes] =  DEBUG_PHYSICS_SHAPE_COLOR;

}

void DebugSystem::show(bool _show) {
    m_show = _show;
}

bool DebugSystem::isShown() const {
    return m_show;
}

void DebugSystem::drawPoint(const Vector2f& position, const Color& color) {
    m_triangles.emplace_back(position + Vector2f( 2.0f, -2.0f), color);
    m_triangles.emplace_back(position + Vector2f(-2.0f, -2.0f), color);
    m_triangles.emplace_back(position + Vector2f( 2.0f,  2.0f), color);
    m_triangles.emplace_back(position + Vector2f(-2.0f, -2.0f), color);
    m_triangles.emplace_back(position + Vector2f( 2.0f,  2.0f), color);
    m_triangles.emplace_back(position + Vector2f(-2.0f,  2.0f), color);
}

void DebugSystem::drawLine(const Vector2f& start, const Vector2f& end, const Color& color) {
    m_lines.emplace_back(start, color);
    m_lines.emplace_back(end, color);
}

void DebugSystem::drawLines(const std::vector<Vector2f> &points, const Color& color) {
    if (isEven((int)points.size())) {
        for (std::size_t i = 0; i < points.size(); ++i)
            m_lines.emplace_back(points[i], color);
    }
    else {
        for (std::size_t i = 0; i < points.size()-1; ++i)
            m_lines.emplace_back(points[i], color);
    }
}

void DebugSystem::drawPolyline(const std::vector<Vector2f> &points, const Color& color) {
    for (std::size_t i = 0; i < points.size()-1; ++i)
        drawLine(points[i],points[i+1],color);
}

void DebugSystem::drawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c, const Color& color) {
    drawLine(a,b,color);
    drawLine(b,c,color);
    drawLine(c,a,color);
}

void DebugSystem::drawRectangle(const Vector2f& position, float width, float height,  const Color& color) {
    auto a = position + Vector2f(-width, -height) * 0.5f;
    auto b = position + Vector2f( width, -height) * 0.5f;
    auto c = position + Vector2f( width,  height) * 0.5f;
    auto d = position + Vector2f(-width,  height) * 0.5f;
    drawLine(a,b,color);
    drawLine(b,c,color);
    drawLine(c,d,color);
    drawLine(d,a,color);
}

void DebugSystem::drawCircle(const Vector2f &position, float radius, const Color& color) {
    std::size_t smoothness = 36;
    float angleIncrement = 2.0f * PI / smoothness;
    std::vector<Vector2f> polyline(smoothness + 1);
    for (std::size_t i = 0; i < smoothness + 1; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        polyline[i] = position + Vector2f(std::cos(angle) * radius, std::sin(angle) * radius);
    }
    drawPolyline(polyline,color);
}

void DebugSystem::drawText(const std::string& _text,
               const Vector2f& position,
               const Color& color)
{
    Text text;
    text.setFont(engine.fonts.get("RobotoMonoBold"));
    text.setPosition(position);
    text.setCharacterSize(20);
    text.setScale(0.5f, 0.5f);
    text.setFillColor(color);
    text.setString(_text);
    alignCenter(text);
    m_text.push_back(text);
}


void DebugSystem::start() {
    // m_pauseText
    m_pauseText.setString("PAUSED (F2 = RESUME | F3 = STEP)");
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
        if (m_triangles.size() > 0)
            engine.window.draw(&m_triangles[0], m_triangles.size(), sf::Triangles);
        if (m_lines.size() > 0)
            engine.window.draw(&m_lines[0], m_lines.size(), sf::Lines);
        for (auto& text : m_text)
            engine.window.draw(text);
        // clear drawbales if we are going to advance frames
        if (!m_paused || m_advance)
            clearDrawables();
        // draw info
        engine.window.setView(engine.window.getDefaultView());
        showInfo();
        showWidgetMenu();
    }
    else
        clearDrawables();

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
}

void DebugSystem::showInfo() {
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    if (corner != -1)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ImGui::Begin("Debug Info", &m_widgetFrameActive, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_AlwaysAutoResize*/ | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("CLK: %.2f s", engine.time());
        ImGui::Text("FPS: %u", (int)m_info->fpsDisplay);
        ImGui::Text("FRM: %u", (int)m_info->framesDisplay);
        ImGui::Separator();
        ImGui::Text("CPU: %.2f",   m_info->cpuDisplay);
        ImGui::Text("RAM: %u MB", (int)m_info->ramDisplay);
        ImGui::Separator();
        ImGui::Text("PIX: %d,%d px", input.getRawMousePosition().x, input.getRawMousePosition().y);
        ImGui::Text("X,Y: %.2f, %.2f", input.getMousePosition().x, input.getMousePosition().y);
        ImGui::Separator();
        ImGui::Text("OBJ: %i", (int)Object::getObjectCount());
        ImGui::Text("RND: %i", (int)Renderer::getRendererCount());
        ImGui::Text("BDY: %i", (int)RigidBody::getRigidBodyCount());
        showContextMenu(corner);
    }
    ImGui::End();
}


void DebugSystem::showWidgetMenu() {
    const float DISTANCE = 10.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    if (corner != -1)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ImGui::Begin("Debug Widgets", &m_widgetFrameActive, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        for (std::size_t i = 0; i < WidgetCount; ++i)
            ImGui::Selectable(g_widgetNames[i].c_str(), &widgets[i]);
        showContextMenu(corner);
    }
    ImGui::End();
}

void DebugSystem::clearDrawables() {
    m_text.clear();
    m_lines.clear();
    m_triangles.clear();
}


} // namespace sfvg
