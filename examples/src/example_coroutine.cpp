#include <SFVG/Engine/Object.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <typeinfo>
#include <sstream>
#include <SFVG/Engine/XboxController.hpp>

using namespace sfvg;

class MyObject : public Object {
public:

    Handle<Coroutine> m_coro;

    XboxController m_xbox;

    bool prevPress = false;
    bool currPress = false;
    bool down = false;
    bool up = false;

    MyObject() :
        m_xbox(0)
    {
        m_sqr.setSideLength(50.0f);
        m_sqr.setColor(Reds::FireBrick);
        m_sqr.setPosition(250,250);
        m_font.loadFromFile("../../fonts/Roboto-Bold.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(20);
        m_text.setPosition(5, 5);
        m_text.setFillColor(Whites::White);
    }

    void update() override {

        prevPress = currPress;
        currPress = m_xbox.getButton(XboxController::LB);
        if (currPress != prevPress) {
            up = currPress ? false : true;
            down = currPress ? true : false;
        }


        if (Input::getKeyDown(Key::Num1))
            startCoroutine(Fade());

        if (Input::getKeyDown(Key::Num2))
            startCoroutine(Spin());

        if (Input::getKeyDown(Key::Num3))
            m_coro = startCoroutine(MoveX());

        if (Input::getKeyDown(Key::Num4))
            m_coro = startCoroutine(MoveY());

        if (Input::getKeyDown(Key::Num5))
            m_coro = startCoroutine(Multi());

        if (Input::getKeyDown(Key::Num6))
            m_coro = startCoroutine(Wait());

        if (Input::getKeyDown(Key::Num7))
            m_coro = startCoroutine(Seq());

        if (Input::getKeyDown(Key::S))
            stopCoroutine(m_coro);

        if (Input::getKeyDown(Key::X))
            stopAllCoroutines();

        std::string x = "Coroutines: " + std::to_string(getCoroutineCount());
        if (m_coro.isValid())
            x += "\nHandle<Coroutine> is valid";
        else
            x += "\nHandle<Coroutine> is invalid";
        m_text.setString(x);

        std::ostringstream ss;
        ss << "prev: " << prevPress << std::endl;
        ss << "curr: " << currPress << std::endl;
        ss << "up:   " << up << std::endl;
        ss << "down: " << down << std::endl;


        if (down) {
            m_sqr.setColor(Whites::White);
            print("Down!");
        }
        if (up) {
            m_sqr.setColor(Reds::FireBrick);
            print("Up!");
        }

        up = false;
        down = false;


        m_text.setString(ss.str());

    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_sqr, states);
        target.draw(m_text, states);
    }

    Enumerator MoveX() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            m_sqr.setPosition(250 + 100*std::sin(2.0f * PI * elapsedTime), m_sqr.getPosition().y);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    Enumerator MoveY() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            m_sqr.setPosition(m_sqr.getPosition().x, 250 + 100*std::sin(2.0f * PI * elapsedTime));
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    Enumerator Spin() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            float r = Tween::Smootherstep(0.0f, 720.0f, elapsedTime / 5.0f);
            m_sqr.setRotation(r);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
        m_sqr.setRotation(0.0f);
    }

    Enumerator Fade() {
        float elapsedTime = 0.0f;
        Color from = Reds::FireBrick;
        Color to = Blues::DeepSkyBlue;
        while (elapsedTime < 5.0f) {
            Color tween = Tween::Exponential::Out(from, to, elapsedTime / 5.0f);
            m_sqr.setColor(tween);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
        m_sqr.setColor(to);
    }

    Enumerator Multi() {
        auto x = startCoroutine(MoveX());
        auto y = startCoroutine(MoveY());
        co_yield x;
        co_yield y;
    }

    Enumerator Wait() {
        m_sqr.setRotation(45.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(90.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(135.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(180.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(225.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(270.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(315.0f);
        co_yield new WaitForSeconds(0.1f);
        m_sqr.setRotation(0.0f);
    }

    Enumerator Seq() {
        Sequence<float> seq(Tween::Quadratic::InOut);
        seq[0.00f] = 250;
        seq[0.25f] = 450;
        seq[0.50f] = 250;
        seq[0.75f] = 50;
        seq[1.00f] = 250;
        float elapsedTime = 0.0f;
        while (elapsedTime <= 5.0f) {
            float t = elapsedTime / 5.0f;
            float x = Tween::Linear(50.0f, 450.0f, t);
            float y = seq(t);
            m_sqr.setPosition(x, y);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    SquareShape m_sqr;
    Font m_font;
    Text m_text;
};

int main() {
    Engine engine(500,500);
    engine.setBackgroundColor(Color::Black);
    engine.setWindowTitle("Engine Testing");
    auto root = engine.makeRoot<MyObject>();
    root->setName("root");
    engine.run();
    return 0;
}
