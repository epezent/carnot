#include "imgui_demo.cpp"

using namespace sfvg;

class MyGameObject : public GameObject {
public:
    MyGameObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(Whites::White);
        sr->shape = PolygonShape(6, PolygonShape::CircumscribedRadius, 100);
        engine.textures.load("paper", "../../textures/paper.png");
    }

    void guiCode() {

        const float DISTANCE = 10.0f;
        static int corner = 0;
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        static bool selection[6] = { false, true, false, false, false, true };

        if (corner != -1)
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
        if (ImGui::Begin("SFVG Gizmos", &my_tool_active, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            ImGui::Selectable("Transform", &selection[0]);
            ImGui::Selectable("Local Bounds", &selection[1]);
            ImGui::Selectable("World Bounds", &selection[2]);
            ImGui::Selectable("Wireframe", &selection[3]);
            ImGui::Selectable("Physics COG", &selection[4]);
            ImGui::Selectable("Physics Shape", &selection[5]);
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                if (my_tool_active && ImGui::MenuItem("Close")) my_tool_active = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void update() {
        //guiCode();
        ImGui::ShowDemoWindow(&my_tool_active);
    }


    float my_color[4] = { 0.5, 0.5, 0.5, 0.5 };
    bool my_tool_active;
    Handle<ShapeRenderer> sr;
};



int main(int argc, char const *argv[])
{
    Engine engine;
    engine.getView(0).setCenter(0, 0);
    engine.makeRoot<MyGameObject>();
    engine.run();
    return 0;
}
