#include <Graphics/Color.hpp>
#include <Common/Math.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace carnot {

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

Color hexCode(std::string hex) {
    return static_cast<Color>(toRgb(hex));
}

float luminance(const Color& color) {
    return ( 0.299f * color.r + 0.587f * color.g + 0.114f * color.b) / 255.0f;
}

} // namespace carnot
