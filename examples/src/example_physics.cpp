#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class PhysicsObject : public GameObject {
public:
    PhysicsObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->shape = StarShape(5);
        sr->shape.setColor(randomColor());
    }

    void start() override {
        engine.physics.setGravity(Vector2f());
        rb = addComponent<RigidBody>();
    }

    void update() override {
        if (input.getDoubleClick(MouseButton::Left))
            sr->shape.setColor(randomColor());
        auto v = input.getMousePosition() - transform.getGlobalPosition();
        rb->applyForceToCenter(200.0f * v - 30.0f * rb->getVelocity());
        std::stringstream ss;
        ss << "Local  " << transform.getPosition() << std::endl;
        ss << "Global " << transform.getGlobalPosition();
        engine.debug.drawLabel(ss.str(), transform.getGlobalPosition());
        engine.debug.drawLine(transform.getGlobalPosition(), input.getMousePosition(), Greens::Chartreuse);
    }

    Handle<ShapeRenderer> sr;
    Handle<RigidBody> rb;
};

int main(int argc, char const *argv[]) {
    Engine engine(1920, 1080, WindowStyle::Fullscreen);
    engine.getView(0).setCenter(0, 0);
    engine.setLayerCount(2);
    engine.debug.show(true);
    engine.window.setKeyRepeatEnabled(false);
    auto root = engine.makeRoot<PhysicsObject>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
