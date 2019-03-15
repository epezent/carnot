#include <Carnot/Carnot.hpp>
#include <deque>

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace carnot;

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

    TfxDemo() : chCB(8, false), sinwave(4410) { 
        chCB[0] = true;
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
            ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
            ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
            auto title = str(durMs, "ms /", samples, " samples");
            ImGui::PlotLines("", &sinwave[0], (int)sinwave.size(),  0, title.c_str(), -1.0f, 1.0f, ImVec2(0,94));
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
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
    Engine::init(480,360, WindowStyle::Close);
    Engine::window->setTitle("TactorFX");
    Engine::makeRoot<TfxDemo>();
    Engine::run();
    
    return 0;
}
