#include <SFVG/SFVG.hpp>

using namespace sfvg;

class MyGameObject : public GameObject {
public:
    MyGameObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(Whites::White);
        sr->shape = PolygonShape(6, PolygonShape::CircumscribedRadius, 100);
    }

    void guiCode() {
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    print("Saved!");
                }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Edit a color (stored as ~4 floats)
        ImGui::ColorEdit4("Color", my_color);
        // Plot some values
        const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
        ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = 0; n < 50; n++)
            ImGui::Text("%04d: Some text", n);
        ImGui::EndChild();
        ImGui::End();

        Color col;
        col.r = my_color[0] * 255;
        col.g = my_color[1] * 255;
        col.b = my_color[2] * 255;
        col.a = my_color[3] * 255;

        sr->setColor(col);
    }

    void update() {
        guiCode();
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

