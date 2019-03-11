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

//==============================================================================
// Named Colors
//==============================================================================

namespace Pinks
{
    const extern Color Pink;
    const extern Color LightPink;
    const extern Color HotPink;
    const extern Color DeepPink;
    const extern Color PaleVioletRed;
    const extern Color MediumVioletRed;
}

namespace Reds
{
    const extern Color LightSalmon;
    const extern Color Salmon;
    const extern Color DarkSalmon;
    const extern Color LightCoral;
    const extern Color IndianRed;
    const extern Color Crimson;
    const extern Color FireBrick;
    const extern Color DarkRed;
    const extern Color Red;
}

namespace Oranges
{
    const extern Color OrangeRed;
    const extern Color Tomato;
    const extern Color Coral;
    const extern Color DarkOrange;
    const extern Color Orange;
}

namespace Yellows {
    const extern Color Yellow;
    const extern Color LightYellow;
    const extern Color LemonChiffon;
    const extern Color LightGoldenrodYellow;
    const extern Color PapayaWhip;
    const extern Color Moccasin;
    const extern Color PeachPuff;
    const extern Color PaleGoldenrod;
    const extern Color Khaki;
    const extern Color DarkKhaki;
    const extern Color Gold;
}

namespace Browns
{
    const extern Color Cornsilk;
    const extern Color BlanchedAlmond;
    const extern Color Bisque;
    const extern Color NavajoWhite;
    const extern Color Wheat;
    const extern Color BurlyWood;
    const extern Color Tan;
    const extern Color RosyBrown;
    const extern Color SandyBrown;
    const extern Color Goldenrod;
    const extern Color DarkGoldenrod;
    const extern Color Peru;
    const extern Color Chocolate;
    const extern Color SaddleBrown;
    const extern Color Sienna;
    const extern Color Brown;
    const extern Color Maroon;
}

namespace Greens
{
    const extern Color DarkOliveGreen;
    const extern Color Olive;
    const extern Color OliveDrab;
    const extern Color YellowGreen;
    const extern Color LimeGreen;
    const extern Color Lime;
    const extern Color LawnGreen;
    const extern Color Chartreuse;
    const extern Color GreenYellow;
    const extern Color SpringGreen;
    const extern Color MediumSpringGreen;
    const extern Color LightGreen;
    const extern Color PaleGreen;
    const extern Color DarkSeaGreen;
    const extern Color MediumAquamarine;
    const extern Color MediumSeaGreen;
    const extern Color SeaGreen;
    const extern Color ForestGreen;
    const extern Color Green;
    const extern Color DarkGreen;
}

namespace Cyans
{
    const extern Color Aqua;
    const extern Color Cyan;
    const extern Color LightCyan;
    const extern Color PaleTurquoise;
    const extern Color Aquamarine;
    const extern Color Turquoise;
    const extern Color MediumTurquoise;
    const extern Color DarkTurquoise;
    const extern Color LightSeaGreen;
    const extern Color CadetBlue;
    const extern Color DarkCyan;
    const extern Color Teal;
}

namespace Blues {
    const extern Color LightSteelBlue;
    const extern Color PowderBlue;
    const extern Color LightBlue;
    const extern Color SkyBlue;
    const extern Color LightSkyBlue;
    const extern Color DeepSkyBlue;
    const extern Color DodgerBlue;
    const extern Color CornflowerBlue;
    const extern Color SteelBlue;
    const extern Color RoyalBlue;
    const extern Color Blue;
    const extern Color MediumBlue;
    const extern Color DarkBlue;
    const extern Color Navy;
    const extern Color MidnightBlue;
}

namespace Purples
{
    const extern Color Lavender;
    const extern Color Thistle;
    const extern Color Plum;
    const extern Color Violet;
    const extern Color Orchid;
    const extern Color Fuchsia;
    const extern Color Magenta;
    const extern Color MediumOrchid;
    const extern Color MediumPurple;
    const extern Color BlueViolet;
    const extern Color DarkViolet;
    const extern Color DarkOrchid;
    const extern Color DarkMagenta;
    const extern Color Purple;
    const extern Color Indigo;
    const extern Color DarkSlateBlue;
    const extern Color SlateBlue;
    const extern Color MediumSlateBlue;
}

namespace Whites
{
    const extern Color White;
    const extern Color Snow;
    const extern Color Honeydew;
    const extern Color MintCream;
    const extern Color Azure;
    const extern Color AliceBlue;
    const extern Color GhostWhite;
    const extern Color WhiteSmoke;
    const extern Color Seashell;
    const extern Color Beige;
    const extern Color OldLace;
    const extern Color FloralWhite;
    const extern Color Ivory;
    const extern Color AntiqueWhite;
    const extern Color Linen;
    const extern Color LavenderBlush;
    const extern Color MistyRose;
}

namespace Grays
{
    const extern Color Gainsboro;
    const extern Color LightGray;
    const extern Color Silver;
    const extern Color DarkGray;
    const extern Color Gray;
    const extern Color DimGray;
    const extern Color LightSlateGray;
    const extern Color SlateGray;
    const extern Color DarkSlateGray;
    const extern Color Black;
    const extern Color Gray10;
    const extern Color Gray20;
    const extern Color Gray30;
    const extern Color Gray40;
    const extern Color Gray50;
    const extern Color Gray60;
    const extern Color Gray70;
    const extern Color Gray80;
    const extern Color Gray90;
}

} // namespace sfvg
