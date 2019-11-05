#include <carnot>

using namespace carnot;

class SequenceDemo : public GameObject {

    Handle<ShapeRenderer> sr;

    Sequence<Color> gradient;
    Sequence<Vector2f> position;
    Sequence<float> rotation;

    void start() override {
        sr = addComponent<ShapeRenderer>(make<SquareShape>(50));     

        gradient[0.00f] = Blues::DeepSkyBlue;
        gradient[0.25f] = Reds::FireBrick;
        gradient[0.50f] = Yellows::Gold;
        gradient[0.75f] = Greens::Chartreuse;
        gradient[1.00f] = Blues::DeepSkyBlue;
        position.setTween(Tween::Smoothstep);

        position[0.00f] = Vector2f(100,100);
        position[0.25f] = Vector2f(400,100);
        position[0.50f] = Vector2f(400,400);
        position[0.75f] = Vector2f(100,400);
        position[1.00f] = Vector2f(100,100);
        position.setTween(Tween::Bounce::Out);

        rotation[0.00f] = 0;
        rotation[0.25f] = 90;
        rotation[0.50f] = 180;
        rotation[0.75f] = 90;
        rotation[1.00f] = 0;
        rotation.setTween(Tween::Exponential::Out);
    }

    void update() override {
        auto t = Math::fmod(Engine::time(), 10.0f) / 10.0f;
        sr->setColor(gradient(t));
        transform.setPosition(position(t));
        transform.setRotation(rotation(t));   
    }

};

int main() {
    Engine::init(500,500, WindowStyle::Close, "Sequences Demo");
    Engine::makeRoot<SequenceDemo>();;
    Engine::run();
}