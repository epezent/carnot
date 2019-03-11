#include "imgui_demo.cpp"

using namespace sfvg;

class MyGameObject : public GameObject {
public:
    MyGameObject(Engine& engine) : GameObject(engine) {
    }

    void update() {
        ImGui::ShowDemoWindow(&my_tool_active);
    }

    bool my_tool_active;

};



int main(int argc, char const *argv[])
{
    Engine engine(1000,1000);
    engine.makeRoot<MyGameObject>();
    engine.run();
    return 0;
}
