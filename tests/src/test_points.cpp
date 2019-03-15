#include <Carnot/Graphics.hpp>
#include <Carnot/Animation.hpp>
#include <iostream>

using namespace carnot;

template <typename T>
void print(const T& t) {
    std::cout << t << std::endl;
}

void print(const Point& point) {
    std::cout << "(" << point.x << "," << point.y << ")" << std::endl;
}

void print(const Points& points) {
    for (std::size_t i = 0; i < points.size(); ++i) {
        print(points[i]);
    }
}

int main(int argc, char* argv[]) {

    Points points({Point(0,0), Point(100,100), Point(0,100)});

    sf::Vector2f vec(300,400);
    auto norm = normal(vec);

    print(norm);
    print(parallel(Point(10,10), Point(10,10) + vec, Point(0,0), vec));

    print(unit(vec));
}
