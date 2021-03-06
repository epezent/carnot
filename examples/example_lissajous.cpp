#define CARNOT_USE_DISCRETE_GPU
// #define CARNOT_NO_CONSOLE

#include <carnot>
using namespace carnot;

class LissaCircle : public GameObject {
public:
    LissaCircle(Color _color, float _f, bool col) :
        column(col),
        color(_color),
        f(_f)
    {
        guide = addComponent<LineRenderer>();
        guide->setColor(Grays::Gray20);
        guide->setPointCount(2);
        path = addComponent<LineRenderer>();
        path->setColor(color);
        path->fromShape(CircleShape(40.0f));
        dot = addComponent<ShapeRenderer>();
        dot = makeChild<GameObject>();
        dot->addComponent<ShapeRenderer>();
        dot->getComponent<ShapeRenderer>()->setShape(make<CircleShape>(5.0f));
        dot->getComponent<ShapeRenderer>()->setColor(Color::White);
    }

    void update() {
        dotPos.x = 40.0f * std::cos(0.25f*f*Engine::time());
        dotPos.y = 40.0f * std::sin(0.25f*f*Engine::time());
        guide->setPoint(0, dotPos);
        if (column)
            guide->setPoint(1,dotPos.x,800);
        else
            guide->setPoint(1,800,dotPos.y);
        dot->transform.setLocalPosition(dotPos);
    }

    Handle<LineRenderer> guide;
    Handle<LineRenderer> path;
    Handle<GameObject> dot;
    bool column;
    Color color;
    float f;
    Vector2f dotPos;
};

class LissaPath : public GameObject {
public:
    LissaPath(Handle<LissaCircle> _r, Handle<LissaCircle> _c) :
        r(_r), c(_c)
    {
        path = addComponent<LineRenderer>();
        path->setColor(Tween::Linear(r->color, c->color, 0.5f));

        dot = makeChild<GameObject>();
        dot->addComponent<ShapeRenderer>();
        dot->getComponent<ShapeRenderer>()->setShape(make<CircleShape>(3.0f));
        dot->getComponent<ShapeRenderer>()->setColor(Color::White);
    }

    void update() {
        Vector2f pos;
        pos.y = r->transform.getPosition().y + r->dotPos.y;
        pos.x = c->transform.getPosition().x + c->dotPos.x;
        dot->transform.setPosition(pos);
        path->addPoint(pos);
        if (Input::getKeyDown(Key::Space))
            path->setPointCount(0);
    }

    Handle<LissaCircle> r, c;
    Handle<LineRenderer> path;
    Handle<GameObject> dot;
};

class LissaRoot : public GameObject {
public:

    LissaRoot() :
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
        for (int i = 0; i < 7; ++i) {
            rHeaders[i] = makeChild<LissaCircle>(colors[i], i+1,false);
            rHeaders[i]->transform.setPosition(50.0f, i * 100.0f + 150.0f);
            cHeaders[i] = makeChild<LissaCircle>(colors[i], i+1,true);
            cHeaders[i]->transform.setPosition(i * 100.0f + 150.0f, 50.0f);
        }

        // make nodes
        for (int r = 0; r < 7; ++r) {
            nodes[r].resize(7);
            for (int c = 0; c < 7; ++c) {
                nodes[r][c] = makeChild<LissaPath>(rHeaders[r],cHeaders[c]);
            }
        }

        lr = addComponent<LineRenderer>();
        lr->setColor(Color::Black);
        lr->addPoint(400,400);
    }

    std::vector<Handle<LissaCircle>> rHeaders;
    std::vector<Handle<LissaCircle>> cHeaders;
    std::vector<std::vector<Handle<LissaPath>>> nodes;
    Handle<LineRenderer> lr;
};


int main(int argc, char const *argv[])
{
    Engine::init(800,800, WindowStyle::Default);
    Engine::window->setTitle("Lissajous");
    Engine::makeRoot<LissaRoot>();
    Engine::run();
    
    return 0;
}
