#include <SFML/Graphics.hpp>
# include <EE/Graphics.hpp>
#include <iostream>

using namespace ee;

int main(int argc, char* argv[]) {

    eeInit();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "EE Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");
    sf::Text label1, label2, label3, label4;
    label1.setFont(font);
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(20);
    label2 = label1; label3 = label1; label4 = label1;
    label1.setString("1"); label2.setString("2");
    label3.setString("3"); label4.setString("4");

    sf::VertexArray line1(sf::Lines, 2);
    sf::VertexArray line2(sf::Lines, 2);

    line1[0].position = sf::Vector2f(250,100);
    line1[1].position = sf::Vector2f(400,900);

    line2[0].position = sf::Vector2f(750,100);
    line2[1].position = sf::Vector2f(600,900);


    CircleShape point(10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        auto mousePosition = sf::Mouse::getPosition(window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            line1[0].position = sf::Vector2f(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            line1[1].position = sf::Vector2f(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            line2[0].position = sf::Vector2f(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            line2[1].position = sf::Vector2f(mousePosition);

        if (intersect(line1[0].position,line1[1].position,line2[0].position,line2[1].position))
            point.setColor(Greens::Chartreuse);
        else
            point.setColor(Reds::FireBrick);

        point.setPosition(intersection(line1[0].position,line1[1].position,line2[0].position,line2[1].position));


        label1.setPosition(line1[0].position);
        label2.setPosition(line1[1].position);
        label3.setPosition(line2[0].position);
        label4.setPosition(line2[1].position);

        window.clear(Grays::Black);
        window.draw(line1);
        window.draw(line2);
        window.draw(point);
        window.draw(label1);
        window.draw(label2);
        window.draw(label3);
        window.draw(label4);

        window.display();
    }

    return 0;
}





