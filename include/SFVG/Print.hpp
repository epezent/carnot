#pragma once

#include <vector>
#include <iostream>
#include <SFVG/Graphics/Color.hpp>
#include <SFVG/Imports.hpp>

namespace sfvg {

template <typename T>
inline void print(const T& value) {
    std::cout << value << std::endl;
}

inline void print(const String& str) {
    std::string stdstr = str;
    std::cout << stdstr << std::endl;
}

inline void print(const sf::Color& color) {
    std::cout << "R: " << (int)color.r << " G: " << (int)color.g << " B: " << (int)color.b << std::endl;
}

inline void print(const HSV& hsv) {
    std::cout << "H: " << hsv.h << " S: " << hsv.s << " V: " << hsv.v << std::endl;
}

inline void print(const CMYK& cmyk) {
    std::cout << "C: " << cmyk.c << " M: " << cmyk.y << " Y: " << cmyk.y << " K: " << cmyk.k << std::endl;
}

template <typename T>
inline void print(sf::Vector2<T> value) {
    std::cout << value.x << " , " << value.y << std::endl;
}

template <typename T>
inline void print(sf::Rect<T> value) {
    std::cout << value.left << " " << value.top << " " << value.width << " " << value.height << std::endl;
}

template <typename T>
inline void print(std::vector<T> value) {
    for (std::size_t i = 0; i < value.size(); ++i) {
        std::cout << value[i] << " ";
    }
    std::cout << std::endl;
}

}
