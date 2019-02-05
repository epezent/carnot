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

    void reset() {
        rb->setPosition(500,250);
        rb->setVelocity(random(-1000,1000),random(-250,250));
    }

    Handle<RigidBody> rb;
    Handle<RigidBody> sr;
};

class Pong : public GameObject {
public:
    Pong(Engine& e) : GameObject(e) {
        makeChild<Wall>(10,500,5,250);
        makeChild<Wall>(10,500,995,250);
        makeChild<Wall>(1000,10,500,5);
        makeChild<Wall>(1000,10,500,495);
        ball = makeChild<Ball>();
    }

    Handle<Ball> ball;
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
