#include <SFVG/Graphics/Color.hpp>
#include <SFVG/Common/Math.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace sfvg {

RGB::operator Color() const {
    Color color;
    color.r = static_cast<Uint8>(255.0f * r);
    color.g = static_cast<Uint8>(255.0f * g);
    color.b = static_cast<Uint8>(255.0f * b);
    color.a = static_cast<Uint8>(255.0f * a);
    return color;
}

HSV::operator Color() const {
    Color color;
    return static_cast<Color>(toRgb(*this));
}


RGB toRgb(const Color& color) {
    return RGB{(static_cast<float>(color.r)/255.0f),
               (static_cast<float>(color.g)/255.0f),
               (static_cast<float>(color.b)/255.0f),
               (static_cast<float>(color.a)/255.0f)};
}

RGB toRgb(const HSV& in) {
    float      hh, p, q, t, ff;
    long        i;
    RGB         out;

    if(in.s <= 0.0f) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        out.a = in.a;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0f) hh = 0.0f;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0f - in.s);
    q = in.v * (1.0f - (in.s * ff));
    t = in.v * (1.0f - (in.s * (1.0f - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }

    out.a = in.a;
    return out;
}

RGB toRgb(std::string hex) {
    if (hex[0] == '#')
        hex.erase(0,1);
    unsigned int r, g, b, a;
    if (hex.length() == 6) {
        #ifdef _WIN32
            sscanf_s(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        #else
            sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        #endif
        return RGB{static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f,  1.0f};
    }
    else if (hex.length() == 8) {
        #ifdef _WIN32
            sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
        #else
            sscanf(hex.c_str(), "%02x%02x%02x%02x",  &r, &g, &b, &a);
        #endif
        return RGB{static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f};
    }
    else
        return RGB{1.0f,1.0f,1.0f,1.0f};
}

HSV toHsv(const RGB& in) {
    HSV out;
    out.a = in.a;
    float      min, max, delta;
    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;
    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;
    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001f)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0f ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0f;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0f + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0f + ( in.r - in.g ) / delta;  // between magenta & cyan
    out.h *= 60.0f;                              // degrees
    if( out.h < 0.0f )
        out.h += 360.0f;
    return out;
}

HSV toHsv(const Color& color) {
    return toHsv(toRgb(color));
}

HSV toHsv(std::string hex) {
    return toHsv(toRgb(hex));
}

//==============================================================================
// Named Colors
//==============================================================================

namespace Pinks
{
    const Color Pink(255,192,203);
    const Color LightPink(255,182,193);
    const Color HotPink(255,105,180);
    const Color DeepPink(255,20,147);
    const Color PaleVioletRed(219,112,147);
    const Color MediumVioletRed(199,21,133);
}

namespace Reds
{
    const Color LightSalmon(255,160,122);
    const Color Salmon(250,128,114);
    const Color DarkSalmon(233,150,122);
    const Color LightCoral(240,128,128);
    const Color IndianRed(205,92,92);
    const Color Crimson(220,20,60);
    const Color FireBrick(178,34,34);
    const Color DarkRed(139,0,0);
    const Color Red(255,0,0);
}

namespace Oranges
{
    const Color OrangeRed(255,69,0);
    const Color Tomato(255,99,71);
    const Color Coral(255,127,80);
    const Color DarkOrange(255,140,0);
    const Color Orange(255,165,0);
}

namespace Yellows
{
    const Color Yellow(255,255,0);
    const Color LightYellow(255,255,224);
    const Color LemonChiffon(255,250,205);
    const Color LightGoldenrodYellow (250,250,210);
    const Color PapayaWhip(255,239,213);
    const Color Moccasin(255,228,181);
    const Color PeachPuff(255,218,185);
    const Color PaleGoldenrod(238,232,170);
    const Color Khaki(240,230,140);
    const Color DarkKhaki(189,183,107);
    const Color Gold(255,215,0);
}

namespace Browns
{
    const Color Cornsilk(255,248,220);
    const Color BlanchedAlmond(255,235,205);
    const Color Bisque(255,228,196);
    const Color NavajoWhite(255,222,173);
    const Color Wheat(245,222,179);
    const Color BurlyWood(222,184,135);
    const Color Tan(210,180,140);
    const Color RosyBrown(188,143,143);
    const Color SandyBrown(244,164,96);
    const Color Goldenrod(218,165,32);
    const Color DarkGoldenrod(184,134,11);
    const Color Peru(205,133,63);
    const Color Chocolate(210,105,30);
    const Color SaddleBrown(139,69,19);
    const Color Sienna(160,82,45);
    const Color Brown(165,42,42);
    const Color Maroon(128,0,0);
}

namespace Greens
{
    const Color DarkOliveGreen(85,107,47);
    const Color Olive(128,128,0);
    const Color OliveDrab(107,142,35);
    const Color YellowGreen(154,205,50);
    const Color LimeGreen(50,205,50);
    const Color Lime(0,255,0);
    const Color LawnGreen(124,252,0);
    const Color Chartreuse(127,255,0);
    const Color GreenYellow(173,255,47);
    const Color SpringGreen(0,255,127);
    const Color MediumSpringGreen (0,250,154);
    const Color LightGreen(144,238,144);
    const Color PaleGreen(152,251,152);
    const Color DarkSeaGreen(143,188,143);
    const Color MediumAquamarine(102,205,170);
    const Color MediumSeaGreen(60,179,113);
    const Color SeaGreen(46,139,87);
    const Color ForestGreen(34,139,34);
    const Color Green(0,128,0);
    const Color DarkGreen(0,100,0);
}

namespace Cyans
{
    const Color Aqua(0,255,255);
    const Color Cyan(0,255,255);
    const Color LightCyan(224,255,255);
    const Color PaleTurquoise(175,238,238);
    const Color Aquamarine(127,255,212);
    const Color Turquoise(64,224,208);
    const Color MediumTurquoise(72,209,204);
    const Color DarkTurquoise(0,206,209);
    const Color LightSeaGreen(32,178,170);
    const Color CadetBlue(95,158,160);
    const Color DarkCyan(0,139,139);
    const Color Teal(0,128,128);
}

namespace Blues
{
    const Color LightSteelBlue(176,196,222);
    const Color PowderBlue(176,224,230);
    const Color LightBlue(173,216,230);
    const Color SkyBlue(135,206,235);
    const Color LightSkyBlue(135,206,250);
    const Color DeepSkyBlue(0,191,255);
    const Color DodgerBlue(30,144,255);
    const Color CornflowerBlue(100,149,237);
    const Color SteelBlue(70,130,180);
    const Color RoyalBlue(65,105,225);
    const Color Blue(0,0,255);
    const Color MediumBlue(0,0,205);
    const Color DarkBlue(0,0,139);
    const Color Navy(0,0,128);
    const Color MidnightBlue(25,25,112);
}

namespace Purples
{
    const Color Lavender(230,230,250);
    const Color Thistle(216,191,216);
    const Color Plum(221,160,221);
    const Color Violet(238,130,238);
    const Color Orchid(218,112,214);
    const Color Fuchsia(255,0,255);
    const Color Magenta(255,0,255);
    const Color MediumOrchid(186,85,211);
    const Color MediumPurple(147,112,219);
    const Color BlueViolet(138,43,226);
    const Color DarkViolet(148,0,211);
    const Color DarkOrchid(153,50,204);
    const Color DarkMagenta(139,0,139);
    const Color Purple(128,0,128);
    const Color Indigo(75,0,130);
    const Color DarkSlateBlue(72,61,139);
    const Color SlateBlue(106,90,205);
    const Color MediumSlateBlue (123,104,238);
}

namespace Whites
{
    const Color White(255,255,255);
    const Color Snow(255,250,250);
    const Color Honeydew(240,255,240);
    const Color MintCream(245,255,250);
    const Color Azure(240,255,255);
    const Color AliceBlue(240,248,255);
    const Color GhostWhite(248,248,255);
    const Color WhiteSmoke(245,245,245);
    const Color Seashell(255,245,238);
    const Color Beige(245,245,220);
    const Color OldLace(253,245,230);
    const Color FloralWhite(255,250,240);
    const Color Ivory(255,255,240);
    const Color AntiqueWhite(250,235,215);
    const Color Linen(250,240,230);
    const Color LavenderBlush(255,240,245);
    const Color MistyRose(255,228,225);
}

namespace Grays
{
    const Color Gainsboro(220,220,220);
    const Color LightGray(211,211,211);
    const Color Silver(192,192,192);
    const Color DarkGray(169,169,169);
    const Color Gray(128,128,128);
    const Color DimGray(105,105,105);
    const Color LightSlateGray(119,136,153);
    const Color SlateGray(112,128,144);
    const Color DarkSlateGray(47,79,79);
    const Color Black(0,0,0);
    const Color Gray10(26,26,26);
    const Color Gray20(51,51,51);
    const Color Gray30(77,77,77);
    const Color Gray40(102,102,102);
    const Color Gray50(128,128,128);
    const Color Gray60(153,153,153);
    const Color Gray70(179,179,179);
    const Color Gray80(204,204,204);
    const Color Gray90(230,230,230);
}

} // namespace sfvg
