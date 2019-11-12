#include <carnot>

using namespace carnot;

/// Ball
class Ball : public GameObject {
public:
    Ball() {
        transform.setPosition(500, 250);
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
        startCoroutine(resetCoro());
    }

    Enumerator resetCoro() {
        rb->setPosition(500, 250);
        rb->setVelocity(0,0);
        co_yield new WaitForSeconds(1);
        rb->setVelocity(Math::unit(Vector2f(Random::range(-10.0f, 10.0f), Random::range(-2.0f, 2.0f))) * 750.0f);
    }

    Handle<RigidBody> rb;
};


/// Paddle
class Paddle : public GameObject {
public:
    Paddle(float x, const Color &color, float _speed) : speed(_speed) {

        auto shape = make<Shape>();
        shape->addPoint(-5,-50);
        shape->addPoint(5,-50);
        shape->addPoint(10,0);
        shape->addPoint(5,50);
        shape->addPoint(-5,50);
        shape->addPoint(-10,0);

        auto sr = addComponent<ShapeRenderer>();
        sr->setColor(color);
        sr->setShape(shape);
        transform.setPosition(x, 250);
        rb = addComponent<RigidBody>(RigidBody::Kinematic);

        rb->addShape(shape, 1, 0.1f, 1.0f);
    }

    void setY(float y) {
        rb->setPosition(rb->getPosition() + Vector2f(0, y));
    }

    void scorePoint() {
        onScore.emit(++score);
    }

    int score = 0;
    float speed;
    Handle<RigidBody> rb;
    Signal<void(int)> onScore;
};

/// Player controlled paddle
class Player : public Paddle {
public:
    using Paddle::Paddle;

    void update() override {
        if (Input::getKey(Key::Up))
            setY(-speed * Engine::deltaTime());
        if (Input::getKey(Key::Down))
            setY(speed * Engine::deltaTime());
    }
};

/// AI controlled paddle
class Agent : public Paddle {
public:
    using Paddle::Paddle;

    void start() override {
        ball = findSibling<Ball>();
    }

    void update() override {
        bool ballToRight = ball->transform.getPosition().x > transform.getPosition().x;
        bool movingToard = ballToRight ? ball->rb->getVelocity().x <= 0 : ball->rb->getVelocity().x >= 0;
        if (movingToard) {
            float ballY = ball->transform.getPosition().y;
            float thisY = this->transform.getPosition().y;
            float delta = ballY - thisY;
            if (Math::abs(delta) > 20) {
                int dir = Math::sign(delta);
                setY(dir * speed * Engine::deltaTime());
            }
        }
    }

    Handle<Ball> ball;
};

/// Outer walls
class Wall : public GameObject {
public:
    Wall(float width, float height, float x, float y) {
        transform.setLocalPosition(x, y);
        auto sr = addComponent<ShapeRenderer>();
        sr->setShape(make<RectangleShape>(width, height));
        sr->setColor(Color::White);
        auto rb = addComponent<RigidBody>(RigidBody::Static);
        rb->addBoxShape(width, height, 1.0f, 0.0f, 1.0f);
    }
};

/// Left and right walls which register points
class Goal : public Wall {
public:
    Goal(Handle<Paddle> offender, Handle<Ball> ball, float width, float height, float x, float y) : Wall(width, height, x, y) {
        getComponent<RigidBody>()->onCollisionEnter.connect([=](Collision col) {
            offender->scorePoint();
            ball->reset();
        });
    }
};

/// The master GameObject
class Pong : public GameObject {
public:
    Pong()  {
        auto player = makeChild<Player>(100, Blues::DeepSkyBlue, 500);
        auto agent  = makeChild<Agent>(900, Reds::FireBrick, 250);
        auto ball   = makeChild<Ball>();

        makeChild<Goal>(agent, ball, 10, 500, 5, 250);
        makeChild<Goal>(player, ball, 10, 500, 995, 250);
        makeChild<Wall>(1000, 10, 500, 5);
        makeChild<Wall>(1000, 10, 500, 495);

        auto pScore = addComponent<TextRenderer>();
        pScore->text.setCharacterSize(20);
        pScore->text.setPosition(20, 20);
        pScore->text.setString("0");
        alignTopLeft(pScore->text);

        auto aScore = addComponent<TextRenderer>();
        aScore->text.setCharacterSize(20);
        aScore->text.setPosition(980, 20);
        aScore->text.setString("0");
        alignTopRight(aScore->text);

        player->onScore.connect([=](int s) {
            pScore->text.setString(str(s));
            alignTopLeft(pScore->text);
        });

        agent->onScore.connect([=](int s) {
            aScore->text.setString(str(s));
            alignTopRight(aScore->text);
        });
    }
};

int main(int argc, char const *argv[]) {
    Engine::init(1000, 500, WindowStyle::Close, "Pong");
    Engine::makeRoot<Pong>();
    Physics::setGravity(Vector2f(0, 0));
    Engine::run();
    return 0;
}
