#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class PhysicsObject : public GameObject {
public:
    PhysicsObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->shape = SquareShape(100);
        sr->shape.setGradient(Gradient(randomColor(),randomColor(),random(0.f,360.f)));
        setName("physicsGO");
        rb = addComponent<RigidBody>();
        col = addComponent<BoxCollider>(100,100);
    }

    void start() override {
        engine.physics.setDamping(0.5);
    }

    void update() override {
        if (input.getDoubleClick(MouseButton::Left))
            sr->shape.setGradient(Gradient(randomColor(),randomColor(),random(0.f,360.f)));


        if (input.getMouse(MouseButton::Left)) {
            auto v = input.getMousePosition() - transform.getGlobalPosition();
            rb->applyForceToCenter(200.0f * v - 15.0f * rb->getVelocity());
        }

        // engine.debug.drawText(str("Position:",transform.getPosition()), transform.getGlobalPosition()+ Vector2f(0,65));
        // engine.debug.drawText(str("Velocity:",rb->getVelocity()),transform.getGlobalPosition() + Vector2f(0,80));
        engine.debug.drawText(str("Mass:",rb->getMass()),transform.getPosition() + Vector2f(0,65));
        engine.debug.drawText(str("Moment:",rb->getMoment()),transform.getPosition() + Vector2f(0,80));
        engine.debug.drawPoint(transform.getPosition());

    }

    Handle<ShapeRenderer> sr;
    Handle<Collider> col;
    Handle<RigidBody> rb;
};

int main(int argc, char const *argv[]) {
    Engine engine;
    engine.getView(0).setCenter(0, 0);
    engine.physics.setGravity(Vector2f());
    engine.setLayerCount(2);
    engine.debug.show(true);
    engine.window.setKeyRepeatEnabled(false);
    auto root = engine.makeRoot<GameObject>();
    root->makeChild<PhysicsObject>();
    auto x = root->makeChild<GameObject>();
    x->transform.setPosition(0,200);
    auto sr = x->addComponent<ShapeRenderer>();
    sr->shape = SquareShape(100);
    sr->shape.setColor(Whites::White);
    x->addComponent<RigidBody>(RigidBody::Dynamic);
    x->addComponent<BoxCollider>(100,100);
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
