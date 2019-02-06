#include "imgui_demo.cpp"

using namespace sfvg;

class MyGameObject : public GameObject {
public:
    MyGameObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(Whites::White);
        sr->shape = PolygonShape(6, PolygonShape::CircumscribedRadius, 100);
        engine.textures.load("paper", "../../textures/paper.png");
        lr = addComponent<LineRenderer>(2);
        lr->fromShape(sr->shape);
        auto c = Reds::FireBrick;
        c.a = 128;
        lr->setColor(Reds::FireBrick);
        lr->setThicnkess(10);
    }

    void update() {
        lr->setPoint(1, input.getMousePosition());
        ImGui::ShowDemoWindow(&my_tool_active);
    }


    float my_color[4] = { 0.5, 0.5, 0.5, 0.5 };
    bool my_tool_active;
    Handle<ShapeRenderer> sr;
    Handle<LineRenderer> lr;
};



int main(int argc, char const *argv[])
{
    Engine engine;
    engine.getView(0).setCenter(0, 0);
    engine.makeRoot<MyGameObject>();
    engine.run();
    return 0;
}
