#include <carnot>

using namespace carnot;

class TriggerDemo : public GameObject {
public:

    Handle<Trigger> tr;
    Handle<ShapeRenderer> sr;

    TriggerDemo() {
        sr = addComponent<ShapeRenderer>(make<StarShape>());
        sr->setColor(Color::White);
        sr->getShape()->addHole(CircleShape(25));
        tr = addComponent<Trigger>(sr->getShape());

        tr->onMouseEnter.connect(this, &TriggerDemo::randomizeColor);    // you can connect a member function
        tr->onMouseExit.connect([=](){ sr->setColor(Whites::White);}); // or a lambda
        tr->onMouseStay.connect([=](){ transform.rotate(90*Engine::deltaTime());} );

    }

    void randomizeColor() {
        sr->setColor(Random::color());
    }
    
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500,"Trigger Example");
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<TriggerDemo>();
    Engine::run();
    return 0;
}
