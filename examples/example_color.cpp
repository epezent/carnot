#include <SFVG/SFVG.hpp>

using namespace sfvg;

int main(int argc, char const *argv[])
{
    Color color = toRgb("7fff00");
    print(color);
    RGB rgb = toRgb(color);
    print(rgb);
    print((Color)rgb);
    HSV hsv = toHsv("7fff00");
    print(hsv);
    print((Color)toRgb(hsv));
    return 0;
}
