#ifndef SFVG_ANIMATION_DETAIL_HPP
#define SFVG_ANIMATION_DETAIL_HPP

#ifdef __GNUC__
  #define ATTR_UNUSED __attribute__((unused))
#else
  #define ATTR_UNUSED
#endif

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
    auto l ATTR_UNUSED = {(f(std::get<Is>(t)), 0)...};
}

template <typename T, typename F, int... Is>
void for_each2(T&& t1, T&& t2, F f, seq<Is...>) {
    auto l ATTR_UNUSED = {
        (f(std::get<Is>(t1), std::get<Is>(t2)), 0)...};
}

template <typename T, typename F, int... Is>
void for_each3(T&& t1, T&& t2, T&& t3, F f, seq<Is...>) {
    auto l ATTR_UNUSED = {
        (f(std::get<Is>(t1), std::get<Is>(t2), std::get<Is>(t3)), 0)...};
}

template <typename... Ts, typename F>
void for_each_in_tuple(std::tuple<Ts...>& t, F f) {
    for_each(t, f, gen_seq<sizeof...(Ts)>());
}

template <typename... Ts, typename F>
void for_each_in_tuple2(std::tuple<Ts...>& t1, std::tuple<Ts...>& t2, F f) {
    for_each2(t1, t2, f, gen_seq<sizeof...(Ts)>());
}

template <typename... Ts, typename F>
void for_each_in_tuple3(std::tuple<Ts...>& t1,
                        std::tuple<Ts...>& t2,
                        std::tuple<Ts...>& t3,
                        F f) {
    for_each3(t1, t2, t3, f, gen_seq<sizeof...(Ts)>());
}

}  // namespace detail

}  // namespace sfvg

#endif  // SFVG_ANIMATION_DETAIL_HPP
