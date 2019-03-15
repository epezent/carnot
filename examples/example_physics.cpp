#include <Carnot/Carnot.hpp>

using namespace carnot;

class Wall : public GameObject {
public:
    Wall(float width, float height, float x, float y) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = make<RectangleShape>(width,height);
        sr->setColor(Color::Black);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width,height);
    }
};

class RectObject : public GameObject {
public:
    RectObject() {
        sr = addComponent<ShapeRenderer>();
        sr->setColor(color = randomColor());
        start = Input::getMousePosition();
    }

    void update() {
        if (!confirmed && Input::getMouseUp(MouseButton::Right)) {
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
        if (!confirmed && Input::getMouse(MouseButton::Right)) {
            end = Input::getMousePosition();
            auto size = absVec(start - end);
            sr->shape = make<RectangleShape>(size.x, size.y);
            transform.setPosition(start + 0.5f * (end - start));
        }
        if (rb) {
            Debug::drawText(str("mass = ",rb->getMass()), transform.getPosition(), Whites::White);
            auto localPos = transform.worldToLocal(Input::getMousePosition());
            if (inBounds(localPos, sr->shape->getLocalBounds()) && Input::getKeyDown(Key::D))
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
    Player() {
        makeChild<Wall>(750,50,0,375);
        makeChild<Wall>(750,50,0,-375);
        makeChild<Wall>(50,750,-375,0);
        makeChild<Wall>(50,750,375,0);
    }

    void update() {
        if (Input::getKeyDown(Key::G))
            Physics::setGravity(Vector2f(0,1000) - Physics::getGravity());
        if (Input::getMouseDown(MouseButton::Right))
            makeChild<RectObject>();
        if (Input::getMouseDown(MouseButton::Middle)) {
            auto go = makeChild<GameObject>();
            go->transform.setPosition(Input::getMousePosition());
            go->addComponent<ShapeRenderer>();
            go->getComponent<ShapeRenderer>()->setColor(randomColor());
            go->getComponent<ShapeRenderer>()->shape = make<CircleShape>(30);
            go->getComponent<ShapeRenderer>()->shape->addHole(StarShape(20,25,20));
            go->addComponent<RigidBody>();
            go->getComponent<RigidBody>()->addShape(go->getComponent<ShapeRenderer>()->shape);
            go->getComponent<RigidBody>()->setShapeDensity(0, 1.0f);
        }
    }
};

int main(int argc, char const *argv[]) {
    Engine::init(750,750);
    Engine::getView(0).setCenter(0, 0);
    Engine::setLayerCount(2);
    Debug::show(true);
    Engine::setBackgroundColor(Grays::Gray10);
    Engine::window->setKeyRepeatEnabled(false);
    Engine::makeRoot<Player>();
    Engine::window->setTitle("Evan's Engine");
    Engine::run();
    
    return 0;
}
