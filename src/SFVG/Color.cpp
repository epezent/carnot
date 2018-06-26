#include <SFVG/Color.hpp>
#include <SFVG/Math.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace sfvg {

sf::Color Color::cmyk(float c, float m, float y, float k) {
    clamp(c);
    clamp(m);
    clamp(y);
    clamp(k);
    sf::Uint8 r = static_cast<sf::Uint8>(255.0f * (1.0f - c) * (1.0f - k));
    sf::Uint8 g = static_cast<sf::Uint8>(255.0f * (1.0f - m) * (1.0f - k));
    sf::Uint8 b = static_cast<sf::Uint8>(255.0f * (1.0f - y) * (1.0f - k));
    return sf::Color(r,g,b);
}

sf::Color Color::hsv(float h, float s, float v) {
    clamp(h);
    clamp(s);
    clamp(v);
    float hh, p, q, t, ff;
    if (s == 0.0f){
        sf::Uint8 rgb = static_cast<sf::Uint8>(255.0f * v);
        return sf::Color(rgb, rgb, rgb);
    }
    hh = h;
    if (hh == 1.0f)
        hh = 0.0f;
    hh *= 6.0f;
    long i = (long)hh;
    ff = hh - i;
    p = v * (1.0f - s);
    q = v * (1.0f - s * ff);
    t = v * (1.0f - s * (1.0f - ff));
    sf::Uint8 r, g, b;
    switch(i) {
    case 0:
        r = static_cast<sf::Uint8>(255.0f * v);
        g = static_cast<sf::Uint8>(255.0f * t);
        b = static_cast<sf::Uint8>(255.0f * p);
        break;
    case 1:
        r = static_cast<sf::Uint8>(255.0f * q);
        g = static_cast<sf::Uint8>(255.0f * v);
        b = static_cast<sf::Uint8>(255.0f * p);
        break;
    case 2:
        r = static_cast<sf::Uint8>(255.0f * p);
        g = static_cast<sf::Uint8>(255.0f * v);
        b = static_cast<sf::Uint8>(255.0f * t);
        break;
    case 3:
        r = static_cast<sf::Uint8>(255.0f * p);
        g = static_cast<sf::Uint8>(255.0f * q);
        b = static_cast<sf::Uint8>(255.0f * v);
        break;
    case 4:
        r = static_cast<sf::Uint8>(255.0f * t);
        g = static_cast<sf::Uint8>(255.0f * p);
        b = static_cast<sf::Uint8>(255.0f * v);
        break;
    case 5:
    default:
        r = static_cast<sf::Uint8>(255.0f * v);
        g = static_cast<sf::Uint8>(255.0f * p);
        b = static_cast<sf::Uint8>(255.0f * q);
        break;
    }

    return sf::Color(r,g,b);
}

