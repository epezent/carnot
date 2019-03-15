#pragma once
#include <Carnot/Common/Imports.hpp>

namespace carnot {

template <typename T> inline void alignTopLeft(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left,                     rect.top);
}

template <typename T> inline void alignTopCenter(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width * 0.5f, rect.top);
}

template <typename T> inline void alignTopRight(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width,        rect.top);
}

template <typename T> inline void alignCenterLeft(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left,                     rect.top + rect.height * 0.5f);
}

template <typename T> inline void alignCenter(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f);
}

template <typename T> inline void alignCenterRight(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width,        rect.top + rect.height * 0.5f);
}

template <typename T> inline void alignBottomLeft(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left,                     rect.top + rect.height);
}

template <typename T> inline void alignBottomCenter(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width * 0.5f, rect.top + rect.height);
}

template <typename T> inline void alignBottomRight(T& t) {
    FloatRect rect = t.getLocalBounds();
    t.setOrigin(rect.left + rect.width,        rect.top + rect.height);
}


} // namespace carnot
