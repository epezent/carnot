#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;



class Player : public GameObject {
public:
    Player(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(100,100);
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
            auto v = input.getMousePosition() - transform.getPosition();
            rb->applyForceToCenter(200.0f * v - 15.0f * rb->getVelocity());
        }

        engine.debug.drawText(str("Mass:",rb->getMass()),transform.getLocalPosition() + Vector2f(0,65));
        engine.debug.drawText(str("Moment:",rb->getMoment()),transform.getLocalPosition() + Vector2f(0,80));
        engine.debug.drawPoint(transform.getLocalPosition());

    }

    Handle<ShapeRenderer> sr;
    Handle<Collider> col;
    Handle<RigidBody> rb;
};

class Wall : public GameObject {
public:
    Wall(Engine& engine, float width, float height, float x, float y) : GameObject(engine) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(width,height);
        sr->shape.setColor(Color::Black);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        auto co = addComponent<BoxCollider>(width,height);
    }
};

class FreeBody : public GameObject {
public:
    FreeBody(Engine& engine, Vector2f pos) : GameObject(engine) {
        float width = 15;
        float height = 15;
        transform.setLocalPosition(pos);
        transform.setLocalRotation(random(0.0f,360.0f));
        sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(width,height);
        sr->shape.setColor(color = randomColor());
        rb = addComponent<RigidBody>(RigidBody::Dynamic);
        co = addComponent<BoxCollider>(width,height);
    }

    void update() {
        auto localPos = transform.worldToLocal(input.getMousePosition());

        if (inBounds(localPos, sr->shape.getLocalBounds()) && input.getMouseDown(MouseButton::Left))
            active = true;
        if (input.getMouseUp(MouseButton::Left))
            active = false;

        if (active || inBounds(localPos, sr->shape.getLocalBounds())) {
            sr->shape.setColor(Color::Black);
            if (input.getMouse(MouseButton::Left)) {
                auto v = input.getMousePosition() - transform.getPosition();
                rb->applyForceToCenter(200.0f * v - 15.0f * rb->getVelocity());
            }
            if (input.getKeyDown(Key::D))
                destroy();
        }
        else
            sr->shape.setColor(color);

    }

    Color color;
    bool active = false;
    Handle<ShapeRenderer> sr;
    Handle<RigidBody> rb;
    Handle<Collider> co;
};

class Root : public GameObject {
public:
    Root(Engine& engine) : GameObject(engine) {
        makeChild<Wall>(1920,20,0,540);
        makeChild<Wall>(1920,20,0,-540);
        makeChild<Wall>(20,1080,-960,0);
        makeChild<Wall>(20,1080,960,0);
        // makeChild<Player>();
    }

    void update() {
        if (input.getKeyDown(Key::G))
            engine.physics.setGravity(Vector2f(0,1000) - engine.physics.getGravity());
        if (input.getMouse(MouseButton::Right))
            makeChild<FreeBody>(input.getMousePosition());
    }
};

int main(int argc, char const *argv[]) {
    Engine engine;
    engine.getView(0).setCenter(0, 0);
    engine.setLayerCount(2);
    engine.debug.show(true);
    engine.setBackgroundColor(Grays::Gray50);
    engine.window.setKeyRepeatEnabled(false);
    auto root = engine.makeRoot<Root>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
