#ifndef SFVG_KEY_FRAME_HPP
#define SFVG_KEY_FRAME_HPP

#include <SFVG/Animation/Property.hpp>

namespace sfvg {

template <typename... Ps>
class Animation;

template <typename... Ps>
class Frame {
public:
    template <typename P, typename V>
    Frame<Ps...>& set(V value,
                      V (*func)(const V&, const V&, float) = Tween::Linear<V>)
    {
        std::get<P>(m_properties).value = value;
        std::get<P>(m_properties).absValue = value;
        std::get<P>(m_properties).func  = func;
        std::get<P>(m_properties).mode = Mode::Absolute;
        return *this;
    }

    template <typename P, typename V>
    Frame<Ps...>& change(V value,
                      V (*func)(const V&, const V&, float) = Tween::Linear<V>)
    {
        std::get<P>(m_properties).value = value;
        std::get<P>(m_properties).absValue = value;
        std::get<P>(m_properties).func  = func;
        std::get<P>(m_properties).mode = Mode::Relative;
        return *this;
    }

    template <typename P>
    P& get() {
        return std::get<P>(m_properties);
    }

    template <typename TSubject>
    void set(TSubject* subject) {
        detail::for_each_in_tuple(m_properties,
                                  detail::set_functor<TSubject>(subject));
    }

private:
    friend class Animation<Ps...>;
    std::tuple<Ps...> m_properties;
};

}  // namespace sfvg

#endif  // SFVG_KEY_FRAME_HPP
