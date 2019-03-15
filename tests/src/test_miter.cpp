#include <SFML/Graphics.hpp>
# include <EE/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace ee;

sf::RenderWindow* G_WINDOW;
sf::Font* G_FONT;

void drawLine(Point a, Point b, sf::Color color) {
    sf::VertexArray v(sf::Lines);
    v.resize(2);
    v[0].position = a;
    v[0].color = color;
    v[1].position = b;
    v[1].color = color;
    G_WINDOW->draw(v);
}

void drawTriangle(Point a, Point b, Point c, sf::Color color) {


    sf::VertexArray t(sf::Triangles);
    t.resize(3);
    t[0].position = a;
    t[1].position = b;
    t[2].position = c;
    auto alpha = color; alpha.a = 64; 
    t[0].color = alpha;
    t[1].color = alpha;
    t[2].color = alpha;

    G_WINDOW->draw(t);


    sf::VertexArray v(sf::LinesStrip);
    v.resize(4);
    v[0].position = a;
    v[0].color = color;
    v[1].position = b;
    v[1].color = color;
    v[2].position = c;
    v[2].color = color;
    v[3].position = a;
    v[3].color = color;
    G_WINDOW->draw(v);


    Point center = (a + b + c) * (1.0f / 3.0f);


    SquareShape p(3);
    p.setPosition(center);
    p.setColor(color);


    G_WINDOW->draw(p);   
}

void drawLabel(Point position, const std::string& str) {
    sf::Text text;
    text.setFillColor(sf::Color::White);
    text.setFont(*G_FONT);
    text.setCharacterSize(10);
    text.setString(str);
    text.setPosition(position);
    auto rect = text.getLocalBounds();
    text.setOrigin(rect.left + 0.5f * rect.width, rect.top + 0.5f * rect.height);
    G_WINDOW->draw(text);
}

