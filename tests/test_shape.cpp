#include <carnot>

using namespace carnot;

class ShapeTester : public GameObject {
public:

    ShapeTester() {
        auto sr = addComponent<ShapeRenderer>();
        sr->setColor(Reds::Red);
        sr->setShape(SquareShape(100));
        sr->getShape()->setRadii(25);
        // sr->getShape()->addHole(RectangleShape(100,50));
        auto hole2 = RectangleShape(25,50);
        hole2.move(-37.5,0);
        sr->getShape()->addHole(hole2);
        sr->getShape()->addHole(RectangleShape(50,100));
    }
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<ShapeTester>();
    Engine::run();
    return 0;
}
