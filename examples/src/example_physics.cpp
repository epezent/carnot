#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class PhysicsObject : public GameObject {
public:
    PhysicsObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->shape = SquareShape(100);
        sr->shape.setColor(randomColor());
    }

    void start() override {
        engine.physics.setGravity(Vector2f());
        rb = addComponent<RigidBody>();
        // engine.window.setMouseCursorVisible(bool visible)
    }

    void update() override {
        if (input.getDoubleClick(MouseButton::Left))
            sr->shape.setGradient(Gradient(randomColor(),randomColor(),random(0.f,360.f)));
        auto v = input.getMousePosition() - transform.getGlobalPosition();
        rb->applyForceToCenter(500.0f * v - 40.0f * rb->getVelocity());
        engine.debug.drawText(str("Position",transform.getPosition()), transform.getGlobalPosition());
        engine.debug.drawLine(transform.getPosition(), input.getMousePosition());
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
