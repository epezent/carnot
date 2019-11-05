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
        rb->addBoxShape(width,height,1.0f, 0.0f, 1.0f);
    }
};

class Ball : public GameObject {
public:
    Ball() {
        transform.setPosition(500,250);
        auto sr = addComponent<ShapeRenderer>();
        sr->setShape(make<CircleShape>(10));
        sr->setColor(Whites::White);
        rb = addComponent<RigidBody>();
        rb->addCircleShape(10, Vector2f(), 1.0f, 0.1f, 1.0f);
        rb->setLinearDamping(0);
        rb->setAngularDamping(0);
        reset();
    }

    void update() {
        if (Input::getKeyDown(Key::R))
            reset();
    }

    void reset() {
        rb->setPosition(500,250);
        rb->setVelocity(Math::unit(Vector2f(Random::range(-10.0f,10.0f),Random::range(-2.0f,2.0f)))*750.0f);
    }

    Handle<RigidBody> rb;
};

class Paddle : public GameObject {
public:

    Paddle(float x, const Color& color, float _speed) : speed(_speed) {
        auto sr = addComponent<ShapeRenderer>();
        sr->setColor(color);
        sr->setShape(make<RectangleShape>(10,100));
        transform.setPosition(x,250);
        rb = addComponent<RigidBody>(RigidBody::Kinematic);
        rb->addBoxShape(10, 100, 1, 0.1f, 1.0f);
    }

    void move(float y) {
        rb->setPosition(rb->getPosition() + Vector2f(0, y));
    }

    float speed;

    Handle <RigidBody> rb;

};



class Player : public Paddle {
public:

    using Paddle::Paddle;

    void update() override {
        if (Input::getKey(Key::Up))
            move(-speed * Engine::deltaTime());
        if (Input::getKey(Key::Down))
            move(speed * Engine::deltaTime());
    }
};

class Agent : public Paddle {
public:

    using Paddle::Paddle;

    Handle<Ball> ball;

    void start() override {
        ball = findSibling<Ball>();
    }

    void update() override  {
        if (ball->rb->getVelocity().x > 0) {
            float ballY = ball->transform.getPosition().y;
            float thisY = this->transform.getPosition().y;
            float delta = ballY - thisY;
            if (Math::abs(delta) > 20) {
                int dir = Math::sign(delta);
                move(dir * speed * Engine::deltaTime());
            }
        }
    }
};

class Pong : public GameObject {
public:
    Pong() {
        makeChild<Wall>(10,500,5,250);
        makeChild<Wall>(10,500,995,250);
        makeChild<Wall>(1000,10,500,5);
        makeChild<Wall>(1000,10,500,495);
        makeChild<Ball>();
        makeChild<Player>(100, Blues::DeepSkyBlue, 500);
        makeChild<Agent>(900,   Reds::FireBrick,   250);
    }
};

int main(int argc, char const *argv[])
{
    Engine::init(1000,500, "Pong");
    Engine::makeRoot<Pong>();
    Physics::setGravity(Vector2f(0,0));
    Engine::run();
    
    return 0;
}
