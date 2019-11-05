#include <carnot>

using namespace carnot;

class ShapeDemo : public GameObject {

    void addShape(Ptr<Shape> shape, float x, float y) {
        auto go = makeChild<GameObject>();
        go->transform.setPosition(x,y);
        auto sr = go->addComponent<ShapeRenderer>(shape);
        sr->setColor(Random::color());
    }

    void start() override {
        // basic shapes
        addShape(make<SquareShape>(80), 100,100);
        addShape(make<RectangleShape>(40,80), 200,100);
        addShape(make<CircleShape>(40),300,100);
        addShape(make<PolygonShape>(6, PolygonShape::CircumscribedRadius, 40),400,100);
        addShape(make<StarShape>(5,20,40), 100, 200);
        addShape(make<CrossShape>(80,80,20), 200, 200);

        // rounding
        auto sqr = make<SquareShape>(80);
        sqr->setRadii(10);
        sqr->setRadius(0, 20);
        sqr->setRadius(2, 20);
        addShape(sqr, 100, 300);

        

    }

};

int main(int argc, char* argv[]) {

    Engine::init(500,500,"Shapes Demo");
    Engine::makeRoot<ShapeDemo>();
    Engine::run();

    return 0;
}





