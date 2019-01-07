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

class BoxCollider : public Object {
public:
    BoxCollider(float width, float height) :
        m_width(width), m_height(height)
    {

    }

    void draw(RenderTarget& target, RenderStates states) const override {
        std::array<sf::Vertex, 5> verts;
        verts[0].position = Vector2f(-m_width/2,-m_height/2);
        verts[1].position = Vector2f( m_width/2,-m_height/2);
        verts[2].position = Vector2f( m_width/2, m_height/2);
        verts[3].position = Vector2f(-m_width/2, m_height/2);
        verts[4].position = Vector2f(-m_width/2,-m_height/2);
        for (auto& v : verts)
            v.color = randomColor();
        target.draw(&verts[0], 5, sf::LineStrip, states);
    }

    float m_width;
    float m_height;

};

template <typename Renderable>
class Renderer : public Object {
public:

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(renderable, states);
    }

    Renderable renderable;
};

class MyObject : public Object {
public:

    void start() override {
        // m_sqr = makeChild<Renderer<SquareShape>>();
        // m_sqr->renderable.setSideLength(250);
        // m_sqr->renderable.setGradient(Gradient(Blues::DeepSkyBlue,Reds::FireBrick));
        m_col = makeChild<BoxCollider>(250,250);
    }

    void update() override {
        if (Input::getKey(Key::R))
            rotate(60*Engine::deltaTime());
    }

private:
    Handle<Renderer<SquareShape>> m_sqr;
    Handle<BoxCollider> m_col;
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
