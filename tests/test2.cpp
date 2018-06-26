#include <SFVG/Math.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
	sf::Vector2f v1(0, 100);
	sf::Vector2f v2(-50, 200);
	auto v3 = sfvg::lerp(v1,v2,0.5f);
	std::cout << v3.x << " " << v3.y << std::endl;
	return 0;
}