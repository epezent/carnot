#include <SFVG/Graphics.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <sstream>

using namespace sfvg;

class ColorDemo : public Object {
public:

    ColorDemo() {
        m_sqr.setSideLength(500.0f);
        m_sqr.setColor(Color::Black);
        m_sqr.setPosition(250,250);
        m_font.loadFromFile("../../fonts/Roboto-Bold.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(20);
        m_text.setPosition(5, 5);
        m_text.setFillColor(Color::Black);
        // init background
        std::size_t n = 50;
        float s = 500.0f/n;
        SquareShape bg(s);
        bg.setColor(Grays::Gray50);
        for (std::size_t i = 0; i < n/2; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                bg.setPosition(s*(j%2) + s/2.0f  + 2*i * s, s/2  + j * s);
                m_background.push_back(bg);
            }
        }
    }

    void update() override {
        int dir = 0;
        if (Input::getKeyDown(Key::Up))
            dir = 1;
        else if (Input::getKeyDown(Key::Down))
            dir = -1;

        if (dir != 0) {
            if (Input::getKey(Key::R))
                m_color.r += dir;
            if (Input::getKey(Key::G))
                m_color.g += dir;
            if (Input::getKey(Key::B))
                m_color.b += dir;
            if (Input::getKey(Key::A))
                m_color.a += dir;

            if (Input::getKey(Key::H))
                m_color.setH(m_color.getH() + dir * 1.0f);
            if (Input::getKey(Key::S))
                m_color.setS(m_color.getS() + dir * 1.0f / 255.0f);
            if (Input::getKey(Key::V))
                m_color.setV(m_color.getV() + dir * 1.0f / 255.0f);

            if (Input::getKey(Key::C))
                m_color.setC(m_color.getC() + dir * 1.0f / 255.0f);
            if (Input::getKey(Key::M))
                m_color.setM(m_color.getM() + dir * 1.0f / 255.0f);
            if (Input::getKey(Key::Y))
                m_color.setY(m_color.getY() + dir * 1.0f / 255.0f);
            if (Input::getKey(Key::K))
                m_color.setK(m_color.getK() + dir * 1.0f / 255.0f);
        }

        m_sqr.setColor(m_color);

        std::ostringstream ss;
        ss << "R: " << (int)m_color.r << std::endl;
        ss << "G: " << (int)m_color.g << std::endl;
        ss << "B: " << (int)m_color.b << std::endl;
        ss << "A: " << (int)m_color.a << std::endl;
        ss << std::endl;
        ss << "C: " << m_color.getC() << std::endl;
        ss << "M: " << m_color.getM() << std::endl;
        ss << "Y: " << m_color.getY() << std::endl;
        ss << "K: " << m_color.getK() << std::endl;
        ss << std::endl;
        ss << "H: " << m_color.getH() << std::endl;
        ss << "S: " << m_color.getS() << std::endl;
        ss << "V: " << m_color.getV() << std::endl;

        m_text.setString(ss.str());
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (auto& sqr : m_background)
            target.draw(sqr, states);
        target.draw(m_sqr, states);
        target.draw(m_text, states);
    }

    XColor m_color;
    SquareShape m_sqr;
    Font m_font;
    Text m_text;
    std::vector<SquareShape> m_background;
};



int main(int argc, char const *argv[])
{
    Engine engine(500,500);
    engine.setBackgroundColor(Color::White);
    engine.setWindowTitle("Engine Testing");
    auto root = engine.makeRoot<ColorDemo>();
    root->setName("root");
    engine.run();
    return 0;
}
