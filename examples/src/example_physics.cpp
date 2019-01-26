#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>

using namespace sfvg;

class Wall : public GameObject {
public:
    Wall(Engine& engine, float width, float height, float x, float y) : GameObject(engine) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(width,height);
        sr->shape.setColor(Color::Black);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width,height);
    }
};

class FreeBody : public GameObject {
public:
    FreeBody(Engine& engine, Vector2f pos) : GameObject(engine) {
        transform.setPosition(pos);
        transform.setRotation(random(0.0f,360.0f));
        sr = addComponent<ShapeRenderer>();
        rb = addComponent<RigidBody>(RigidBody::Dynamic);
        auto shape = SquareShape(20);
        sr->shape = shape;
        sr->shape.setColor(color = randomColor());
        rb->addShape(shape);
        shape.setPosition(20, 0);
        shape.applyTransform();
        rb->addShape(shape);
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
            if (input.getKey(Key::D))
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
        makeChild<Wall>(750,50,0,375);
        makeChild<Wall>(750,50,0,-375);
        makeChild<Wall>(50,750,-375,0);
        makeChild<Wall>(50,750,375,0);
    }

    void update() {
        if (input.getKeyDown(Key::G))
            engine.physics.setGravity(Vector2f(0,1000) - engine.physics.getGravity());
        if (input.getMouseDown(MouseButton::Right))
            makeChild<FreeBody>(input.getMousePosition());
    }
};

int main(int argc, char const *argv[]) {
    Engine engine(750,750);
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
