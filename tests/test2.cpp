#include <SFVG/Animation/Tween.hpp>
#include <SFVG/Graphics/Shapes/StarShape.hpp>
#include <SFVG/Graphics/Shapes/SquareShape.hpp>
#include <SFVG/Graphics/Shapes/PolygonShape.hpp>
#include <SFVG/Animation/Animation.hpp>
#include <SFML/Graphics.hpp>
#include <SFVG/Graphics/Color.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include "Fps.hpp"

using namespace sfvg;

void printColor(const sf::Color& color) {
    std::cout << (int)color.r << ",";
    std::cout << (int)color.g << ",";
    std::cout << (int)color.b << ",";
    std::cout << (int)color.a << std::endl;
}

template <typename T>
T MyTween(const T& a, const T& b, float t) {
    float t_prime = 0.1f * std::sin(2.0f * PI * 5.0f * t) + t;
    return Tween::Linear(a,b,t_prime);
}

int main(int argc, char* argv[]) {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(20);
    text.setPosition(5, 5);

    FPS fps;

    // ====

    StarShape shape(6, 50, 100);
    shape.setFillGradient(Gradient(Yellows::Gold, Yellows::LemonChiffon, 0.0f));
    shape.setRadii(10);
    shape.setPosition(250, 250);
    // shape.showWireFrame(true);
    // shape.showBoundsBox(true);

    StarShape star(6, 100, 300);

    Animation<PPosition, PRotation, PScale, PFillGradient> anim;
    anim.keyFrame(0.00f).setFrom(&shape);

    anim.keyFrame(0.25f).delta<PPosition>(sf::Vector2f(500, 0), Tween::Elastic::Out)
                        .delta<PRotation>(90.0f)
                        .absolute<PScale>(sf::Vector2f(2.0f,2.0f))
                        .absolute<PFillGradient>(Gradient(Reds::Crimson, Reds::Salmon));

    anim.keyFrame(0.50f).delta<PPosition>(sf::Vector2f(0, 500), Tween::Bounce::Out)
                        .delta<PRotation>(90.0f)
                        .absolute<PScale>(sf::Vector2f(1.0f,1.0f))
                        .absolute<PFillGradient>(Gradient(Blues::DodgerBlue, Blues::DeepSkyBlue));

    anim.keyFrame(0.75f).delta<PPosition>(sf::Vector2f(-500,0), Tween::Back::Out)
                        .delta<PRotation>(90.0f)
                        .absolute<PScale>(sf::Vector2f(2.0f,2.0f))
                        .absolute<PFillGradient>(Gradient(Greens::Chartreuse, Greens::DarkGreen));

    anim.keyFrame(1.00f).delta<PPosition>(sf::Vector2f(0,-500), Tween::Exponential::In)
                        .delta<PRotation>(90.0f)
                        .absolute<PScale>(sf::Vector2f(1.0f,1.0f))
                        .setFrom<PFillGradient>(&shape).tween = Tween::Smootheststep;
    anim.setDuration(sf::seconds(4));
    anim.setLoop(true);

    // Animation<pPoints, PRotation> anim;
    // anim.keyFrame(0).setFrom(&shape);
    // anim.keyFrame(1).setFrom(&star).get<pPoints>().tween = Tween::Exponential::In;
    // anim.keyFrame(1).delta<PRotation>(90.0f);


    // ====

    sf::Clock dtClock;

    // Main loop
    while (window.isOpen()) {

        fps.update();

        sf::Event event;

        sf::Time dt = dtClock.restart();

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::P)
            {
                anim.start();
            }


            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::R)
            {
                anim.restart();
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::S)
            {
                anim.stop();
            }

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        if (anim.isPlaying()) {
            anim.update(dt);
            anim.applyTo(&shape);
        }

        text.setString("FPS: " + std::to_string(fps.getFPS()));
        window.clear(Grays::Black);
        window.draw(shape);
        window.draw(text);
        window.display();
    }

    return 0;
}





