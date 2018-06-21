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

#define     WINDOW_WIDTH    1000.0f
#define     WINDOW_HEIGHT   1000.0f

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFVG", sf::Style::Default, settings);
    // window.setVerticalSyncEnabled(true);

    sf::Shader gradient;
    gradient.loadFromFile("../shaders/linear_gradient.frag", sf::Shader::Fragment);
    gradient.setUniform("texture", sf::Shader::CurrentTexture);
    gradient.setUniform("u_color1", sf::Glsl::Vec4(1,0,0,1));
    gradient.setUniform("u_color2", sf::Glsl::Vec4(0,0,1,1));

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Yellow);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::Black);
    text.setCharacterSize(40);

    //==========================================================================

    std::size_t N = 3;

    std::vector<PolygonShape> shapes;
    for (std::size_t x = 0; x < 10; ++x) {
        for (std::size_t y = 0; y < 10; ++y) {
            PolygonShape shape(N, PolygonShape::CircumscribedRadius, 50);
            shape.setPosition(x*100 + 50, y*100 + 50);
            shapes.push_back(shape);
        }
        N++;
    }



    //==========================================================================


    FPS fps;
    sf::Vector2i mousePosition;

    sf::Clock deltaClock;
    sf::Clock absClock;

    PolygonShape hole(100, PolygonShape::CircumscribedRadius, 10);

   // Main loop
    while (window.isOpen()) {
        // Event handling
        fps.update();

        sf::Time dt = deltaClock.restart();

        sf::Event event;
        // Event loop (keyboard, mouse ...
        while (window.pollEvent(event))
        {
            // Close the window if a key is pressed or if requested
            if (event.type == sf::Event::Closed)
                window.close();

            // Exit the app when escape key is pressed
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        if (N < 3)
            N = 3;

        std::ostringstream ss;
        ss << "FPS: " << fps.getFPS();

        window.clear(sf::Color::Black);


        for (std::size_t i = 0; i < shapes.size(); ++i) {
            shapes[i].rotate(45.0f * dt.asSeconds());
            //shapes[i].setSideCount(N);
            shapes[i].setRadii(10);
            shapes[i].setHoleCount(1);
            shapes[i].setHole(0, Shape::offsetShape(shapes[i], -10, Shape::Round));
        }

        for (std::size_t i = 0; i < shapes.size(); ++i) {
            window.draw(shapes[i], &gradient);
        }

        text.setString(ss.str());
        window.draw(text);
        window.display();
    }

    return 0;
}

// GPU: 1.20 ms
// CPU: 12.5  ms


