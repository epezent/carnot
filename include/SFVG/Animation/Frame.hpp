#ifndef SFVG_KEY_FRAME_HPP
#define SFVG_KEY_FRAME_HPP

#include <SFVG/Animation/Property.hpp>

namespace sfvg {

template <typename... Ps>
class Animation;

template <typename... Ps>
class Frame {
public:

    /// Sets the Frame's Property P to a V value and optionally sets the
    /// tweening function to be used (defaults to linear interpolation)
    template <typename P, typename V>
    Frame<Ps...>& absolute(V value, V (*func)(const V&, const V&, float) = Tween::Linear<V>);

    /// Sets the Frame's Property P to a V value and optionally sets the
    /// tweening function to be used (defaults to linear interpolation)
    template <typename P, typename V>
    Frame<Ps...>& delta(V value, V (*func)(const V&, const V&, float) = Tween::Linear<V>);

    /// Gets a Property P from the Frame
    template <typename P>
    P& get();

    /// Applies the Frame's Properties to a Subject S
    template <typename S>
    void applyTo(S* subject);

    /// Sets the Frame's Properties from a Subject S
    template <typename S>
    void setFrom(S* subject);

private:
    friend class Animation<Ps...>;
    std::tuple<Ps...> m_properties;  ///< tuple of FrameProperties
};

//==============================================================================
// DEFINITIONS
//==============================================================================

namespace detail {

template <typename S>
struct ApplyToFunctor {
    ApplyToFunctor(S* subject) : m_subject(subject) {}
    template <typename P>
    void operator()(P&& p) {
        p.set(m_subject, p.m_absValue);
    }
    S* m_subject;
};

template <typename S>
struct SetFromFunctor {
    SetFromFunctor(S* subject) : m_subject(subject) {}
    template <typename P>
    void operator()(P&& p) {
        p.m_setValue = p.get(m_subject);
        p.m_absValue = p.m_setValue;
    }
    S* m_subject;
};

} // namespace detail

template <typename... Ps>
template <typename P, typename V>
Frame<Ps...>& Frame<Ps...>::absolute(V value, V (*func)(const V&, const V&, float))
{
    std::get<P>(m_properties).m_setValue = value;
    std::get<P>(m_properties).m_absValue = value;
    std::get<P>(m_properties).m_func  = func;
    std::get<P>(m_properties).m_mode = Mode::Absolute;
    return *this;
}

template <typename... Ps>
template <typename P, typename V>
Frame<Ps...>& Frame<Ps...>::delta(V value, V (*func)(const V&, const V&, float))
{
    std::get<P>(m_properties).m_setValue = value;
    std::get<P>(m_properties).m_absValue = value;
    std::get<P>(m_properties).m_func  = func;
    std::get<P>(m_properties).m_mode = Mode::Delta;
    return *this;
}

template <typename... Ps>
template <typename P>
P& Frame<Ps...>::get() {
    return std::get<P>(m_properties);
}

template <typename... Ps>
template <typename S>
void Frame<Ps...>::applyTo(S* subject) {
    detail::for_each_in_tuple(m_properties, detail::ApplyToFunctor<S>(subject));
}

template <typename... Ps>
template <typename S>
void Frame<Ps...>::setFrom(S* subject) {
    detail::for_each_in_tuple(m_properties, detail::SetFromFunctor<S>(subject));
}

}  // namespace sfvg

#endif  // SFVG_KEY_FRAME_HPP
