#include <SFVG/SFVG.hpp>

using namespace sfvg;

class CircleObject : public GameObject {
public:
    CircleObject(Engine& engine) : GameObject(engine) {
        sr1 = addComponent<ShapeRenderer>();
        sr1->shape = RectangleShape(100,20);
        sr1->setColor(randomColor());

        sr2 = addComponent<ShapeRenderer>();
        sr2->shape = RectangleShape(100,20);
        sr2->shape.setPosition(100, 100);
        sr2->setGradient(Gradient(randomColor(),randomColor()));
    }

    void update() {
        if (input.getKeyDown(Key::Space))
            sr1->shape.setRadii(5);
        if (input.getKey(Key::R))
            sr1->shape.rotate(60*engine.deltaTime());
        if (input.getKey(Key::Right))
            transform.rotate(60*engine.deltaTime());
        engine.debug.drawText("Hello,World", transform.getPosition());
    }
    Handle<ShapeRenderer> sr1;
    Handle<ShapeRenderer> sr2;
};

class SquareObject : public GameObject {
public:

    SquareObject(Engine& engine) :
        GameObject(engine)
    {

        sr1 = addComponent<ShapeRenderer>();
        sr1->shape = SquareShape(500);
        sr1->setColor(Whites::White);
        sr1->shape.addHole(SquareShape(498));

        sr2 = addComponent<ShapeRenderer>();
        sr2->shape = RectangleShape(500,30);
        sr2->setColor(Whites::White);
        sr2->shape.setPosition(0, (-250 + 30/2.0f));
        // makeChild<CircleObject>()->transform.setLocalPosition(250, 250);
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
    Engine engine(500, 500, WindowStyle::None);
    engine.getView(0).setCenter(0, 0);
    engine.setLayerCount(2);
    engine.window.setKeyRepeatEnabled(false);
    engine.makeRoot<SquareObject>();
    engine.window.setTitle("Evan's Engine");
    // engine.window.setPosition(Vector2i(0,0));
    engine.run();
    return 0;
}
