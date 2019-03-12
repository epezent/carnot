#include "imgui_demo.cpp"

using namespace sfvg;

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
