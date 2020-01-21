#include "ImGui/imgui_demo.cpp"
#include <carnot>

using namespace carnot;

class MyGameObject : public GameObject {
public:

    void update() {
        ImGui::ShowDemoWindow(&window_open);
    }

    bool window_open;

};

int main(int argc, char const *argv[])
{
    Engine::init(1000,1000,WindowStyle::Default,"ImGUI (imgui_demo.cpp)");
    Engine::makeRoot<MyGameObject>();
    Engine::run();
    
    return 0;
}
