#include <SFVG/SFVG.hpp>

using namespace sfvg;

class TestObject : public GameObject {
public:

    TestObject(Engine& engine) :
        GameObject(engine)
    {
        stroke = addComponent<StrokeRenderer>();
        sr = addComponent<ShapeRenderer>();
        sr->shape = SquareShape(50);
        sr->setColor(Grays::Gray50);
        sr->shape.setPosition(250,250);

        auto b = Blues::DeepSkyBlue;
        b.a = 128;
        sr->setGradient(Gradient(b, Greens::Chartreuse));
        // stroke->addPoint(0,0);
        // stroke->addPoint(250,250);
        stroke->setColor(b);
        stroke->setThickness(10);
        stroke->setPointCount(2);
    }

    void update() override {

        if (input.getKey(Key::R)) {
            if (input.getKey(Key::LControl))
                transform.rotate(90*engine.deltaTime());
            else
                transform.rotate(-90*engine.deltaTime());
        }     

        stroke->setPoint(1, input.getMousePosition());

        if (input.getKeyDown(Key::Up))
            transform.scale(0.5f,0.5f);
        else if (input.getKeyDown(Key::Down))
            transform.scale(2.0f,2.0f);

    }

private:

    Handle<StrokeRenderer> stroke;
    Handle<ShapeRenderer> sr;
};

int main(int argc, char const *argv[]) {
    Engine engine(500, 500);
    engine.makeRoot<TestObject>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
