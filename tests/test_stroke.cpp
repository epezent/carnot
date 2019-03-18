#include <carnot>

using namespace carnot;

class StrokeTester : public GameObject {
public:

    Handle<StrokeRenderer> sr1;

    StrokeTester() {

        auto bg = addComponent<ShapeRenderer>(make<Shape>(SquareShape(1000)));
        bg->setEffect(make<Checkerboard>(Color::Black, Grays::Gray10, 100));
        bg->showGizmos = false;

        sr1 = addComponent<StrokeRenderer>();
        sr1->setColor(withAlpha(Blues::DeepSkyBlue, 0.5f));
        sr1->addVertex(-400,0);
        sr1->addVertex(-200,0);
        sr1->addVertex(0,0);
        sr1->addVertex(200,0);
        sr1->addVertex(400,0);
        sr1->setThickness(25);
        sr1->setThickness(0,10);
        sr1->setThickness(2,10);
    }

    void update() {
        for (auto i : range(5))
            if (Input::getKeyDown((Key)((std::size_t)Key::Num1 + i)))
                point = i;

        sr1->setPoint(point, Input::getMousePosition());
        sr1->setThickness(point, sr1->getThickness(point) + Input::getScroll());
    }

    std::size_t point = 2;

};

int main() {
    Engine::init(1000,1000);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<StrokeTester>();
    Debug::setGizmoActive(Debug::gizmoId("Wireframe"), true);
    Debug::show(true);
    Engine::run();
}