#pragma once

#include <SFVG/Graphics/Points.hpp>
#include <SFVG/Tween.hpp>
#include <iostream>
#include <vector>

namespace sfvg {

enum class PropertyType {
    Skip     = 0,  ///< Property is skpped (default)
    Absolute = 1,  ///< Property is an absolute keyframe property
    Delta    = 2,  ///< Property is delta keyframe property
    Tweened  = 3   ///< Property is a tweened frame property
};

template <typename V>
struct Property {
    PropertyType type;                      ///< Property mode
    V (*tween)(const V&, const V&, float);  ///< tweening function
    V setValue;                             ///< value set by caller
    V absValue;                             ///< absolute value used internally
};

/// Helper macro for quickly defining new Animation Properties.
///
/// This simply replaces NAME, TYPE, SETTER, GETTER with a chosen name for the
/// Property, the type the Property operates on, and the name of the getter and
/// setter functions the Property should call from a Subject
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

#define SFVG_IDX_PROPERTY(NAME, TYPE, SETTER, GETTER)   \
    template <std::size_t Idx>                         \
    struct NAME : public Property<TYPE> {               \
                                                        \
        template <typename S>                           \
        void set(S* subject, const TYPE & value) {      \
            subject->SETTER(Idx, value);                \
        }                                               \
                                                        \
        template <typename S>                           \
        TYPE get(S* subject) {                          \
            return subject->GETTER(Idx);                \
        }                                               \
    };

//==============================================================================
// BUILT-IN PROPERTIES
//==============================================================================

//            NAME          TYPE          SETTER           GETTER
SFVG_PROPERTY(PPosition,     sf::Vector2f, setPosition,     getPosition);
SFVG_PROPERTY(PRotation,     float,        setRotation,     getRotation);
SFVG_PROPERTY(PScale,        sf::Vector2f, setScale,        getScale);
SFVG_PROPERTY(PColor,        sf::Color,    setColor,        getColor);
SFVG_PROPERTY(PGradient,     Gradient,     setGradient,     getGradient);
SFVG_PROPERTY(PPoints,       Points,       setPoints,       getPoints);
SFVG_PROPERTY(PRadii,  std::vector<float>, setRadii,        getRadii);

}  // namespace sfvg
