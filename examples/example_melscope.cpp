// #define CARNOT_NO_CONSOLE
#include <carnot>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <Engine/ImGui/imgui_internal.h>
#include <experimental/filesystem>

//=============================================================================

namespace ImGui {

void PlotEx2(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    // skip render if SkipItems on
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    // get ImGuiContext and Style
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0, &frame_bb))
        return;
    const bool hovered = ItemHoverable(inner_bb, 0);

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data, i);
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    // render background frame
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);


        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
        const ImU32 col_hovered = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

        for (int n = 0; n < values_count; n++)
        {
            const float t1 = t0 + t_step;
            // const int v1_idx = (int)(t0 * item_count + 0.5f);
            // IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            // const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
            // const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            // ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            //window->DrawList->AddLine(pos0, pos1, col_hovered);
            // else if (plot_type == ImGuiPlotType_Histogram)
            // {
            //     if (pos1.x >= pos0.x + 2.0f)
            //         pos1.x -= 1.0f;
            //     window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            // }

            // t0 = t1;
            //tp0 = tp1;

        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f,0.0f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData2
{
    const float* Values;
    int Stride;

    ImGuiPlotArrayGetterData2(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter2(void* data, int idx)
{
    ImGuiPlotArrayGetterData2* plot_data = (ImGuiPlotArrayGetterData2*)data;
    const float v = *(const float*)(const void*)((const unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void PlotLines2(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
    ImGuiPlotArrayGetterData2 data(values, stride);
    PlotEx2(ImGuiPlotType_Lines, label, &Plot_ArrayGetter2, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

}






using namespace carnot;

void tooltip(const std::string& tip) {
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(tip.c_str());
}

class MelScope : public GameObject {
public:

    MelScope() {
        sinwave.resize(1000);
        color = Random::color();
    }

    void update() {
        if (!paused) {
            for (auto i : range(1000)) {
                sinwave[i] = 0.5f * Math::sin(2 * Math::PI * (Engine::time() + (float)i/100.0f)) + 0.5f * Math::sin(2 * Math::PI * 0.5 * (Engine::time() + (float)i/100.0f));
            }
        }
        auto winSize = static_cast<Vector2f>(Engine::window->getSize());
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(Vector2f(winSize.x - 10, winSize.y - 10), ImGuiCond_Always);
        ImGui::Begin("TactorFX", &visible, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

            if (ImGui::Button(ICON_FA_PLUS_SQUARE) || Input::getKeyDown(Key::A) && Input::getKey(Key::LControl)) {
                ImGui::OpenPopup("Add MelShare");
            }
            tooltip("Add MelShare (Ctrl+A)");

            if (ImGui::BeginPopupModal("Add MelShare", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                static char buf[32] = "";
                ImGui::InputText("", buf, IM_ARRAYSIZE(buf));
                ImGui::SameLine();
                if (ImGui::Button("OK")) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_PAUSE) || Input::getKeyDown(Key::Space)) {
                paused = !paused;
            }
            tooltip("Pause Scope (Space)");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_SYNC_ALT)) {

            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_EXPAND)) {

            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CROSSHAIRS)) {

            }

            
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_TH) || Input::getKeyDown(Key::G) && Input::getKey(Key::LControl)) {
            }
            tooltip("Scope Grid (Ctrl+G)");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_PALETTE) || Input::getKeyDown(Key::C) && Input::getKey(Key::LControl)) {
                color = Random::color();
            }
            tooltip("Configure Colors (Ctrl+C)");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CAMERA)) {

            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FILE_CSV)) {

            }
            tooltip("Save to CSV");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_COGS)) {

            }
            tooltip("Settings");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_INFO)) {

            }
            tooltip("Info");

            ImGui::PushItemWidth(winSize.x - 25);
            ImGui::PushStyleColor(ImGuiCol_PlotLines, color);
            ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, color);
            auto title = str("Plot 0");
            ImGui::PlotLines2("Hmm", &sinwave[0], (int)sinwave.size(),  0, title.c_str(), -1.0f, 1.0f, ImVec2(0,winSize.y-52));
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
            // ImGui::RenderText(ImVec2(10,10), "Hello, World");
        ImGui::End();
    }

    Color color;
    bool visible;
    std::vector<float> sinwave;
    bool paused = false;

};

int main(int argc, char const *argv[])
{
    Engine::init(480,360, WindowStyle::Default);
    Engine::window->setTitle("MEL Scope");
    Engine::makeRoot<MelScope>();
    Engine::run();    
    return 0;
}
