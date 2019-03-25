#include <carnot>
using namespace carnot;

class Wall : public GameObject {
public:
    Wall(float width, float height, float x, float y) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->setShape(make<RectangleShape>(width,height));
        sr->setColor(Grays::Gray50);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addShape(make<RectangleShape>(width, height), 1.0f);
    }
};

class RectObject : public GameObject {
public:
    RectObject() {
        sr = addComponent<ShapeRenderer>();
        color = Random::color();
        sr->setColor(color);
        start = Input::getMousePosition();
    }

    void update() {
        if (!confirmed && Input::getMouseUp(MouseButton::Left)) {
            if (Math::absVec(start - end) == Vector2f(0,0)) {
                destroy();
            }
            else {
                confirmed = true;
                rb = addComponent<RigidBody>(RigidBody::Dynamic);
                rb->addShape(sr->getShape(), 1.0f, 0.5f, 0.0f);
            }
        }
        if (!confirmed && Input::getMouse(MouseButton::Left)) {
            end = Input::getMousePosition();
            auto size = Math::absVec(start - end);
            sr->setShape(make<RectangleShape>(size.x, size.y));
            transform.setPosition(start + 0.5f * (end - start));
        }
        if (rb) {
            Debug::drawText(str("mass = ",rb->getMass()), transform.getPosition(), Whites::White);
            auto localPos = transform.worldToLocal(Input::getMousePosition());
            if (Math::inBounds(localPos, sr->getShape()->getLocalBounds()) && Input::getKeyDown(Key::D))
                destroy();
            
        }
    }

    Color color;
    Handle<ShapeRenderer> sr;
    Handle<RigidBody> rb;
    Vector2f start, end;
    bool confirmed = false;
};

class CompoundBody : public GameObject {
public:
    Handle<RigidBody> rb;

    CompoundBody() {
        transform.setPosition(Input::getMousePosition());
        rb = addComponent<RigidBody>();
        rb->addBoxShape(100,25);
        rb->addCircleShape(25, Vector2f(50,0));
        rb->addCircleShape(25, Vector2f(-50,0));
    }

    void update() override {
        if (Input::getKeyDown(Key::I))
            print(rb->getShapeCount());
        if (Input::getKey(Key::Up))
            rb->applyForceToCenter(0,-2000);
        if (Input::getKey(Key::Down))
            rb->applyForceToCenter(0,2000);
        if (Input::getKey(Key::Left))
            rb->applyForceToCenter(-2000,0);
        if (Input::getKey(Key::Right))
            rb->applyForceToCenter(2000,0);
        if (Input::getKey(Key::PageDown))
            rb->applyTorqueToCenter(100);
        if (Input::getKey(Key::PageUp))
            rb->applyTorqueToCenter(-100);
    }
};

class Player : public GameObject {
public:
    Player() {
        makeChild<Wall>(750,50,0,375);
        makeChild<Wall>(750,50,0,-375);
        makeChild<Wall>(50,750,-375,0);
        makeChild<Wall>(50,750,375,0);

        auto bg = addComponent<ShapeRenderer>(make<SquareShape>(750));
        bg->setEffect(make<Checkerboard>(Grays::Gray70, Whites::White, 100));
    }

    void update() {
        if (Input::getKeyDown(Key::G))
            Physics::setGravity(Vector2f(0,1000) - Physics::getGravity());
        if (Input::getMouseDown(MouseButton::Left))
            makeChild<RectObject>();
        if (Input::getMouseDown(MouseButton::Right)) {
            auto go = makeChild<GameObject>();
            go->transform.setPosition(Input::getMousePosition());
            go->addComponent<ShapeRenderer>();
            go->getComponent<ShapeRenderer>()->setColor(Random::color());
            go->getComponent<ShapeRenderer>()->setShape(make<CircleShape>(30));
            go->getComponent<ShapeRenderer>()->getShape()->addHole(StarShape(20,25,20));
            go->addComponent<RigidBody>();
            go->getComponent<RigidBody>()->addShape(go->getComponent<ShapeRenderer>()->getShape(), 1.0f, 0.5f, 1.0f);
            go->getComponent<RigidBody>()->setShapeDensity(0, 1.0f);
        }
        if (Input::getKeyDown(Key::C))
            makeChild<CompoundBody>();
    }
};

int main(int argc, char const *argv[]) {
    Engine::init(750,750);
    Engine::getView(0).setCenter(0, 0);
    Engine::makeRoot<Player>();
    Debug::show(true);
    Debug::setGizmoActive(Debug::gizmoId("Physics"), true);
    Engine::run();    
    return 0;
}
