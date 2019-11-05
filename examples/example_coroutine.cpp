#include <carnot>

using namespace carnot;

class CoroutineDemo : public GameObject {
public:

    Handle<Coroutine> m_coro;
    Handle<ShapeRenderer> sr;
    Ptr<SquareShape> sqr;

    void start() override {
        sr = addComponent<ShapeRenderer>();
        sqr = make<SquareShape>(50);
        sr->setShape(sqr);
        sr->setColor(Reds::FireBrick);
        transform.setPosition(250,250);
    }

    void update() override {

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
    }


    Enumerator MoveX() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            transform.setPosition(250 + 100*std::sin(2.0f * Math::PI * elapsedTime), transform.getPosition().y);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    Enumerator MoveY() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            transform.setPosition(transform.getPosition().x, 250 + 100*std::sin(2.0f * Math::PI * elapsedTime));
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    Enumerator Spin() {
        float elapsedTime = 0.0f;
        while (elapsedTime < 5.0f) {
            float r = Tween::Smootherstep(0.0f, 720.0f, elapsedTime / 5.0f);
            transform.setRotation(r);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
        transform.setRotation(0.0f);
    }

    Enumerator Fade() {
        float elapsedTime = 0.0f;
        Color from = Reds::FireBrick;
        Color to = Blues::DeepSkyBlue;
        while (elapsedTime < 5.0f) {
            Color tween = Tween::Exponential::Out(from, to, elapsedTime / 5.0f);
            sr->setColor(tween);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
        sr->setColor(to);
    }

    Enumerator Multi() {
        co_yield startCoroutine(MoveX());
        co_yield startCoroutine(MoveY());
    }

    Enumerator Wait() {
        transform.setRotation(45.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(90.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(135.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(180.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(225.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(270.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(315.0f);
        co_yield new WaitForSeconds(0.1f);
        transform.setRotation(0.0f);
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
            transform.setPosition(x, y);
            elapsedTime += Engine::deltaTime();
            co_yield nullptr;
        }
    }

    SquareShape m_sqr;
    Font m_font;
    Text m_text;
};

int main() {

    Engine::init(500,500, "Coroutines");
    Engine::setBackgroundColor(Color::Black);
    Engine::makeRoot<CoroutineDemo>();
    Engine::run();
    return 0;
}
