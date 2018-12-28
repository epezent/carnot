#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Graphics.hpp>

using namespace sfvg;

class MyObject : public Object {
public:

    MyObject() {
        m_circle.setCircleRadius(50);
        m_circle.setColor(Reds::FireBrick);
    }

    void update() override {

        if (Input::getKeyDown(Key::C)) {
            startCoroutine(ColorCoro());
        }

        if (Input::getKeyDown(Key::X)) {
            stopAllCoroutines();
        }

        if (Input::getKeyDown(Key::M)) {
            startCoroutine(SinCoro());
        }

        if (Input::getKeyDown(Key::H)) {
            print(getCoroutineCount());
        }

        if (Input::getKeyDown(Key::B)) {
            startCoroutine(Blink(100));
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_circle, states);
    }

    Enumerator SinCoro() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            float positionY = 250 + std::sin(2.0f * PI * elapsedTime) * 100.0f;
            float positionX = getPosition().x;
            setPosition(positionX, positionY);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    Enumerator Blink(int k) {
        for (int i = 0; i < k; ++i) {
            if (isEven(i))
                m_circle.setColor(Whites::White);
            else
                m_circle.setColor(Reds::Red);
            co_yield new WaitForSeconds(0.05f);
        }
    }

    Enumerator ColorCoro() {
        float elapsedTime = 0.0f;
        Color from = Reds::FireBrick;
        Color to = Blues::DeepSkyBlue;
        while (elapsedTime < 2.0f) {
            Color tween = Tween::Linear(from, to, elapsedTime / 2.0f);
            m_circle.setColor(tween);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
        m_circle.setColor(to);
        co_yield new WaitForSeconds(2.0f);
        m_circle.setColor(Greens::Chartreuse);
        co_yield new WaitForSeconds(1.0f);
        m_circle.setColor(Yellows::Gold);
        co_yield startCoroutine(Blink(100));
    }
    CircleShape m_circle;
};

struct Blob {
    Blob() {print("Made Blob"); }
    ~Blob() {print("Destroyed Blob");}
};

void test(Ptr<Blob> blob) {

}

int test2() {
    return 1;
}

int main() {

    Engine engine;
    engine.setWindowSize(Vector2f(500, 500));
    engine.setBackgroundColor(Color::Black);
    engine.setWindowTitle("Engine Testing");

    auto root = engine.makeRoot<MyObject>();
    root->setRotation(45);
    root->setPosition(250, 250);
    root->setName("root");

    engine.run();

    return 0;
}
