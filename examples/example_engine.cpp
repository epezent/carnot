#include <SFVG/SFVG.hpp>

using namespace sfvg;

class TestObject : public GameObject {
public:

    TestObject()
    {
        // init background
        bg = makeChild<GameObject>();
        std::size_t n = 50;
        float s = 500.0f/n;
        for (std::size_t i = 0; i < n/2; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                auto check = bg->addComponent<ShapeRenderer>();
                check->shape = make<SquareShape>(s);
                check->shape->setPosition(s*(j%2) + s/2.0f  + 2*i * s, s/2  + j * s);
                check->setColor(Grays::Gray50);
                check->setLayer(0);
            }
        }

        stroke = addComponent<StrokeRenderer>();
        stroke->setLayer(1);
        sr = addComponent<ShapeRenderer>();
        sr->setLayer(1);
        sr->shape = make<SquareShape>(50);
        sr->setColor(Grays::Gray50);
        sr->shape->setPosition(250,250);

        auto b = Blues::DeepSkyBlue;
        b.a = 128;
        sr->setGradient(Gradient(b, Greens::Chartreuse));
        stroke->addVertex(250,250,randomColor(), 5.0f);
    }

    void update() override {

        if (Input::getKey(Key::R)) {
            if (Input::getKey(Key::LControl))
                transform.rotate(90*Engine::deltaTime());
            else
                transform.rotate(-90*Engine::deltaTime());
        }     

        if (Input::getMouseDown(MouseButton::Left))
            stroke->addVertex(Input::getMousePosition(),randomColor(), 5.0f);

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


int main(int argc, char const *argv[]) {
    Engine::init(500, 500);
    Engine::setLayerCount(2);
    auto root = Engine::makeRoot<TestObject>();
    Engine::setBackgroundColor(Whites::White);
    Engine::window->setTitle("Evan's Engine");
    Engine::run();    
    return 0;
}
