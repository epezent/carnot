#ifndef SFVG_PROPERTY_HPP
#define SFVG_PROPERTY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFVG/Animation/Detail/Detail.hpp>
#include <SFVG/Animation/Tween.hpp>
#include <iostream>

namespace sfvg {

enum class Mode {
    Absolute = 0,  ///< Property value is used as an absolute
    Delta    = 1   ///< Property value is used relative to last KeyFrame
};

template <typename V>
struct Property {
    Mode m_mode;                             ///< Property mode
    V (*m_func)(const V&, const V&, float);  ///< tweening function
    V m_setValue;                            ///< value set by caller
    V m_absValue;                            ///< absolute value used internally
};

/// Helper macro for quickly defining new Animation Properties.
///
/// This simply replaces NAME, TYPE, SETTER, GETTER with the Property class
/// name, the type the property operates on, and the name of the property's
/// setter and getter functions.
#define SFVG_PROPERTY(NAME, TYPE, SETTER, GETTER)   \
    struct NAME : public Property<TYPE> {           \
                                                    \
        template <typename S>                       \
        void set(S* subject, const TYPE & value) {  \
            subject->SETTER(value);                 \
        }                                           \
                                                    \
        template <typename S>                       \
        TYPE get(S* subject) {                      \
            return subject->GETTER();               \
        }                                           \
    };

//==============================================================================
// BUILT-IN PROPERTIES
//==============================================================================

//            NAME          TYPE          SETTER           GETTER
SFVG_PROPERTY(Position,     sf::Vector2f, setPosition,     getPosition);
SFVG_PROPERTY(Rotation,     float,        setRotation,     getRotation);
SFVG_PROPERTY(Scale,        sf::Vector2f, setScale,        getScale);
SFVG_PROPERTY(FillColor,    sf::Color,    setFillColor,    getFillColor);
SFVG_PROPERTY(FillGradient, Gradient,     setFillGradient, getFillGradient);

}  // namespace sfvg

#endif  // SFVG_PROPERTY_HPP
