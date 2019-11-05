#include <carnot>

using namespace carnot;

class TriggerExample : public GameObject {
public:

    Handle<Trigger> tr;
    Handle<ShapeRenderer> sr;

    TriggerExample() {
        sr = addComponent<ShapeRenderer>(make<StarShape>());
        sr->setColor(Color::White);
        tr = addComponent<Trigger>(sr->getShape());
        sr->getShape()->addHole(CircleShape(25));
    }

    void onMouseEnter() {
        sr->setColor(Random::color());
    }

    void onMouseExit() {
        sr->setColor(Color::White);
    }

    void onMouseStay() {
        transform.rotate(90*Engine::deltaTime());
    }
    
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500,"Trigger Example");
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<TriggerExample>();
    Engine::run();
    return 0;
}
