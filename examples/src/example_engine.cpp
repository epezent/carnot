#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk/chipmunk.h>

using namespace sfvg;

class RigidBody;

class Collider;

cpSpace *g_space;
float g_dt = 1.0f / 60.0f;

class RigidBody : public Component {
public:

    enum BodyType {
        Dynamic,
        Kinematic,
        Static
    };

    RigidBody() {
        auto moment = cpMomentForBox(1.0f, 100, 100);
        m_body = cpSpaceAddBody(g_space, cpBodyNew(1.0f, moment));
    }

    ~RigidBody() {
        cpBodyFree(m_body);
    }

    void start() override {
        print("RigidBody added");
    }

    void update() override {
        auto position = cpBodyGetPosition(m_body);
        auto angle = cpBodyGetAngle(m_body);
        object()->transform.setPosition((float) position.x, (float) position.y);
        object()->transform.setRotation((float) angle * RAD2DEG);
    }

    void onComponentAdded(Handle<Component> component) override {
        auto candidate = component.as<Collider>();
        if (candidate) {
            print("RigidBody connected with Collider");
            m_collider = candidate;
        }
    }

    void applyForceToCenter(float forceX, float forceY) {
        auto pos = cpBodyGetPosition(m_body);
        cpBodyApplyForceAtWorldPoint(m_body, cpv(forceX, forceY), cpv(pos.x, pos.y));
    }

    void applyTorqueToCenter(float torque) {
        torque += (float) cpBodyGetTorque(m_body);
        cpBodySetTorque(m_body, (double) torque);
    }

private:
    Handle<Collider> m_collider;
    cpBody *m_body;
};

class Collider : public Component {
public:

    void start() override {
        //print("Collider added");
    }

    void onComponentAdded(Handle<Component> component) override {
        auto candidate = component.as<RigidBody>();
        if (candidate) {
            print("Collider connected with RigidBody");
            m_rigidbody = candidate;
        }
    }

private:
    Handle<RigidBody> m_rigidbody;
};

class BoxCollider : public Collider {
public:

};

class Renderer : public Component {
public:

    void start() override {
        print("Renderer added");
    }

};

class MyObject : public Object {
public:

    MyObject(Color color) {
        sqr.setColor(color);
        sqr.setSideLength(100);
    }

    void start() override {
        rb = addComponent<RigidBody>();
    }

    void update() override {
        if (Input::getKey(Key::C))
            addComponent<Collider>();
        if (Input::getKeyDown(Key::D))
            addComponent<Renderer>();
        if (Input::getKeyDown(Key::B))
            addComponent<BoxCollider>();

        if (Input::getKey(Key::Up))
            rb->applyForceToCenter(0, -3000);
        if (Input::getKey(Key::Right))
            rb->applyTorqueToCenter(1000);

    }

    void draw(RenderTarget &target, RenderStates states) const override {
        target.draw(sqr, states);
    }

private:
    SquareShape sqr;
    Handle<RigidBody> rb;
};

class World : public Object {
public:

    World() {
        g_space = cpSpaceNew();
        cpSpaceSetIterations(g_space, 10);
        cpSpaceSetSleepTimeThreshold(g_space, 0.5f);
        cpSpaceSetCollisionSlop(g_space, 0.5f);
        cpSpaceSetGravity(g_space, cpv(0, 1000));
    }

    void update() override {
        cpSpaceStep(g_space, g_dt);
    }

private:

};

int main(int argc, char const *argv[]) {
    Engine engine(500, 1000);
    engine.textures.load("paper", "../textures/paper.png");
    engine.textures.get("paper").setSmooth(true);
    engine.window.setKeyRepeatEnabled(false);
    Sprite sprite;
    sprite.setTexture(engine.textures.get("paper"));
    auto world = engine.makeRoot<World>();
    world->makeChild<MyObject>(Greens::Chartreuse);
    engine.getView(0).setCenter(0, 500);
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
