#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <SFVG/Graphics/Color.hpp>
#include <SFVG/Imports.hpp>

namespace sfvg {

//=============================================================================
// Helpers
//=============================================================================

std::string numToStr(float number);
std::string numToStr(double number);

//=============================================================================
// Stream Overloads
//=============================================================================

std::ostream & operator << (std::ostream &out, const Color& color);
std::ostream & operator << (std::ostream &out, const RGB& rgb);
std::ostream & operator << (std::ostream &out, const HSV& hsv);
std::ostream & operator << (std::ostream &out, const CMYK& cmyk);
std::ostream & operator << (std::ostream &out, const sf::String& str);

template <typename T>
std::ostream & operator << (std::ostream &out, const sf::Vector2<T>& value) {
    out << "(X:" << value.x << ",Y:" << value.y << ")";
    return out;
}

template <typename T>
std::ostream & operator << (std::ostream &out, const sf::Rect<T>& value) {
    out << "(L:" << value.left << ",T:" << value.top << ",W:" << value.width << ",H:" << value.height << ")";
    return out;
}

//=============================================================================
// Print
//=============================================================================

/// Prints a value to std::cout and then starts a new line
template <typename T>
inline void print(const T& value) {
    std::cout << value << std::endl;
}

/// Prints variadic number of values to std::cout with separating spaces and then starts a new line
template <typename Arg, typename... Args>
void print(Arg&& arg, Args&&... args) {
    std::cout << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(std::cout << ' ' << std::forward<Args>(args)), 0)...};
    std::cout << std::endl;
}

} // namespace sfvg