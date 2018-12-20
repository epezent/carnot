#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
// #include <MEL/Utility/RingBuffer.hpp>
#include <vector>

#include <iostream>

template <typename T>
inline void print(const T& value) {
    std::cout << value << std::endl;
}

inline void print(const String& str) {
    std::string stdstr = str;
    std::cout << stdstr << std::endl;
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

// template <typename T>
// inline void print(mel::RingBuffer<T> value) {
//     for (std::size_t i = 0; i < value.size(); ++i) {
//         std::cout << value[i] << " ";
//     }
//     std::cout << std::endl;
// }
