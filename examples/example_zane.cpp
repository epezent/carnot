#include <carnot>

using namespace carnot;

class ZaneDemo : public GameObject {

    Handle<ShapeRenderer> sr;
    Ptr<RectangleShape> rect;
    Sequence<Color> grad;

    void start() override {
        rect = make<RectangleShape>();
        rect->setWidth(0);
        rect->setHeight(20);
        sr = addComponent<ShapeRenderer>(rect);
        sr->setColor(Blues::DeepSkyBlue);
        transform.setPosition(250,250);
        grad[0.0f] = Blues::Blue;
        grad[0.5f] = Reds::Red;
        grad[1.0f] = Greens::Green;
    }

    void update() override {
        auto t = Engine::time();
        float w =  200 * sin(t) + 200;
        rect->setWidth(w);
        sr->setColor(grad(0.5f*sin(t)+0.5f));
        
    }

};

int main() {
    Engine::init(500,500, WindowStyle::Close);
    Engine::window->setTitle("TactorFX");
    Engine::makeRoot<ZaneDemo>();
    Engine::run();
}