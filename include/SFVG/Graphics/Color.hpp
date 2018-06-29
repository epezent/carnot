#ifndef SFVG_COLOR_HPP
#define SFVG_COLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <string>

namespace sfvg {

namespace Color {

    /// \brief Creates a color from CMYK values
    ///
    /// \param c Cyan component [0..1]
    /// \param m Magenta component [0..1]
    /// \param y Yellow component [0..1]
    /// \param k Black component [0..1]
    sf::Color cmyk(float c, float m, float y, float k);

    /// Creates a color from HSV values
    sf::Color hsv(float h, float s, float v);

    /// Creates a color from hex code
    sf::Color hex(std::string hex);
}


//==============================================================================
// NAMED COLORS
//==============================================================================

namespace Pinks
{
    const extern sf::Color Pink;
    const extern sf::Color LightPink;
    const extern sf::Color HotPink;
    const extern sf::Color DeepPink;
    const extern sf::Color PaleVioletRed;
    const extern sf::Color MediumVioletRed;
}

namespace Reds
{
    const extern sf::Color LightSalmon;
    const extern sf::Color Salmon;
    const extern sf::Color DarkSalmon;
    const extern sf::Color LightCoral;
    const extern sf::Color IndianRed;
    const extern sf::Color Crimson;
    const extern sf::Color FireBrick;
    const extern sf::Color DarkRed;
    const extern sf::Color Red;
}

namespace Oranges
{
    const extern sf::Color OrangeRed;
    const extern sf::Color Tomato;
    const extern sf::Color Coral;
    const extern sf::Color DarkOrange;
    const extern sf::Color Orange;
}

namespace Yellows {
    const extern sf::Color Yellow;
    const extern sf::Color LightYellow;
    const extern sf::Color LemonChiffon;
    const extern sf::Color LightGoldenrodYellow;
    const extern sf::Color PapayaWhip;
    const extern sf::Color Moccasin;
    const extern sf::Color PeachPuff;
    const extern sf::Color PaleGoldenrod;
    const extern sf::Color Khaki;
    const extern sf::Color DarkKhaki;
    const extern sf::Color Gold;
}

namespace Browns
{
    const extern sf::Color Cornsilk;
    const extern sf::Color BlanchedAlmond;
    const extern sf::Color Bisque;
    const extern sf::Color NavajoWhite;
    const extern sf::Color Wheat;
    const extern sf::Color BurlyWood;
    const extern sf::Color Tan;
    const extern sf::Color RosyBrown;
    const extern sf::Color SandyBrown;
    const extern sf::Color Goldenrod;
    const extern sf::Color DarkGoldenrod;
    const extern sf::Color Peru;
    const extern sf::Color Chocolate;
    const extern sf::Color SaddleBrown;
    const extern sf::Color Sienna;
    const extern sf::Color Brown;
    const extern sf::Color Maroon;
}

namespace Greens
{
    const extern sf::Color DarkOliveGreen;
    const extern sf::Color Olive;
    const extern sf::Color OliveDrab;
    const extern sf::Color YellowGreen;
    const extern sf::Color LimeGreen;
    const extern sf::Color Lime;
    const extern sf::Color LawnGreen;
    const extern sf::Color Chartreuse;
    const extern sf::Color GreenYellow;
    const extern sf::Color SpringGreen;
    const extern sf::Color MediumSpringGreen;
    const extern sf::Color LightGreen;
    const extern sf::Color PaleGreen;
    const extern sf::Color DarkSeaGreen;
    const extern sf::Color MediumAquamarine;
    const extern sf::Color MediumSeaGreen;
    const extern sf::Color SeaGreen;
    const extern sf::Color ForestGreen;
    const extern sf::Color Green;
    const extern sf::Color DarkGreen;
}

namespace Cyans
{
    const extern sf::Color Aqua;
    const extern sf::Color Cyan;
    const extern sf::Color LightCyan;
    const extern sf::Color PaleTurquoise;
    const extern sf::Color Aquamarine;
    const extern sf::Color Turquoise;
    const extern sf::Color MediumTurquoise;
    const extern sf::Color DarkTurquoise;
    const extern sf::Color LightSeaGreen;
    const extern sf::Color CadetBlue;
    const extern sf::Color DarkCyan;
    const extern sf::Color Teal;
}

namespace Blues {
    const extern sf::Color LightSteelBlue;
    const extern sf::Color PowderBlue;
    const extern sf::Color LightBlue;
    const extern sf::Color SkyBlue;
    const extern sf::Color LightSkyBlue;
    const extern sf::Color DeepSkyBlue;
    const extern sf::Color DodgerBlue;
    const extern sf::Color CornflowerBlue;
    const extern sf::Color SteelBlue;
    const extern sf::Color RoyalBlue;
    const extern sf::Color Blue;
    const extern sf::Color MediumBlue;
    const extern sf::Color DarkBlue;
    const extern sf::Color Navy;
    const extern sf::Color MidnightBlue;
}

namespace Purples
{
    const extern sf::Color Lavender;
    const extern sf::Color Thistle;
    const extern sf::Color Plum;
    const extern sf::Color Violet;
    const extern sf::Color Orchid;
    const extern sf::Color Fuchsia;
    const extern sf::Color Magenta;
    const extern sf::Color MediumOrchid;
    const extern sf::Color MediumPurple;
    const extern sf::Color BlueViolet;
    const extern sf::Color DarkViolet;
    const extern sf::Color DarkOrchid;
    const extern sf::Color DarkMagenta;
    const extern sf::Color Purple;
    const extern sf::Color Indigo;
    const extern sf::Color DarkSlateBlue;
    const extern sf::Color SlateBlue;
    const extern sf::Color MediumSlateBlue;
}

namespace Whites
{
    const extern sf::Color White;
    const extern sf::Color Snow;
    const extern sf::Color Honeydew;
    const extern sf::Color MintCream;
    const extern sf::Color Azure;
    const extern sf::Color AliceBlue;
    const extern sf::Color GhostWhite;
    const extern sf::Color WhiteSmoke;
    const extern sf::Color Seashell;
    const extern sf::Color Beige;
    const extern sf::Color OldLace;
    const extern sf::Color FloralWhite;
    const extern sf::Color Ivory;
    const extern sf::Color AntiqueWhite;
    const extern sf::Color Linen;
    const extern sf::Color LavenderBlush;
    const extern sf::Color MistyRose;
}

namespace Grays
{
    const extern sf::Color Gainsboro;
    const extern sf::Color LightGray;
    const extern sf::Color Silver;
    const extern sf::Color DarkGray;
    const extern sf::Color Gray;
    const extern sf::Color DimGray;
    const extern sf::Color LightSlateGray;
    const extern sf::Color SlateGray;
    const extern sf::Color DarkSlateGray;
    const extern sf::Color Black;
}

} // namespace sfvg

#endif // SFVG_COLOR_HPP
