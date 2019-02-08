#include <SFVG/SFVG.hpp>

using namespace sfvg;

class LissaCircle : public GameObject {
public:
    LissaCircle(Engine& engine, Color _color, float _f, bool col) :
        GameObject(engine),
        column(col),
        color(_color),
        f(_f)
    {
        guide = addComponent<LineRenderer>();
        guide->setColor(Grays::Gray10);
        guide->setPointCount(2);
        path = addComponent<LineRenderer>();
        path->setColor(color);
        path->fromShape(CircleShape(40.0f));
        dot = addComponent<ShapeRenderer>();
        dot->shape = CircleShape(5.0f);
        dot->setColor(Color::White);
    }

    void update() {
        dotPos.x = 40.0f * std::cos(0.25f*f*engine.time());
        dotPos.y = 40.0f * std::sin(0.25f*f*engine.time());
        guide->setPoint(0, dotPos);
        if (column)
            guide->setPoint(1,dotPos.x,800);
        else
            guide->setPoint(1,800,dotPos.y);
        dot->shape.setPosition(dotPos);
    }

    Handle<LineRenderer> guide;
    Handle<LineRenderer> path;
    Handle<ShapeRenderer> dot;
    bool column;
    Color color;
    float f;
    Vector2f dotPos;
};

class LissaPath : public GameObject {
public:
    LissaPath(Engine& engine, Handle<LissaCircle> _r, Handle<LissaCircle> _c) :
        GameObject(engine),
        r(_r), c(_c)
    {
        path = addComponent<LineRenderer>();
        path->setColor(Tween::Linear(r->color, c->color, 0.5f));

        dot = addComponent<ShapeRenderer>();
        dot->shape = CircleShape(3.0f);
        dot->setColor(Color::White);
    }

    void update() {
        Vector2f pos;
        pos.y = r->transform.getPosition().y + r->dotPos.y;
        pos.x = c->transform.getPosition().x + c->dotPos.x;
        dot->shape.setPosition(pos);
        if (addPoint)
            path->addPoint(pos);
        addPoint = !addPoint;
    }

    Handle<LissaCircle> r, c;
    Handle<LineRenderer> path;
    Handle<ShapeRenderer> dot;
    bool addPoint = true;
};

class LissaRoot : public GameObject {
public:

    LissaRoot(Engine& engine) :
        GameObject(engine),
        rHeaders(7),
        cHeaders(7),
        nodes(7)
    {
        // frequencies and colors
        std::vector<Color> colors = {
            Reds::FireBrick,
            Oranges::DarkOrange,
            Yellows::Gold,
            Greens::Chartreuse,
            Blues::DeepSkyBlue,
            Pinks::HotPink,
            Purples::Violet
        };

        // make column/row headers
        for (auto& i : range(0,7)) {
            rHeaders[i] = makeChild<LissaCircle>(colors[i], i+1,false);
            rHeaders[i]->transform.setPosition(50.0f, i * 100.0f + 150.0f);
            cHeaders[i] = makeChild<LissaCircle>(colors[i], i+1,true);
            cHeaders[i]->transform.setPosition(i * 100.0f + 150.0f, 50.0f);
        }

        // make nodes
        for (auto& r : range(0,7)) {
            nodes[r].resize(7);
            for (auto& c : range(0,7)) {
                nodes[r][c] = makeChild<LissaPath>(rHeaders[r],cHeaders[c]);
            }
        }

        lr = addComponent<LineRenderer>();
        lr->setColor(Color::Black);
        lr->addPoint(400,400);
    }

    void update() override {

        if (input.getScroll() > 0)
            engine.getView(0).zoom(1.1f);
        else if (input.getScroll() < 0)
            engine.getView(0).zoom(1.0f/1.1f);

        auto drag = input.dragDeltaRaw(MouseButton::Left);
        engine.getView(0).move(-drag.x, -drag.y);
    }

    std::vector<Handle<LissaCircle>> rHeaders;
    std::vector<Handle<LissaCircle>> cHeaders;
    std::vector<std::vector<Handle<LissaCircle>>> nodes;
    Handle<LineRenderer> lr;
};


int main(int argc, char const *argv[])
{
    Engine engine(800,800);
    engine.window.setFramerateLimit(30);
    engine.makeRoot<LissaRoot>();
    engine.run();
    return 0;
}
