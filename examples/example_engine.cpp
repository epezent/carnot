#include <carnot>
using namespace carnot;

class TestObject : public GameObject {
public:

    TestObject()
    {
        stroke = addComponent<StrokeRenderer>();
        stroke->setLayer(1);
        sr = addComponent<ShapeRenderer>();
        sr->setLayer(1);
        sr->setShape(make<SquareShape>(50));
        sr->setColor(Grays::Gray50);
        sr->getShape()->move(250,250);

        auto b = Blues::DeepSkyBlue;
        b.a = 128;
        sr->setEffect(make<Gradient>(b, Greens::Chartreuse));
        stroke->addVertex(250,250,Random::color(), 5.0f);
    }

    void update() override {

        if (Input::getKey(Key::R)) {
            if (Input::getKey(Key::LControl))
                transform.rotate(90*Engine::deltaTime());
            else
                transform.rotate(-90*Engine::deltaTime());
        }     

        if (Input::getMouseDown(MouseButton::Left))
            stroke->addVertex(Input::getMousePosition(),Random::color(), 5.0f);

        if (Input::getKeyDown(Key::Up))
            transform.scale(0.5f,0.5f);
        else if (Input::getKeyDown(Key::Down))
            transform.scale(2.0f,2.0f);

        if (Input::getKeyDown(Key::Space))
            bg->setEnabled(!bg->isEnabled());

    }

private:

    Handle<StrokeRenderer> stroke;
    Handle<ShapeRenderer> sr;
    Handle<GameObject> bg;
};


class MyObject : public GameObject {
public:

    void start() {
        sr = addComponent<ShapeRenderer>();
        sr->setShape(make<CircleShape>(50.0f));
        sr->setColor(Blues::Blue);
        col.resize(4);
        sr->getShape()->move(250,250);
    }

    void update() override {

        ImGui::Begin("My Window");
        if (ImGui::Button("Yes"))
            print("Yes");
        ImGui::SameLine();
        if (ImGui::Button("No"))
            print("No");

        ImGui::ColorEdit4("Color", &col[0]);
        RGB rbg;
        rbg.r = col[0];
        rbg.g = col[1];
        rbg.b = col[2];
        rbg.a = col[3];

        sr->setColor(rbg);
        ImGui::End();
    }

    Handle<ShapeRenderer> sr;
    std::vector<float> col;
    Color color;

};


int main(int argc, char const *argv[]) {
    Engine::init(500, 500);
    // Engine::setLayerCount(2);
    // auto root = Engine::makeRoot<TestObject>();
    // Engine::setBackgroundColor(Whites::White);
    // Engine::window->setTitle("Evan's Engine");
    Engine::makeRoot<MyObject>();
    Engine::run();    
    return 0;
}
