#include <carnot>

using namespace carnot;

class Beans : public GameObject {
public:

    Beans() {
        system = addComponent<ParticleSystem>();
    }

    Handle<ParticleSystem> system;
};

class Funnel : public GameObject {
public:

    float gapWidth = 50;
    float height   = 200;
    float y        = 100;

    Funnel() {
        auto go = makeChild<GameObject>();
        go->transform.setPosition(0,400);
        auto rb = go->addComponent<RigidBody>(RigidBody::BodyType::Static);
        rb->addBoxShape(400,20);

        go = makeChild<GameObject>();
        go->transform.setPosition(-200,0);
        rb = go->addComponent<RigidBody>(RigidBody::BodyType::Static);
        rb->addBoxShape(20,800);

        go = makeChild<GameObject>();
        go->transform.setPosition(200,0);
        rb = go->addComponent<RigidBody>(RigidBody::BodyType::Static);
        rb->addBoxShape(20,800);

    }

};

int main(int argc, char const *argv[])
{
    Engine::init(400,800,WindowStyle::Close,"Galton Board");
    Engine::getView(0).setCenter(0,0);
    Debug::show(true);
    Debug::setGizmoActive(Debug::gizmoId("Physics"), true);
    auto root = Engine::makeRoot<GameObject>();
    root->makeChild<Funnel>();
    root->makeChild<Beans>();
    Engine::run();
    return 0;
}