int main(int argc, char* argv[]) {

    eeInit();

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Bold.ttf");
    G_FONT = &font;

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(15);
    text.setString("Text");
    text.setPosition(10, 10);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(750, 750), "EE Shapes", sf::Style::Close, settings);
    window.setFramerateLimit(120);

    G_WINDOW = &window;

    SquareShape background(750);
    background.setGradient(Gradient(Grays::Gray50, Grays::Gray40));
    background.setPosition(375, 375);

    // points
    Point A(100, 375);
    Point B(375, 100);
    Point C(375, 650);
    Point D(650, 375);

    float thickness = 50;
    float miterLimit = 4.0;

    Stroke stroke(4);
    stroke.setColor(sf::Color(255,128,128,64));

    ee::CircleShape handle(15);
    handle.setColor(Grays::Gray20);

    while (window.isOpen()) {
        sf::Event event;

        auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        bool inside = (mousePosition.x >= 0 && mousePosition.x <= 1500 &&
                       mousePosition.y >= 0 && mousePosition.y <= 1500);

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled && inside)
                thickness += event.mouseWheelScroll.delta;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
                miterLimit+=0.1f;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
                miterLimit-=0.1f;
        }

        window.clear(Grays::Gray);



        window.draw(background);



        float minDist = INF;
        Point p;
        if (magnitude(mousePosition - A) < minDist) {
            minDist = magnitude(mousePosition - A);
            p = A;
        }
        if (magnitude(mousePosition - B) < minDist) {
            minDist = magnitude(mousePosition - B);
            p = B;
        }
        if (magnitude(mousePosition - C) < minDist) {
            minDist = magnitude(mousePosition - C);
            p = C;
        }
        if (magnitude(mousePosition - D) < minDist) {
            minDist = magnitude(mousePosition - D);
            p = D;
        }

        if (inside && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (p == A)
                A = mousePosition;
            else if (p == B)
                B = mousePosition;
            else if (p == C)
                C = mousePosition;
            else if (p == D)
                D = mousePosition;
        }

        std::string str;

        stroke.setThicnkess(thickness);
        stroke.setPoint(0, A);
        stroke.setPoint(1, B);
        stroke.setPoint(2, C);
        stroke.setPoint(3, D);
        stroke.setMiterLimit(miterLimit);
        //window.draw(stroke);

        handle.setPosition(p);

        float halfThickness = 0.5f * thickness;

        // draw raw lines
        //drawLine(A, B, sf::Color::White);
        //drawLine(B, C, sf::Color::White);
        //drawLine(C, D, sf::Color::White);

        // A
        auto NA = unit(normal(B - A));
        Point A1 = A + NA * halfThickness;
        Point A2 = A - NA * halfThickness;

        // B
        Point NB = unit(normal(B - A));           
        Point TB = unit(unit(C - B) + unit(B - A)); 
        Point MB = normal(TB);     

        float lBm = halfThickness / dot(MB, NB);
        Point Bm1 = B + MB * lBm;
        Point Bm2 = B - MB * lBm;

        float lBx = halfThickness / dot(TB, NB);
        Point Bx1 = B + TB * lBx;
        Point Bx2 = B - TB * lBx;

        Point B1, B2;

        if (lBm > miterLimit * halfThickness) {
            if (winding(A,B,C) < 0) { // ccw

                // drawTriangle(A1, A2, Bx1, Greens::Chartreuse);
                // drawTriangle(A2, Bx1, Bm2, Cyans::Turquoise);
                // drawTriangle(Bx1, Bm2, Bx2, Blues::DeepSkyBlue);
                // B1 = Bx2;
                // B2 = Bm2;


                drawTriangle(A1, A2, Bx1, Greens::Chartreuse);
                drawTriangle(A2, Bx1, Bx2, Cyans::Turquoise);
                B1 = Bx2;
                B2 = Bx1;


            }
            else { // cw

                // drawTriangle(A1, A2, Bm1, Greens::Chartreuse);
                // drawTriangle(A2, Bm1, Bx2, Cyans::Turquoise);
                // drawTriangle(Bm1, Bx2, Bx1, Blues::DeepSkyBlue);
                // B1 = Bm1;
                // B2 = Bx1;

                drawTriangle(A1, A2, Bx1, Greens::Chartreuse);
                drawTriangle(A2, Bx1, Bx2, Cyans::Turquoise);
                B1 = Bx2;
                B2 = Bx1;



            }
        }
        else {
            drawTriangle(A1, A2, Bm1, Greens::Chartreuse);
            drawTriangle(A2, Bm1, Bm2, Cyans::Turquoise);
            B1 = Bm1;
            B2 = Bm2;
        }


        // C
        Point NC = unit(normal(C - B));
        Point TC = unit(unit(D - C) + unit(C - B));
        Point MC = normal(TC);

        float lCm = halfThickness / dot(MC, NC);
        Point Cm1 = C + MC * lCm;
        Point Cm2 = C - MC * lCm;

        float lCx = halfThickness / dot(TC, NC);
        Point Cx1 = C + TC * lCx;
        Point Cx2 = C - TC * lCx;


        Point C1, C2;

        if (lCm > miterLimit * halfThickness) {
            if (winding(B,C,D) < 0) { // ccw

                drawTriangle(B1, B2, Cx1, Purples::BlueViolet);
                drawTriangle(B2, Cx1, Cm2, Purples::Magenta);
                drawTriangle(Cx1, Cm2, Cx2, Reds::Red);
                C1 = Cx2;
                C2 = Cm2;
            }
            else { // cw

                drawTriangle(B1, B2, Cm1, Purples::BlueViolet);
                drawTriangle(B2, Cm1, Cx2, Purples::Magenta);
                drawTriangle(Cm1, Cx2, Cx1, Reds::Red);
                C1 = Cm1;
                C2 = Cx1;
            }
        }
        else {

            drawTriangle(B1, B2, Cm1, Purples::BlueViolet);
            drawTriangle(B2, Cm1, Cm2, Purples::Magenta);
            C1 = Cm1;
            C2 = Cm2;
        }

        if (winding(A1, Bm1, Cm1) < 0)
            str = "CCW";
        else
            str = "CW";

        // D
        auto ND = unit(normal(D - C));
        Point D1 = D + ND * halfThickness;
        Point D2 = D - ND * halfThickness;

        drawTriangle(C1, C2, D1, Oranges::Orange);
        drawTriangle(C2, D1, D2, Yellows::Yellow);

        // draw labels and stuff

        window.draw(handle);

        drawLabel(A, "A");
        drawLabel(B, "B");
        drawLabel(C, "C");
        drawLabel(D, "D");
        drawLabel(Bm1, "Bm1");
        drawLabel(Bm2, "Bm2");
        drawLabel(Bx1, "Bx1");
        drawLabel(Bx2, "Bx2");
        drawLabel(Cm1, "Cm1");
        drawLabel(Cm2, "Cm2");
        drawLabel(Cx1, "Cx1");
        drawLabel(Cx2, "Cx2");

        drawLabel(A1, "A1");
        drawLabel(A2, "A2");
        drawLabel(D1, "D1");
        drawLabel(D2, "D2");

        std::stringstream ss;
        ss << "Thickness:  " << std::fixed << std::setprecision(1) << thickness << std::endl;
        ss << "Miter Limit:  " << std::fixed << std::setprecision(1) << miterLimit << std::endl;
        ss << str;
        text.setString(ss.str());
        window.draw(text);


        window.display();
    }

    return 0;
}