sf::Color Color::hex(std::string hex) {
    if (hex[0] == '#')
        hex.erase(0,1);
    unsigned int r, g, b, a;
    if (hex.length() == 6) {
        #ifdef _WIN32
            sscanf_s(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        #else
            sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        #endif
        return sf::Color(static_cast<sf::Uint8>(r),static_cast<sf::Uint8>(g),static_cast<sf::Uint8>(b));
    }
    else if (hex.length() == 8) {
        #ifdef _WIN32
            sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
        #else
            sscanf(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
        #endif
        return sf::Color(static_cast<sf::Uint8>(r),static_cast<sf::Uint8>(g),static_cast<sf::Uint8>(b),static_cast<sf::Uint8>(a));
    }
    else
        return sf::Color();
}


//==============================================================================
// NAMED COLORS
//==============================================================================

namespace Pinks
{
    const sf::Color Pink(255,192,203);
    const sf::Color LightPink(255,182,193);
    const sf::Color HotPink(255,105,180);
    const sf::Color DeepPink(255,20,147);
    const sf::Color PaleVioletRed(219,112,147);
    const sf::Color MediumVioletRed(199,21,133);
}

namespace Reds
{
    const sf::Color LightSalmon(255,160,122);
    const sf::Color Salmon(250,128,114);
    const sf::Color DarkSalmon(233,150,122);
    const sf::Color LightCoral(240,128,128);
    const sf::Color IndianRed(205,92,92);
    const sf::Color Crimson(220,20,60);
    const sf::Color FireBrick(178,34,34);
    const sf::Color DarkRed(139,0,0);
    const sf::Color Red(255,0,0);
}

namespace Oranges
{
    const sf::Color OrangeRed(255,69,0);
    const sf::Color Tomato(255,99,71);
    const sf::Color Coral(255,127,80);
    const sf::Color DarkOrange(255,140,0);
    const sf::Color Orange(255,165,0);
}

namespace Yellows
{
    const sf::Color Yellow(255,255,0);
    const sf::Color LightYellow(255,255,224);
    const sf::Color LemonChiffon(255,250,205);
    const sf::Color LightGoldenrodYellow (250,250,210);
    const sf::Color PapayaWhip(255,239,213);
    const sf::Color Moccasin(255,228,181);
    const sf::Color PeachPuff(255,218,185);
    const sf::Color PaleGoldenrod(238,232,170);
    const sf::Color Khaki(240,230,140);
    const sf::Color DarkKhaki(189,183,107);
    const sf::Color Gold(255,215,0);
}

namespace Browns
{
    const sf::Color Cornsilk(255,248,220);
    const sf::Color BlanchedAlmond(255,235,205);
    const sf::Color Bisque(255,228,196);
    const sf::Color NavajoWhite(255,222,173);
    const sf::Color Wheat(245,222,179);
    const sf::Color BurlyWood(222,184,135);
    const sf::Color Tan(210,180,140);
    const sf::Color RosyBrown(188,143,143);
    const sf::Color SandyBrown(244,164,96);
    const sf::Color Goldenrod(218,165,32);
    const sf::Color DarkGoldenrod(184,134,11);
    const sf::Color Peru(205,133,63);
    const sf::Color Chocolate(210,105,30);
    const sf::Color SaddleBrown(139,69,19);
    const sf::Color Sienna(160,82,45);
    const sf::Color Brown(165,42,42);
    const sf::Color Maroon(128,0,0);
}

namespace Greens
{
    const sf::Color DarkOliveGreen(85,107,47);
    const sf::Color Olive(128,128,0);
    const sf::Color OliveDrab(107,142,35);
    const sf::Color YellowGreen(154,205,50);
    const sf::Color LimeGreen(50,205,50);
    const sf::Color Lime(0,255,0);
    const sf::Color LawnGreen(124,252,0);
    const sf::Color Chartreuse(127,255,0);
    const sf::Color GreenYellow(173,255,47);
    const sf::Color SpringGreen(0,255,127);
    const sf::Color MediumSpringGreen (0,250,154);
    const sf::Color LightGreen(144,238,144);
    const sf::Color PaleGreen(152,251,152);
    const sf::Color DarkSeaGreen(143,188,143);
    const sf::Color MediumAquamarine(102,205,170);
    const sf::Color MediumSeaGreen(60,179,113);
    const sf::Color SeaGreen(46,139,87);
    const sf::Color ForestGreen(34,139,34);
    const sf::Color Green(0,128,0);
    const sf::Color DarkGreen(0,100,0);
}

namespace Cyans
{
    const sf::Color Aqua(0,255,255);
    const sf::Color Cyan(0,255,255);
    const sf::Color LightCyan(224,255,255);
    const sf::Color PaleTurquoise(175,238,238);
    const sf::Color Aquamarine(127,255,212);
    const sf::Color Turquoise(64,224,208);
    const sf::Color MediumTurquoise(72,209,204);
    const sf::Color DarkTurquoise(0,206,209);
    const sf::Color LightSeaGreen(32,178,170);
    const sf::Color CadetBlue(95,158,160);
    const sf::Color DarkCyan(0,139,139);
    const sf::Color Teal(0,128,128);
}

namespace Blues
{
    const sf::Color LightSteelBlue(176,196,222);
    const sf::Color PowderBlue(176,224,230);
    const sf::Color LightBlue(173,216,230);
    const sf::Color SkyBlue(135,206,235);
    const sf::Color LightSkyBlue(135,206,250);
    const sf::Color DeepSkyBlue(0,191,255);
    const sf::Color DodgerBlue(30,144,255);
    const sf::Color CornflowerBlue(100,149,237);
    const sf::Color SteelBlue(70,130,180);
    const sf::Color RoyalBlue(65,105,225);
    const sf::Color Blue(0,0,255);
    const sf::Color MediumBlue(0,0,205);
    const sf::Color DarkBlue(0,0,139);
    const sf::Color Navy(0,0,128);
    const sf::Color MidnightBlue(25,25,112);
}

namespace Purples
{
    const sf::Color Lavender(230,230,250);
    const sf::Color Thistle(216,191,216);
    const sf::Color Plum(221,160,221);
    const sf::Color Violet(238,130,238);
    const sf::Color Orchid(218,112,214);
    const sf::Color Fuchsia(255,0,255);
    const sf::Color Magenta(255,0,255);
    const sf::Color MediumOrchid(186,85,211);
    const sf::Color MediumPurple(147,112,219);
    const sf::Color BlueViolet(138,43,226);
    const sf::Color DarkViolet(148,0,211);
    const sf::Color DarkOrchid(153,50,204);
    const sf::Color DarkMagenta(139,0,139);
    const sf::Color Purple(128,0,128);
    const sf::Color Indigo(75,0,130);
    const sf::Color DarkSlateBlue(72,61,139);
    const sf::Color SlateBlue(106,90,205);
    const sf::Color MediumSlateBlue (123,104,238);
}

namespace Whites
{
    const sf::Color White(255,255,255);
    const sf::Color Snow(255,250,250);
    const sf::Color Honeydew(240,255,240);
    const sf::Color MintCream(245,255,250);
    const sf::Color Azure(240,255,255);
    const sf::Color AliceBlue(240,248,255);
    const sf::Color GhostWhite(248,248,255);
    const sf::Color WhiteSmoke(245,245,245);
    const sf::Color Seashell(255,245,238);
    const sf::Color Beige(245,245,220);
    const sf::Color OldLace(253,245,230);
    const sf::Color FloralWhite(255,250,240);
    const sf::Color Ivory(255,255,240);
    const sf::Color AntiqueWhite(250,235,215);
    const sf::Color Linen(250,240,230);
    const sf::Color LavenderBlush(255,240,245);
    const sf::Color MistyRose(255,228,225);
}

namespace Grays
{
    const sf::Color Gainsboro(220,220,220);
    const sf::Color LightGray(211,211,211);
    const sf::Color Silver(192,192,192);
    const sf::Color DarkGray(169,169,169);
    const sf::Color Gray(128,128,128);
    const sf::Color DimGray(105,105,105);
    const sf::Color LightSlateGray(119,136,153);
    const sf::Color SlateGray(112,128,144);
    const sf::Color DarkSlateGray(47,79,79);
    const sf::Color Black(0,0,0);
}

} // namespace sfvg
