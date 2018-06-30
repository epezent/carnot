#include <SFVG/Animation/Tween.hpp>
#include <SFVG/Graphics/Shapes/SquareShape.hpp>
#include <SFVG/Animation/Animation.hpp>
#include <SFML/Graphics.hpp>
#include <SFVG/Graphics/Color.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include "Fps.hpp"

using namespace sfvg;

template <typename T>
struct Test {
    Test(T t) : m_t(t) {}
    template <typename S>
    void print(S s);
    T m_t;
};

template <typename T>
template <typename S>
void Test<T>::print(S s) {
    std::cout << m_t << " , " << s << std::endl;
}

void printColor(const sf::Color& color) {
    std::cout << (int)color.r << ",";
    std::cout << (int)color.g << ",";
    std::cout << (int)color.b << ",";
    std::cout << (int)color.a << std::endl;
}

int main(int argc, char* argv[]) {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG", sf::Style::Default, settings);
    //window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(20);
    text.setPosition(5, 5);

    FPS fps;

    // ====

    auto x = Test<int>(2);
    x.print(4.0f);

    SquareShape shape1(50);
    shape1.setFillGradient(Gradient(sf::Color::Yellow, sf::Color::Red));
    shape1.setRadii(10);
    shape1.setPosition(250, 250);

    SquareShape shape2(25);
    shape2.setFillGradient(Gradient(sf::Color::Green, sf::Color::Blue));
    shape2.setRadii(5);
    shape2.setPosition(250, 250);

    shape1.addHole(Shape::offsetShape(shape2, 5));

    Animation<Position, Rotation, Scale> anim;
    anim.keyFrame(0.00f).setFrom(&shape1);
    anim.keyFrame(0.25f).delta<Position>(sf::Vector2f(500, 0), Tween::Exponential::Out);
    anim.keyFrame(0.50f).delta<Position>(sf::Vector2f(0, 500), Tween::Exponential::Out);
    anim.keyFrame(0.75f).delta<Position>(sf::Vector2f(-500,0), Tween::Exponential::Out);
    anim.keyFrame(1.00f).delta<Position>(sf::Vector2f(0,-500), Tween::Exponential::Out);
    anim.keyFrame(0.25f).delta<Rotation>(90.0f, Tween::Exponential::Out);
    anim.keyFrame(0.50f).delta<Rotation>(90.0f, Tween::Exponential::Out);
    anim.keyFrame(0.75f).delta<Rotation>(90.0f, Tween::Exponential::Out);
    anim.keyFrame(1.00f).delta<Rotation>(90.0f, Tween::Exponential::Out);
    anim.keyFrame(0.25f).absolute<Scale>(sf::Vector2f(2.0f,2.0f), Tween::Exponential::Out);
    anim.keyFrame(0.50f).absolute<Scale>(sf::Vector2f(1.0f,1.0f), Tween::Exponential::Out);
    anim.keyFrame(0.75f).absolute<Scale>(sf::Vector2f(2.0f,2.0f), Tween::Exponential::Out);
    anim.keyFrame(1.00f).absolute<Scale>(sf::Vector2f(1.0f,1.0f), Tween::Exponential::Out);

    anim.setDuration(sf::seconds(4));

    // ====

    sf::Clock dtClock;

    // Main loop
    while (window.isOpen()) {

        fps.update();

        sf::Event event;

        sf::Time dt = dtClock.restart();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
            {
                //anim.keyFrame(0.0f).setFrom(&shape1);
                anim.restart();
            }

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        if (anim.isPlaying()) {
            anim.update(dt);
            anim.applyTo(&shape1);
            anim.applyTo(&shape2);
        }

        text.setString("FPS: " + std::to_string(fps.getFPS()));
        window.clear(sf::Color::White);
        window.draw(shape1);
        window.draw(shape2);
        window.draw(text);
        window.display();
    }

    return 0;
}





