#include <SFVG/SFVG.hpp>
#include <deque>



// #ifdef _MSC_VER
// #    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
// #endif

using namespace sfvg;

float sinWave(float f, float t) {
    return std::sin(2.0f * PI * f *t);
}

float sqrWave(float f, float t) {
    return sinWave(f,t) > 0 ? 1.0f : -1.0f;

}

float sawWave(float f, float t) {
   return -2.0f / PI * std::atan(std::cos(PI * f * t) / std::sin(PI * f * t));
}

float triWave(float f, float t) {
    return 2.0f / PI * std::asin(std::sin(2.0f * PI * f * t));
}

class TfxDemo : public GameObject {
public:

    TfxDemo(Engine& engine) : GameObject(engine), chCB(8, false), sinwave(4410) { 
        chCB[0] = true;
    }

    void start() override {

        ImGuiStyle * style = &ImGui::GetStyle();
    
        // style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 2.0f;
        // style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 2.0f;
        // style->ItemSpacing = ImVec2(12, 8);
        // style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
    
        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.9f);
        style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.9f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.9f);
        style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        // style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        // style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
        // style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
        // style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = Blues::LightSkyBlue;
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = Blues::DodgerBlue;
        style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

    }

    void update() override {    

        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(Vector2f(470,350), ImGuiCond_Always);
        ImGui::Begin("TactorFX", &visible, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   

            if (ImGui::Button(ICON_FA_PLAY)) {

            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_STOP)) {

            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_SYNC_ALT)) {

            }

            ImGui::SameLine();
            ImGui::Text("MOTU Pro Audio");

            ImGui::Separator();

            for (std::size_t i = 0; i < 6; ++i)
            {
                auto label = str("Ch",i);
                if (ImGui::Button(label.c_str())) {

                }
                ImGui::SameLine();
                ImGui::Checkbox(str("##",i).c_str(), &chCB[i]);

                if (((i+1) % 6) != 0)
                    ImGui::SameLine();
            }

            ImGui::Separator();

            ImGui::RadioButton("Sine", &wave, 0);   ImGui::SameLine();
            ImGui::RadioButton("Square", &wave, 1); ImGui::SameLine();
            ImGui::RadioButton("Saw", &wave, 2);    ImGui::SameLine();
            ImGui::RadioButton("Triangle", &wave, 3);

            ImGui::DragInt("Frequency", &freq, 0.5f, 0, 250, "%i Hz");
            ImGui::DragInt("Modulation", &mod, 0.5f,  0, 250, "%i Hz");
            ImGui::DragFloat("Amplitude", &amp, 0.005f, 0.0f, 1.0f);
            ImGui::Separator();
            ImGui::DragInt("Attack", &a, 0.5f,  0, 100, "%i ms");
            ImGui::DragInt("Sustain", &s, 0.5f,  0, 100, "%i ms");
            ImGui::DragInt("Release", &r, 0.5f,  0, 100, "%i ms");
            ImGui::Separator();

            sinwave.clear();
            int durMs = a+s+r;
            float durS = (float)durMs / 1000.0f;
            int samples = (int)(durS * 44100.0f);

            for (int i = 0; i < samples; ++i) {
                float t = durS / (float)samples * (float)i;
                float v = amp;
                if (wave == 0)
                    v *= sinWave((float)freq, t);
                else if (wave == 1)
                    v *= sqrWave((float)freq, t);
                else if (wave == 2)
                    v *= sawWave((float)freq, t);
                else if (wave == 3)
                    v *= triWave((float)freq, t);
                if (mod > 0)
                    v*= std::sin(2.0f * PI * (float)mod * t);
                float env = 1.0f;
                if (t < (float)a/1000.0f)
                    env = interp(t,0,(float)a/1000.0f,0.0f,1.0f);
                else if (t > (float)(a+s)/1000.0f)
                    env = interp(t, (float)(a+s)/1000.0f, (float)(a+s+r)/1000.0f, 1.0f, 0.0f);
                v *= env;

                sinwave.push_back(v);
            }
            ImGui::PushItemWidth(375+80);
            auto title = str(durMs, "ms /", samples, " samples");
            ImGui::PlotLines("", &sinwave[0], (int)sinwave.size(),  0, title.c_str(), -1.0f, 1.0f, ImVec2(0,94));
            ImGui::PopItemWidth();
        ImGui::End();
    }

    int   freq = 175;
    float amp  = 0.5f;
    int   wave = 0;

    int mod = 20;
    float amp2 = 1.0f;

    int a = 15;
    int s = 50;
    int r = 35;

    std::deque<bool> chCB;
    std::vector<float> sinwave;

    bool visible;
};


int main(int argc, char const *argv[])
{
    Engine engine(480,360, WindowStyle::Close);
    engine.window.setTitle("TactorFX");

    // Image image;
    // image.loadFromFile("logo.png");
    // engine.window.setIcon(64,64,image.getPixelsPtr());

    engine.makeRoot<TfxDemo>();
    engine.run();
    return 0;
}
