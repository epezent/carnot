#include <SFML/Graphics.hpp>
#include <Graphics.hpp>
#include <Tween.hpp>

using namespace carnot;

#define WINDOW_WIDTH 1000.0f
#define WINDOW_HEIGHT 1000.0f

int main() {

    eeInit();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Carnot",
                            sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("../../fonts/RobotoMono-Bold.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(30);
    text.setPosition(10, 10);

    PolygonShape poly(6, PolygonShape::CircumscribedRadius, 50.0f);
    poly.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    poly.setGradient(Gradient(Blues::DeepSkyBlue, Reds::FireBrick, 45.0f));
    sf::Clock moveClock;
    sf::Vector2f moveFrom = poly.getPosition();
    sf::Vector2f moveTo   = poly.getPosition();
    int function = 1;

    // Main loop
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    moveFrom = poly.getPosition();
                    moveTo.x = event.mouseButton.x;
                    moveTo.y = event.mouseButton.y;
                    moveClock.restart();
                }
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Up)
            {
                function++;
                if (function > 34)
                    function = 0;
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Down)
            {
                function--;
                if (function < 0)
                    function = 34;
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        sf::Vector2f position;
        float t = moveClock.getElapsedTime().asSeconds();
        t = clamp01(t);

        if (function == 0) {
            position = Tween::Instant(moveFrom, moveTo, t);
            text.setString("Instant");
        }
        else if (function == 1) {
            position = Tween::Linear(moveFrom, moveTo, t);
            text.setString("Linear");
        }
        else if (function == 2) {
            position = Tween::Smoothstep(moveFrom, moveTo, t);
            text.setString("Smoothstep");
        }
        else if (function == 3) {
            position = Tween::Smootherstep(moveFrom, moveTo, t);
            text.setString("Smootherstep");
        }
        else if (function == 4) {
            position = Tween::Smootheststep(moveFrom, moveTo, t);
            text.setString("Smootheststep");
        }
        else if (function == 5) {
            position = Tween::Quadratic::In(moveFrom, moveTo, t);
            text.setString("Quadratic::In");
        }
        else if (function == 6) {
            position = Tween::Quadratic::Out(moveFrom, moveTo, t);
            text.setString("Quadratic::Out");
        }
        else if (function == 7) {
            position = Tween::Quadratic::InOut(moveFrom, moveTo, t);
            text.setString("Quadratic::InOut");
        }
        else if (function == 8) {
            position = Tween::Cubic::In(moveFrom, moveTo, t);
            text.setString("Cubic::In");
        }
        else if (function == 9) {
            position = Tween::Cubic::Out(moveFrom, moveTo, t);
            text.setString("Cubic::Out");
        }
        else if (function == 10) {
            position = Tween::Cubic::InOut(moveFrom, moveTo, t);
            text.setString("Cubic::InOut");
        }
        else if (function == 11) {
            position = Tween::Quartic::In(moveFrom, moveTo, t);
            text.setString("Quartic::In");
        }
        else if (function == 12) {
            position = Tween::Quartic::Out(moveFrom, moveTo, t);
            text.setString("Quartic::Out");
        }
        else if (function == 13) {
            position = Tween::Quartic::InOut(moveFrom, moveTo, t);
            text.setString("Quartic::InOut");
        }
        else if (function == 14) {
            position = Tween::Quintic::In(moveFrom, moveTo, t);
            text.setString("Quintic::In");
        }
        else if (function == 15) {
            position = Tween::Quintic::Out(moveFrom, moveTo, t);
            text.setString("Quintic::Out");
        }
        else if (function == 16) {
            position = Tween::Quintic::InOut(moveFrom, moveTo, t);
            text.setString("Quintic::InOut");
        }
        else if (function == 17) {
            position = Tween::Sinusoidal::In(moveFrom, moveTo, t);
            text.setString("Sinusoidal::In");
        }
        else if (function == 18) {
            position = Tween::Sinusoidal::Out(moveFrom, moveTo, t);
            text.setString("Sinusoidal::Out");
        }
        else if (function == 19) {
            position = Tween::Sinusoidal::InOut(moveFrom, moveTo, t);
            text.setString("Sinusoidal::InOut");
        }
        else if (function == 20) {
            position = Tween::Exponential::In(moveFrom, moveTo, t);
            text.setString("Exponential::In");
        }
        else if (function == 21) {
            position = Tween::Exponential::Out(moveFrom, moveTo, t);
            text.setString("Exponential::Out");
        }
        else if (function == 22) {
            position = Tween::Exponential::InOut(moveFrom, moveTo, t);
            text.setString("Exponential::InOut");
        }
        else if (function == 23) {
            position = Tween::Circular::In(moveFrom, moveTo, t);
            text.setString("Circular::In");
        }
        else if (function == 24) {
            position = Tween::Circular::Out(moveFrom, moveTo, t);
            text.setString("Circular::Out");
        }
        else if (function == 25) {
            position = Tween::Circular::InOut(moveFrom, moveTo, t);
            text.setString("Circular::InOut");
        }
        else if (function == 26) {
            position = Tween::Elastic::In(moveFrom, moveTo, t);
            text.setString("Elastic::In");
        }
        else if (function == 27) {
            position = Tween::Elastic::Out(moveFrom, moveTo, t);
            text.setString("Elastic::Out");
        }
        else if (function == 28) {
            position = Tween::Elastic::InOut(moveFrom, moveTo, t);
            text.setString("Elastic::InOut");
        }
        else if (function == 29) {
            position = Tween::Back::In(moveFrom, moveTo, t);
            text.setString("Back::In");
        }
        else if (function == 30) {
            position = Tween::Back::Out(moveFrom, moveTo, t);
            text.setString("Back::Out");
        }
        else if (function == 31) {
            position = Tween::Back::InOut(moveFrom, moveTo, t);
            text.setString("Back::InOut");
        }
        else if (function == 32) {
            position = Tween::Bounce::In(moveFrom, moveTo, t);
            text.setString("Bounce::In");
        }
        else if (function == 33) {
            position = Tween::Bounce::Out(moveFrom, moveTo, t);
            text.setString("Bounce::Out");
        }
        else if (function == 34) {
            position = Tween::Bounce::InOut(moveFrom, moveTo, t);
            text.setString("Bounce::InOut");
        }

        poly.setPosition(position);
        text.setString(text.getString() + "\n[UP/DOWN]");
        window.clear(sf::Color::White);
        window.draw(poly);
        window.draw(text);
        window.display();
    }

    return 0;
}
