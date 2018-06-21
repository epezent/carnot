#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFVG/Shape.hpp>
#include <SFVG/Fps.hpp>
#include <iostream>
#include <cmath>
#include <sstream>
#include <limits>
#include <SFVG/Shapes/SquareShape.hpp>
#include <SFVG/Shapes/RectangleShape.hpp>
#include <SFVG/Shapes/StarShape.hpp>
#include <SFVG/Shapes/PolygonShape.hpp>
#include <SFVG/ShapeBatch.hpp>
#include <SFVG/Detail/Detail.hpp>

#define     WINDOW_WIDTH    1000.0f
#define     WINDOW_HEIGHT   1000.0f

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFVG", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::RenderTexture rTexture, hBlurTex, vBlurTex;
    rTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);
    hBlurTex.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);
    vBlurTex.create(WINDOW_WIDTH, WINDOW_HEIGHT, settings);

    sf::Shader gradient;
    gradient.loadFromFile("../shaders/linear_gradient.frag", sf::Shader::Fragment);
    gradient.setUniform("texture", sf::Shader::CurrentTexture);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(30);
    text.setPosition(10, 10);

    //==========================================================================

    SquareShape background(1000.0f);
    background.setPosition(500,500);

    PolygonShape poly(6, PolygonShape::CircumscribedRadius, 100);
    poly.setPosition(500, 500);

    PolygonShape circle(100, PolygonShape::CircumscribedRadius, 10);
    circle.setFillColor(sf::Color(0,0,0,128));
    circle.setPosition(500, 500);

    //==========================================================================
    FPS fps;
    sf::Vector2i mousePosition;

    sf::Clock deltaClock;
    sf::Clock absClock;
    int N = 6;
    int offset = -25;
    float radius = 10;

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
                offset--;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
               offset++;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LBracket){
                radius--;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RBracket){
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
        gradient.setUniform("u_color1", sf::Glsl::Vec4(1,1,1,1));
        gradient.setUniform("u_color2", sf::Glsl::Vec4(0.5,0.5,0.5,1));
        gradient.setUniform("u_angle", 45.0f);
        rTexture.draw(background, &gradient);
        gradient.setUniform("u_color1", sf::Glsl::Vec4(0,1,0,1));
        gradient.setUniform("u_color2", sf::Glsl::Vec4(0,0,1,1));
        gradient.setUniform("u_angle", 0.0f);
        rTexture.draw(poly, &gradient);
        rTexture.draw(circle);
        rTexture.display();
        sf::Sprite sprite(rTexture.getTexture());

        std::ostringstream ss;
        ss << "FPS: " << fps.getFPS() << "\n";
        ss << "N: " << N << "\n";
        ss << "Offset: " << offset << "\n";
        ss << "Radius: " << radius;

        text.setString(ss.str());

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(text);
        window.display();
    }

    return 0;
}


