#include <SFVG/Animation/Tween.hpp>
#include <SFVG/Graphics/Shapes/SquareShape.hpp>
#include <SFVG/Animation/Animation.hpp>
#include <SFML/Graphics.hpp>
#include <SFVG/Graphics/Color.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <SFML/Graphics/RectangleShape.hpp>
#include "FPS.hpp"

using namespace sfvg;



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
    text.setCharacterSize(30);
    text.setPosition(10, 10);

    FPS fps;

    // ====


    // SquareShape shape(50);
    // shape.setRadii(5);
    // shape.setFill(solid(a));
    sf::RectangleShape shape(sf::Vector2f(50,50));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(250, 250);

    Animation<Position, FillColor, Rotation> anim;
    anim.keyFrame(0.0f).set<Position>(shape.getPosition())
                       .set<FillColor>(shape.getFillColor());
    anim.keyFrame(0.5f).set<Position>(sf::Vector2f(100, 100),    Tween::Smoothstep)
                       .set<FillColor>(sf::Color::Blue,          Tween::Smoothstep)
                       .change<Rotation>(45.0f,                  Tween::Smoothstep);
    anim.keyFrame(1.0f).change<Position>(sf::Vector2f(150, 150), Tween::Smoothstep)
                       .set<FillColor>(sf::Color::Red,           Tween::Smoothstep)
                       .change<Rotation>(-45.0f,                 Tween::Smoothstep);
    anim.setDuration(sf::seconds(1));
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
                anim.restart();

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        anim.update(&shape, dt);
        text.setString("FPS: " + std::to_string(fps.getFPS()));
        window.clear(sf::Color::White);
        window.draw(shape);
        window.draw(text);
        window.display();
    }

    return 0;
}





