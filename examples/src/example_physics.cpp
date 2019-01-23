#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class PhysicsObject : public GameObject {
public:
    PhysicsObject(Engine& engine) : GameObject(engine) {
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = SquareShape(50);
        sr->shape.setColor(randomColor());
        tr = addComponent<TextRenderer>();
        tr->text.setFont(engine.fonts.get("RobotoMonoBold"));
        tr->text.setFillColor(Whites::White);
        tr->text.setCharacterSize(20);
        tr->text.scale(0.5f, 0.5f);
        tr->text.setString("0.0");
        alignCenter(tr->text);
    }

    void start() override {
        // engine.physics.setGravity(Vector2f());
    }

    void update() override {
        if (engine.input.getKeyDown(Key::R)) {
            rb = addComponent<RigidBody>();
        }
        if (rb) {
            if (input.getKeyDown(Key::D))
                removeComponent(rb->getIndex());
            if (input.getKey(Key::F)) {
                rb->applyForceToCenter(Vector2f(0,-2000));
                // rb->applyTorqueToCenter(10.0f);
                // rb->setVelocity(Vector2f(50.0f,50.0f));
            }
        }

        // std::stringstream ss;
        // ss << "Local  " << transform.getPosition() << std::endl;
        // ss << "Global " << transform.getGlobalPosition();
        // tr->text.setString(ss.str());
        // alignCenter(tr->text);
    }

    Handle<RigidBody> rb;
    Handle<TextRenderer> tr;
};

int main(int argc, char const *argv[]) {
    Engine engine(500, 1000);
    engine.getView(0).setCenter(0, 0);
    engine.setLayerCount(2);
    engine.showInfo(true);
    engine.window.setKeyRepeatEnabled(false);
    auto root = engine.makeRoot<PhysicsObject>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
