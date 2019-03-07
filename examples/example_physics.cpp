#include <SFVG/SFVG.hpp>

using namespace sfvg;

class Wall : public GameObject {
public:
    Wall(Engine& engine, float width, float height, float x, float y) : GameObject(engine) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(width,height);
        sr->setColor(Color::Black);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width,height);
    }
};

class RectObject : public GameObject {
public:
    RectObject(Engine& engine) : GameObject(engine) {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(color = randomColor());
        start = input.getMousePosition();
    }

    void update() {
        if (!confirmed && input.getMouseUp(MouseButton::Right)) {
            if (absVec(start - end) == Vector2f(0,0)) {
                destroy();
            }
            else {
                confirmed = true;
                rb = addComponent<RigidBody>(RigidBody::Dynamic);
                rb->addShape(sr->shape);
                rb->setShapeDensity(0, 1.0f);
                rb->setShapeFriction(0, 0.7f);
                rb->setShapeElasticity(0, 0.0f);
            }
        }
        if (!confirmed && input.getMouse(MouseButton::Right)) {
            end = input.getMousePosition();
            auto size = absVec(start - end);
            sr->shape = RectangleShape(size.x, size.y);
            transform.setPosition(start + 0.5f * (end - start));
        }
        if (rb) {
            engine.debug.drawText(str("mass = ",rb->getMass()), transform.getPosition());
            auto localPos = transform.worldToLocal(input.getMousePosition());
            if (inBounds(localPos, sr->shape.getLocalBounds()) && input.getKeyDown(Key::D))
                destroy();
            
        }
    }

    Color color;
    Handle<ShapeRenderer> sr;
    Handle<RigidBody> rb;
    Vector2f start, end;
    bool confirmed = false;
};

class Player : public GameObject {
public:
    Player(Engine& engine) : GameObject(engine) {
        makeChild<Wall>(750,50,0,375);
        makeChild<Wall>(750,50,0,-375);
        makeChild<Wall>(50,750,-375,0);
        makeChild<Wall>(50,750,375,0);
    }

    void update() {
        if (input.getKeyDown(Key::G))
            engine.physics.setGravity(Vector2f(0,1000) - engine.physics.getGravity());
        if (input.getMouseDown(MouseButton::Right))
            makeChild<RectObject>();
        if (input.getMouseDown(MouseButton::Middle)) {
            auto go = makeChild<GameObject>();
            go->transform.setPosition(input.getMousePosition());
            go->addComponent<ShapeRenderer>();
            go->getComponent<ShapeRenderer>()->setColor(randomColor());
            go->getComponent<ShapeRenderer>()->shape = CircleShape(30);
            go->getComponent<ShapeRenderer>()->shape.addHole(StarShape(20,25,20));
            go->addComponent<RigidBody>();
            go->getComponent<RigidBody>()->addCircleShape(30);
            go->getComponent<RigidBody>()->setShapeDensity(0, 1.0f);
        }
    }
};


int main(int argc, char const *argv[]) {
    Engine engine(750,750);
    engine.getView(0).setCenter(0, 0);
    engine.setLayerCount(2);
    engine.debug.show(true);
    engine.setBackgroundColor(Grays::Gray10);
    engine.window.setKeyRepeatEnabled(false);
    auto root = engine.makeRoot<Player>();
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
