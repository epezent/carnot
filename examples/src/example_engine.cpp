#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>
#include <chipmunk.h>

using namespace sfvg;

class TestComp : public Component {
public:
    TestComp(GameObject& go) : Component(go) { }
    void update() override {
        if (clock.getElapsedTime() > seconds(0.5f))
        {
            print("Ping");
            clock.restart();
        }
    }
    Clock clock;
};

class CircleObject : public GameObject {
public:
    CircleObject(Engine& engine, Color color) :  GameObject(engine)
    {
        cir.setCircleRadius(50);
        cir.setColor(color);
        transform.setPosition(250, 250);
        text.setFont(engine.fonts.get("Roboto"));
        text.setFillColor(Whites::White);
        text.setString(getName());
        text.setCharacterSize(15);
        alignCenter(text);
    }

    void update() override {
        if (input.getKeyDown(Key::E) && isEnabled())
            print("Enabled!");
        if (input.getKeyDown(Key::A) && isEnabled())
            print("Active!");
        if (input.getDoubleClick(MouseButton::Left))
            cir.setColor(randomColor());
        if (input.getKeyDown(Key::S))
            startCoroutine(coro());

    }

    Enumerator coro() {
        float elapseTime = 0.0f;
        transform.setPosition(100, 100);
        while (elapseTime < 2.0f) {
            auto newPosition = Tween::Smoothstep(Vector2f(100,100), Vector2f(400,400), elapseTime / 2.0f);
            transform.setPosition(newPosition);
            elapseTime += engine.deltaTime();
            co_yield nullptr;
        }
        transform.setPosition(400, 400);
    }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(cir, states);
        target.draw(text, states);
    }

private:

    CircleShape cir;
    Text text;
};

class SquareObject : public GameObject {
public:

    SquareObject(Engine& engine, Color color) :
        GameObject(engine)
    {
        sqr.setSideLength(100);
        sqr.setColor(color);
        transform.setPosition(250,250);
    }

    void update() override {

    }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(sqr, states);
    }

private:

    SquareShape sqr;

};

int main(int argc, char const *argv[]) {
    Engine engine(500, 500);
    engine.window.setKeyRepeatEnabled(false);
    engine.makeRoot<CircleObject>(Reds::FireBrick);
    engine.window.setTitle("Evan's Engine");
    engine.run();
    return 0;
}
