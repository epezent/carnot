#include <carnot>

using namespace carnot;

void renderingThread(sf::RenderWindow* window)
{
    // activate the window's context
    window->setActive(true);

    sf::CircleShape circle(100);
    circle.setFillColor(sf::Color::Red);
    Clock clock;
    // the rendering loop
    while (window->isOpen())
    {
        // draw...
        auto t = clock.getElapsedTime().asSeconds();
        circle.setPosition(100*sin(2*Math::PI*t),0);
        window->clear();
        window->draw(circle);
        // end the current frame
        window->display();
    }
}

int main()
{
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");

    // deactivate its OpenGL context
    window.setActive(false);

    // launch the rendering thread
    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}

