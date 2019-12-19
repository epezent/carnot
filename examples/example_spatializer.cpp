#include <carnot>

using namespace carnot;

class Node : public GameObject {
public:
    Handle<ShapeRenderer> sr;
    Ptr<CircleShape> circle;
    bool held = false;

    Node(float radius) {
        circle = make<CircleShape>(radius);
        sr = addComponent<ShapeRenderer>(circle);
        sr->setColor(Grays::Gray10);
    }
};

class Grid : public GameObject {
public:
    std::vector<Handle<Node>> nodes;
    float zoneRadius;
    float nodeRadius;
    Handle<ShapeRenderer> sr;
    Ptr<CircleShape> circle;
    Handle<GameObject> zone;

    Grid(int n, float size, float zoneRadius, float nodeRadius) : zoneRadius(zoneRadius), nodeRadius(nodeRadius) {
        float spacing = size / (n-1);
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                auto node = makeChild<Node>(nodeRadius);
                node->transform.setPosition(c * spacing - size / 2, r * spacing- size / 2);
                nodes.push_back(node);
            }
        }
    }

    void start() {
        zone = makeChild<GameObject>();
        circle = make<CircleShape>(zoneRadius);
        sr = zone->addComponent<ShapeRenderer>(circle);
        sr->setColor(withAlpha(Greens::Chartreuse, 0.1f));
    }

    void update() override {
        auto mouse = Input::getMousePosition();
        for (auto& node : nodes) {
            float d = Math::magnitude(node->transform.getPosition() - mouse);
            float t = Math::clamp01(d / zoneRadius);
            node->sr->setColor(Tween::Linear(Greens::Chartreuse, Grays::Gray20, t));
            float s = Tween::Linear(2.0f,1.0f,t);
            node->transform.setLocalScale(s,s);
            if (d < s * nodeRadius || node->held) {
                if (node->held)
                    node->transform.move(Input::dragDelta(MouseButton::Left));
                if (Input::dragStarted(MouseButton::Left))
                    node->held = true;
                if (Input::dragEnded(MouseButton::Left))
                    node->held = false;
                node->sr->setColor(Greens::GreenYellow);
            }
        }

        zoneRadius += Input::getScroll();
        circle->setCircleRadius(zoneRadius);
        zone->transform.setPosition(Input::getMousePosition());
    }
};

int main(int argc, char const *argv[])
{
    Engine::init(550,550,"Spatializer Demo");
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<Grid>(5,350, 150, 10);
    Engine::run();
    return 0;
}

