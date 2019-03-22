#include <carnot>

using namespace carnot;

class TriggerTester : public GameObject {
public:

    Handle<Trigger> tr;
    Handle<ShapeRenderer> sr;

    TriggerTester() {
        sr = addComponent<ShapeRenderer>(make<StarShape>());
        sr->setColor(Random::color());
        tr = addComponent<Trigger>(sr->getShape());
        sr->getShape()->setPosition(100,100);
        sr->getShape()->addHole(CircleShape(40));
    }

    void onMouseEnter() {
        sr->setColor(Random::color());
    }

    void onMouseExit() {
        sr->setColor(Random::color());
    }

    void onMouseStay() {
        transform.rotate(90*Engine::deltaTime());
        sr->getShape()->rotate(90*Engine::deltaTime());
    }
    
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<TriggerTester>();
    Engine::run();
    return 0;
}
