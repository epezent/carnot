#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class CircleObject : public GameObject {
public:
    CircleObject(Engine& engine) : GameObject(engine) {
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = CircleShape(50);
        sr->shape.setColor(randomColor());
        tr = addComponent<TextRenderer>();
        tr->text.setFont(engine.fonts.get("Roboto"));
        tr->text.setFillColor(Whites::White);
        tr->text.setCharacterSize(20);
        tr->text.setString("0.0");
        alignCenter(tr->text);
    }

    void update() override {
        if (engine.input.getKeyDown(Key::R)) {
            rb = addComponent<RigidBody>();
        }
        if (rb) {
            auto speed = magnitude(rb->getVelocity());
            tr->text.setString(numToStr(speed));
            alignCenter(tr->text);
            if (input.getKeyDown(Key::D))
                removeComponent(rb->getIndex());
        }
    }

    Handle<RigidBody> rb;
    Handle<TextRenderer> tr;
};

int main(int argc, char const *argv[]) {
    Engine engine(500, 500);
    engine.getView(0).setCenter(0, 250);
    engine.setLayerCount(2);
    engine.window.setKeyRepeatEnabled(false);
    engine.makeRoot<CircleObject>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
