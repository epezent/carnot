#pragma once

#include <Carnot/Animation/Detail/Detail.hpp>
#include <Carnot/Animation/Property.hpp>

namespace carnot {

template <typename... Ps>
class Animation;

template <typename... Ps>
class Frame {
public:

    /// Sets the Frame's Property P to an absolute V value and optionally sets
    /// the tweening function to be used (defaults to Tween::Linear)
    template <typename P, typename V>
    Frame<Ps...>& absolute(V value, V (*func)(const V&, const V&, float) = Tween::Linear);

    /// Sets the Frame's Property P to a delta V value and optionally sets the
    /// tweening function to be used (defaults to Tween::Linear)
    template <typename P, typename V>
    Frame<Ps...>& delta(V value, V (*func)(const V&, const V&, float) = Tween::Linear);

    /// Gets a Property P from the Frame
    template <typename P>
    P& get();

    /// Applies the Frame's Properties to a Subject S
    template <typename S>
    void applyTo(S* subject);

    /// Sets all the Frame's Properties from a Subject's current state and
    /// returns a reference to the itself (defaults each Property's tweening
    /// function to Tween::Linear)
    template <typename S>
    Frame<Ps...>& setFrom(S* subject);

    /// Sets a single Frame's Properties from Subject's current state and
    /// returns a reference to the Property (defaults the Property's tweening
    /// function to Tween::Linear)
    template <typename P, typename S>
    P& setFrom(S* subject);

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
        if (p.type != PropertyType::Skip)
            p.set(m_subject, p.absValue);
    }
    S* m_subject;
};

template <typename S>
struct SetFromFunctor {
    SetFromFunctor(S* subject) : m_subject(subject) {}
    template <typename P>
    void operator()(P&& p) {
        p.type     = PropertyType::Absolute;
        p.tween    = Tween::Linear;
        p.setValue = p.get(m_subject);
        p.absValue = p.setValue;
    }
    S* m_subject;
};

} // namespace detail

template <typename... Ps>
template <typename P, typename V>
Frame<Ps...>& Frame<Ps...>::absolute(V value, V (*func)(const V&, const V&, float))
{
    get<P>().setValue = value;
    get<P>().absValue = value;
    get<P>().tween  = func;
    get<P>().type = PropertyType::Absolute;
    return *this;
}

template <typename... Ps>
template <typename P, typename V>
Frame<Ps...>& Frame<Ps...>::delta(V value, V (*func)(const V&, const V&, float))
{
    get<P>().setValue = value;
    get<P>().absValue = value;
    get<P>().tween  = func;
    get<P>().type = PropertyType::Delta;
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
Frame<Ps...>& Frame<Ps...>::setFrom(S* subject) {
    detail::for_each_in_tuple(m_properties, detail::SetFromFunctor<S>(subject));
    return *this;
}

template <typename... Ps>
template <typename P, typename S>
P& Frame<Ps...>::setFrom(S* subject) {
    P& p = get<P>();
    p.type = PropertyType::Absolute;
    p.setValue = p.get(subject);
    p.absValue = p.setValue;
    p.tween = Tween::Linear;
    return p;
}

}  // namespace carnot
