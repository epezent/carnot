#ifndef SFVG_KEY_FRAME_HPP
#define SFVG_KEY_FRAME_HPP

#include <tuple>

namespace sfvg {

namespace detail {
template <int... Is>
struct seq {};

template <int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template <int... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template <typename T, typename F, int... Is>
void for_each(T&& t, F f, seq<Is...>) {
    auto l __attribute__((unused)) = {(f(std::get<Is>(t)), 0)...};
}
}  // namespace detail

template <typename... Ts, typename F>
void for_each_in_tuple(std::tuple<Ts...>& t, F f) {
    detail::for_each(t, f, detail::gen_seq<sizeof...(Ts)>());
}

template <typename Subject>
struct apply_functor {
    apply_functor(Subject* subject) : m_subject(subject) {}
    template <typename T>
    void operator()(T&& t) {
        t.apply(m_subject);
    }
    Subject* m_subject;
};

template <typename... TProperties>
class KeyFrame {
public:
    template <typename TProperty, typename TValue>
    KeyFrame<TProperties...>& set(TValue value) {
        std::get<TProperty>(m_properties).value = value;
        return *this;
    }

    template <typename TSubject>
    void apply(TSubject* subject) {
        for_each_in_tuple(m_properties, apply_functor<TSubject>(subject));
    }

private:
    std::tuple<TProperties...> m_properties;
};

}  // namespace sfvg

#endif  // SFVG_KEY_FRAME_HPP
