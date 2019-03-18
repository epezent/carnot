#include <carnot>

using namespace carnot;

class EffectDemo : public GameObject {
public:

    Ptr<Gradient> gr;   
    Ptr<Checkerboard> ch;

    EffectDemo() {    
        gr = make<Gradient>();
        for (std::size_t i = 0; i < 6; ++i) {
            gr->setColor((float)i/5.0f, Random::color());
        }
        ch = make<Checkerboard>(RGB{0.05f,0.05f,0.05f,1.0f}, Grays::Black, 50, 1920.0f/1080.0f);
        auto sr = addComponent<ShapeRenderer>(make<RectangleShape>(1920,1080));
        sr->setEffect(ch);
    }

    void update() {
        gr->angle = Math::angle(Input::getMousePosition()) * Math::RAD2DEG; 
        if (Input::getKeyDown(Key::Space))
            gr->setColor(1.0f, Random::color());
        Debug::drawLine(Vector2f(), Input::getMousePosition(), Color::White);
    }
};

int main(int argc, char const *argv[])
{
    Engine::init("Effects Demo");
    Engine::getView(0).setCenter(0,0);
    Engine::makeRoot<EffectDemo>();
    Engine::run();
    return 0;
}
