#include <SFVG/SFVG.hpp>

using namespace sfvg;

class Wall : public GameObject {
public:
    Wall(Engine& engine, float width, float height, float x, float y) : GameObject(engine) {
        transform.setLocalPosition(x,y);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = RectangleShape(width,height);
        sr->setColor(Color::White);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width,height);
        rb->setShapeElasticity(0, 1.0f);
    }
};

class Ball : public GameObject {
public:
    Ball(Engine& e) : GameObject(e) {
        transform.setPosition(500,250);
        auto sr = addComponent<ShapeRenderer>();
        sr->shape = CircleShape(10);
        sr->setColor(Reds::FireBrick);
        rb = addComponent<RigidBody>();
        rb->addCircleShape(10);
        rb->setShapeMass(0, 10.0f);
        rb->setShapeElasticity(0, 1.0f);
        reset();
    }

    void update() {
        if (input.getKeyDown(Key::R))
            reset();
    }

    void reset() {
        transform.setPosition(500,250);
        rb->setVelocity(unit(Vector2f(random(-10.0f,10.0f),random(-2.0f,2.0f)))*750.0f);
    }

    Handle<RigidBody> rb;
};

class Paddle : public GameObject {
public:
    Paddle(Engine& e) : GameObject(e) {
        auto sr = addComponent<ShapeRenderer>();
        sr->setColor(Color::White);
        sr->shape = RectangleShape(10,100);
        transform.setPosition(125,250);
        rb = addComponent<RigidBody>(RigidBody::Kinematic);
        rb->addBoxShape(10, 100);
        rb->setShapeElasticity(0, 1.0f);
    }

    void update() override {
        if (input.getKey(Key::Up))
            rb->setPosition(rb->getPosition() + Vector2f(0,-500) * engine.deltaTime());
        else if (input.getKey(Key::Down))
            rb->setPosition(rb->getPosition() + Vector2f(0,500) * engine.deltaTime());
    }

    Handle <RigidBody> rb;
};

class Pong : public GameObject {
public:
    Pong(Engine& e) : GameObject(e) {
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
    Engine engine(1000,500);
    engine.makeRoot<Pong>();
    engine.physics.setGravity(Vector2f(0,0));
    engine.physics.setDamping(1);
    engine.run();
    return 0;
}
