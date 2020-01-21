#include <carnot>

using namespace carnot;

class DebugTester : public GameObject {
public:

    void update() override {
        Debug::drawCircle(Vector2f(0,0), 100, Reds::FireBrick, true);
        Debug::drawTriangle(Vector2f(-100,0), Vector2f(100,0), Vector2f(0,100), Blues::DeepSkyBlue, true);
        Debug::drawRectangle(Vector2f(0,0), 75,25, withAlpha(Yellows::Gold,0.5f), true);
        StarShape star;
        star.setRadii(5);
        Debug::drawPolygon(star.getVertices(), withAlpha(Greens::Chartreuse,0.5f), true);
    }
};

int main(int argc, char const *argv[])
{
    Engine::init(500,500);
    Engine::getView(0).setCenter(0,0);
    Debug::setFunctionKeysEnabled(false);
    Engine::makeRoot<DebugTester>();
    Debug::show(true);
    Engine::run();
    return 0;
}
