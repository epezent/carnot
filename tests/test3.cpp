#include <SFVG/Graphics/Points.hpp>
#include <SFVG/Animation/Tween.hpp>
#include <iostream>

using namespace sfvg;

void print(const Point& point) {
    std::cout << "(" << point.x << "," << point.y << ")" << std::endl;
}

void print(const Points& points) {
    for (std::size_t i = 0; i < points.size(); ++i) {
        print(points[i]);
    }
}

int main(int argc, char* argv[]) {

    Points points1({Point(1,2), Point(3,4), Point(5,6)});
    Points points2({Point(7,8), Point(9,10), Point(11,12)});

    auto points3 = Tween::Exponential::In(points1, points2, 0.5f);
    print(points3);

}
