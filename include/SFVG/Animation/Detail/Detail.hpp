#ifndef SFVG_ANIMATION_DETAIL_HPP
#define SFVG_ANIMATION_DETAIL_HPP

#include <tuple>
#include <iostream>

namespace sfvg {

    namespace detail {

    // sequence generation
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

    template <typename T, typename F, int... Is>
    void for_each3(T&& t1, T&& t2, T&& t3, F f, seq<Is...>) {
        auto l __attribute__((unused)) = {(f(std::get<Is>(t1),std::get<Is>(t2), std::get<Is>(t3)), 0)...};
    }

    template <typename... Ts, typename F>
    void for_each_in_tuple(std::tuple<Ts...>& t, F f) {
        for_each(t, f, gen_seq<sizeof...(Ts)>());
    }

    template <typename... Ts, typename F>
    void for_each_in_tuple3(std::tuple<Ts...>& t1, std::tuple<Ts...>& t2, std::tuple<Ts...>& t3, F f) {
        for_each3(t1, t2, t3, f, gen_seq<sizeof...(Ts)>());
    }

    template <typename Subject>
    struct set_functor {
        set_functor(Subject* subject) : m_subject(subject) {}
        template <typename T>
        void operator()(T&& t) {
            t.set(m_subject);
        }
        Subject* m_subject;
    };

    struct tween_functor {
        tween_functor(float t) : m_t(t) {}
        template <typename T>
        void operator()(T&& a, T&& b, T&& out) {
            if ((int)b.mode == 0) // Absolute
                b.absValue = b.value;
            else if ((int)b.mode == 1) // Relative
                b.absValue = a.absValue + b.value;
            out.value = b.func(a.absValue, b.absValue, m_t);
        }
        float m_t;
    };

    }  // namespace detail

}


#endif // SFVG_ANIMATION_DETAIL_HPP
