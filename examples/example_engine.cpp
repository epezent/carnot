#include <SFVG/Engine.hpp>
#include <SFVG/Graphics.hpp>

using namespace sfvg;

ResourceManager<Texture, std::string> g_textures;

class SpriteRenderer : public Object {
public:
    SpriteRenderer() {}

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(sprite, states);
    }

    Sprite sprite;
};

class MyObject : public Object {
public:

    MyObject() {
        m_sqr.setGradient(Gradient(Blues::DeepSkyBlue,Reds::FireBrick));
        m_sqr.setSideLength(250);
        m_bg.setSideLength(1000);
        m_bg.setColor(Whites::White);
    }

    void start() override {
        m_sr = makeChild<SpriteRenderer>();
        m_sr->sprite.setTexture(engine().textures.get("paper"));
        print("Start!");
        m_sr->sprite.setColor(Reds::FireBrick);
        m_sr->sprite.setScale(250.0f/2048.0f, 250.0f/2048.0f);
        alignBottomCenter(m_sr->sprite);
    }

    void update() override {
        if (Input::getKeyDown(Key::Up)) {
            engine().getView(0).move(Vector2f(0,-10));
        }
        if (Input::getKeyDown(Key::Down)) {
            engine().getView(0).move(Vector2f(0,10));
        }
    }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(m_bg, states);
        target.draw(m_sqr, states);
    }

private:
    SquareShape m_sqr;
    SquareShape m_bg;
    Handle<SpriteRenderer> m_sr;
};


int main(int argc, char const *argv[])
{
    Engine engine(500,500);
    engine.textures.load("paper", "../../textures/paper.png");
    engine.textures.get("paper").setSmooth(true);
    Sprite sprite;
    sprite.setTexture(engine.textures.get("paper"));
    engine.makeRoot<MyObject>();
    engine.getView(0).setCenter(0,0);
    engine.run();
    return 0;
}
