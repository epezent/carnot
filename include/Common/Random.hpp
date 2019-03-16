#pragma once

#include <Common/Imports.hpp>

namespace carnot {

/// @brief Returns an int random number in the interval [min, max].
/// @pre min <= max
int random(int min, int max);

/// @brief Returns an unsigned int random number in the interval [min, max].
/// @pre min <= max
unsigned int random(unsigned int min, unsigned int max);

/// @brief Returns a float random number in the interval [min, max].
/// @pre min <= max
float random(float min, float max);

/// @brief Returns a float random number in the interval [middle-deviation, middle+deviation].
/// @pre deviation >= 0
float randomDev(float middle, float deviation);

/// @brief Sets the seed of the random number generator.
/// @details Setting the seed manually is useful when you want to reproduce a given sequence of random
///  numbers. Without calling this function, the seed is different at each program startup.
void setRandomSeed(unsigned long seed);

Color randomColor();

}
