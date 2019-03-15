#pragma once

#include <SFVG/Common/Imports.hpp>
#include <string>

namespace sfvg {

/// RGB Color POD type representation
struct RGB {
    float r; /// red component   [0 to 1]
    float g; /// blue component  [0 to 1]
    float b; /// green component [0 to 1]
    float a; /// alpha component [0 to 1]
    operator Color() const;
};

/// HSV Color POD type representation
struct HSV {
    float h; // angle in degrees [0 to 360]
    float s; // a fraction       [0 to 1]
    float v; // a fraction       [0 to 1]
    float a; // alpha component  [0 to 1]
    operator Color() const;
};

/// Creats an RGB from a Color
RGB toRgb(const Color& color);

/// Creates an RGB from an HSV
RGB toRgb(const HSV& hsv);

/// Creates a RGB from a hex code
RGB toRgb(std::string hex);

/// Creates an HSV from a Color
HSV toHsv(const Color& color);

/// Creats an HSV from an RGB
HSV toHsv(const RGB& color);

/// Creates an HSV from a hex code
HSV toHsv(std::string hex);

/// Creats a Color from a hex code
Color hexCode(std::string hex);

//=============================================================================
// Color Functions
//==============================================================================

float luminance(const Color& color);

} // namespace sfvg
