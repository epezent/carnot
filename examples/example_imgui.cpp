#include "imgui_demo.cpp"
#include <carnot>

using namespace carnot;

class MyGameObject : public GameObject {
public:

    void update() {
        ImGui::ShowDemoWindow(&my_tool_active);
    }

    bool my_tool_active;

};



int main(int argc, char const *argv[])
{
    Engine::init();
    Engine::makeRoot<MyGameObject>();
    Engine::run();
    
    return 0;
}
