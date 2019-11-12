#include <carnot>

using namespace carnot;

class Funnel : public GameObject {
public:

    float gapWidth = 50;
    float height   = 200;
    float y        = 100;

    Funnel() {
        auto left  = makeChild<GameObject>();
        left->addComponent<RigidBody>(RigidBody::BodyType::Static);
        auto shape = make<Shape>();
        // shape->addPoint()


        // auto right = makeChild<GameObject>();

    }

};

int main(int argc, char const *argv[])
{
    Engine::init(400,800,WindowStyle::Close,"Galton Board");
    Engine::getView(0).setCenter(200,400);
    Engine::makeRoot<GameObject>();
    Engine::run();
    return 0;
}
