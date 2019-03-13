#include <SFVG/SFVG.hpp>

using namespace sfvg;

const float SIDE_LENGTH =  50.0f;
const float GRID_SIZE   = (2.0f+sqrt(2.0f))*0.5f*SIDE_LENGTH*0.999f;

class Piece : public GameObject {
public:

    Handle<ShapeRenderer> sr;

    Piece() {
        sr = addComponent<ShapeRenderer>();
        *sr->shape = makeShape();
    }

    Shape makeShape() {
        // make primivites
        auto oct = PolygonShape(8, PolygonShape::SideLength, SIDE_LENGTH);
        oct.rotate(360.0f/16.0f);
        oct.applyTransform();
        auto sqr = SquareShape(SIDE_LENGTH);

        Shape shape;
        sqr.move(GRID_SIZE, 0);
        sqr.applyTransform();

        shape = Shape::clipShapes(oct, sqr, Shape::Union)[0];
        oct.move(2 * GRID_SIZE, 0);
        oct.applyTransform();
        shape = Shape::clipShapes(shape, oct, Shape::Union)[0];
        oct.move(-GRID_SIZE, GRID_SIZE);
        oct.applyTransform();
        shape = Shape::clipShapes(shape, oct,  Shape::Union)[0];
        sqr.move(GRID_SIZE,GRID_SIZE);
        sqr.applyTransform();
        shape = Shape::clipShapes(shape, sqr, Shape::Union)[0];
        
        if (shape.getHoleCount()) {
            shape.setHoleCount(0);
        }

        auto offset = Shape::offsetShape(shape,-2.0f);

        return offset;
    }

    void update() {
        auto localPos = transform.worldToLocal(Input::getMousePosition());
        if (sr->shape->isInside(localPos)) {
            sr->setColor(Blues::LightBlue);
            transform.move(Input::dragDelta(MouseButton::Left));
        }
        else {
            auto c = Blues::DeepSkyBlue;
            c.a = 128;
            sr->setColor(c);
        }

        auto points = sr->shape->getPoints();
        for (std::size_t i = 0; i < points.size(); ++i) {
            if (i > 0 && points[i] == points[i-1])
                print("Shit!");
            Debug::drawText(str(i),transform.localToWorld(points[i]),Whites::White);
        }

    };

};

class Puzz : public GameObject {
public:

    Handle<ShapeRenderer> sr;

    Puzz() {
        makeChild<Piece>()->transform.setPosition(0,0);
        makeChild<Piece>()->transform.setPosition(0, 2*GRID_SIZE);        
    }

};

int main(int argc, char const *argv[])
{
    Engine::init(500,500);
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<Puzz>();
    Engine::run();
    return 0;
}
