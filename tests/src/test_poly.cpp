#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Fps.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <limits>
# include <EE/Graphics.hpp>
# include <EE/Math.hpp>
# include <EE/Graphics/Color.hpp>

using namespace ee;

#define     WINDOW_WIDTH    1000.0f
#define     WINDOW_HEIGHT   1000.0f

int main()
{
    eeInit();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "EE", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::RenderTexture rTexture, hBlurTex, vBlurTex;
    rTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);
    hBlurTex.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);
    vBlurTex.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(30);
    text.setPosition(10, 10);

    sf::Texture texture;
    texture.loadFromFile("../textures/paper.png");

    //==========================================================================

    int N = 6;
    int offset = -50;
    float radius = 25;
    float circumscribedRadius = 200;



    SquareShape background(1000.0f);
    background.setPosition(500,500);
    background.setGradient(Gradient(sf::Color::White, sf::Color(128,128,128,255), 45.0f));

    PolygonShape poly(N, PolygonShape::CircumscribedRadius, circumscribedRadius);
    poly.setPosition(500, 500);
    poly.setGradient(Gradient(Greens::Chartreuse, Cyans::Teal, 45));
    //poly.setTexture(&texture);

    PolygonShape circle(100, PolygonShape::CircumscribedRadius, 10);
    circle.setColor(sf::Color(0,0,0,128));
    circle.setPosition(500, 500);

    //==========================================================================
    FPS fps;
    sf::Vector2i mousePosition;

    sf::Clock deltaClock;
    sf::Clock absClock;


   // Main loop
    while (window.isOpen()) {
        // Event handling
        fps.update();

        mousePosition = sf::Mouse::getPosition(window);
        sf::Time dt = deltaClock.restart();

        sf::Event event;
        // Event loop (keyboard, mouse ...
        while (window.pollEvent(event))
        {
            // Close the window if a key is pressed or if requested
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
                N++;
                poly.setSideCount(N);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
                N--;
                if (N < 3)
                    N = 3;
               poly.setSideCount(N);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
                circumscribedRadius--;
                if (circumscribedRadius < 0.0f)
                    circumscribedRadius = 0.0f;
                poly.setCircumscribedRadius(circumscribedRadius);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
                circumscribedRadius++;
                poly.setCircumscribedRadius(circumscribedRadius);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A){
                offset--;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D){
               offset++;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S){
                radius--;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W){
               radius++;
            }

            // Exit the app when escape key is pressed
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float distance = std::numeric_limits<float>::max();
        std::size_t closest = 0;
        for (std::size_t i = 0; i < poly.getPointCount(); ++i) {
            auto position = poly.getTransform().transformPoint(poly.getPoint(i)) - sf::Vector2f(mousePosition.x, mousePosition.y);
            float newDistance = std::sqrt(position.x*position.x + position.y*position.y);
            if (newDistance < distance) {
                distance = newDistance;
                closest = i;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            poly.setPoint(closest, poly.getInverseTransform().transformPoint(sf::Vector2f(mousePosition.x,  mousePosition.y)));
        }

        auto moveTo = poly.getTransform().transformPoint(poly.getPoint(closest));
        // float mag = magnitude((moveTo - circle.getPosition()));
        // auto direction = (moveTo - circle.getPosition()) / mag;
        circle.setPosition(moveTo);

        // udpate poly
        poly.rotate(45 * dt.asSeconds());
        if (offset >= 0) {
            poly.setHoleCount(0);
            offset = 0;
        }
        else {
            poly.setHoleCount(1);
            poly.setHole(0, Shape::offsetShape(poly, offset, Shape::Round));
        }
        if (radius < 0.0f)
            radius = 0.0f;
        poly.setRadii(radius);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            poly.showWireFrame(true);
            poly.showBoundsBox(true);
        }
        else {
            poly.showWireFrame(false);
            poly.showBoundsBox(false);
        }

        // draw to render texture
        rTexture.clear(sf::Color::White);
        rTexture.draw(background);
        rTexture.draw(poly);
        rTexture.draw(circle);
        rTexture.display();
        sf::Sprite sprite(rTexture.getTexture());

        std::ostringstream ss;
        ss << "FPS: " << fps.getFPS() << "\n";
        ss << "N: " << N << " [UP,DOWN]\n";
        ss << "Radius: " << circumscribedRadius << " [LEFT,RIGHT]\n";
        ss << "Offset: " << offset << " [A,D]\n";
        ss << "Corner Radius: " << radius << " [S,W]";


        text.setString(ss.str());

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(text);
        window.display();
    }

    return 0;
}


