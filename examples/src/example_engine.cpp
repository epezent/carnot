#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class CircleObject : public GameObject {
public:

    CircleObject(Engine& engine) : GameObject(engine) {
        auto sr = addComponent<ShapeRenderer>();
        sr->drawable = CircleShape(50);
        sr->drawable.setColor(randomColor());
    }

};

class SquareObject : public GameObject {
public:

    SquareObject(Engine& engine) :
        GameObject(engine)
    {

        sr1 = addComponent<ShapeRenderer>();
        sr1->drawable = SquareShape(50);
        sr1->drawable.setColor(randomColor());

        sr2 = addComponent<ShapeRenderer>();
        sr2->drawable = SquareShape(25);
        sr2->drawable.setColor(randomColor());

        makeChild<CircleObject>()->transform.setPosition(250, 250);
    }

    void update() override {

        transform.move(input.dragDelta(MouseButton::Left));
        transform.rotate(input.dragDelta(MouseButton::Right).x);

        if (input.getKeyDown(Key::S))
            sr1->decrementLayer();
        if (input.getKeyDown(Key::W))
            sr1->incrementLayer();
    }

private:

    Handle<ShapeRenderer> sr1, sr2;

};

int main(int argc, char const *argv[]) {
    Engine engine(500, 500);
    engine.setLayerCount(2);
    engine.window.setKeyRepeatEnabled(false);
    engine.makeRoot<SquareObject>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
