#include <carnot>
using namespace carnot;

class Wall : public GameObject {
public:
    Wall(float width, float height, float x, float y) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->setShape(make<RectangleShape>(width,height));
        sr->setColor(Color::White);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width,height);
    }
};

class Ball : public GameObject {
public:
    Ball() {
        transform.setPosition(500,250);
        auto sr = addComponent<ShapeRenderer>();
        sr->setShape(make<CircleShape>(10));
        sr->setColor(Reds::FireBrick);
        rb = addComponent<RigidBody>();
        rb->addCircleShape(10);
        reset();
    }

    void update() {
        if (Input::getKeyDown(Key::R))
            reset();
    }

    void reset() {
        transform.setPosition(500,250);
        rb->setVelocity(Math::unit(Vector2f(Random::range(-10.0f,10.0f),Random::range(-2.0f,2.0f)))*750.0f);
    }

    Handle<RigidBody> rb;
};

class Paddle : public GameObject {
public:
    Paddle() {
        auto sr = addComponent<ShapeRenderer>();
        sr->setColor(Color::White);
        sr->setShape(make<RectangleShape>(10,100));
        transform.setPosition(125,250);
        rb = addComponent<RigidBody>(RigidBody::Kinematic);
        rb->addBoxShape(10, 100);
    }

    void update() override {
        if (Input::getKey(Key::Up))
            rb->setPosition(rb->getPosition() + Vector2f(0,-500) * Engine::deltaTime());
        else if (Input::getKey(Key::Down))
            rb->setPosition(rb->getPosition() + Vector2f(0,500) * Engine::deltaTime());
    }

    Handle <RigidBody> rb;
};

class Pong : public GameObject {
public:
    Pong() {
        makeChild<Wall>(10,500,5,250);
        makeChild<Wall>(10,500,995,250);
        makeChild<Wall>(1000,10,500,5);
        makeChild<Wall>(1000,10,500,495);
        ball = makeChild<Ball>();
        paddle = makeChild<Paddle>();
    }

    Handle<Ball> ball;
    Handle<Paddle> paddle;
};

int main(int argc, char const *argv[])
{
    Engine::init(1000,500);
    Engine::makeRoot<Pong>();
    Physics::setGravity(Vector2f(0,0));
    Engine::run();
    
    return 0;
}
