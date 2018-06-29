#ifndef SFVG_PROPERTY_HPP
#define SFVG_PROPERTY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFVG/Animation/Detail/Detail.hpp>
#include <SFVG/Animation/Tween.hpp>

namespace sfvg {

enum class Mode {
    Absolute = 0,  ///< Property value is used as an absolute
    Relative = 1   ///< Property value is used relative to last KeyFrame
};

template <typename V>
struct Property {
    Mode mode                            = Mode::Absolute;
    V (*func)(const V&, const V&, float) = Tween::Linear<V>;
    V value                              = V();
    V absValue = V();
};

//==============================================================================
// SFVG PROPERTIES
//==============================================================================

struct Rotation : public Property<float> {
    template <typename S>
    void set(S* subject) {
        subject->setRotation(value);
    }
};

struct Position : public Property<sf::Vector2f> {
    template <typename S>
    void set(S* subject) {
        subject->setPosition(value);
    }
};

struct Scale : public Property<sf::Vector2f> {
    template <typename S>
    void set(S* subject) {
        subject->setScale(value);
    }
};

struct FillColor : public Property<sf::Color> {
    template <typename S>
    void set(S* subject) {
        subject->setFillColor(value);
    }
};

}  // namespace sfvg

#endif  // SFVG_PROPERTY_HPP
