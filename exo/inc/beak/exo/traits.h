/**
 * @file        executor traits
 *
 * @author      Barath Kannan
 * @date        Mon Feb 26 23:34:57 2018
 * @ingroup     exo
 */

#ifndef BEAK_EXO_TRAITS_H
#define BEAK_EXO_TRAITS_H

#include <beak/exo/detail/detector.h>

namespace beak::exo::traits {

template <class T, class F>
using executor_post_t = decltype(std::declval<T&>().post(std::declval<F&&>()));

template <class T, class F>
using executor_dispatch_t = decltype(std::declval<T&>().dispatch(std::declval<F&&>()));

template <class T, class F>
using executor_execute_t = decltype(std::declval<T&>().execute(std::declval<F&&>()));

template <typename T, typename F>
constexpr bool has_executor_post = beak::exo::detail::is_detected_v<executor_post_t, T, F>;

template <typename T, typename F>
constexpr bool has_executor_dispatch = beak::exo::detail::is_detected_v<executor_dispatch_t, T, F>;

template <typename T, typename F>
constexpr bool has_executor_execute = beak::exo::detail::is_detected_v<executor_execute_t, T, F>;

template <typename T, typename F = void (*)()>
constexpr bool is_executor = has_executor_post<T, F> || has_executor_dispatch<T, F> || has_executor_execute<T, F>;

} //namespace beak::exo::traits

#endif // BEAK_EXO_TRAITS_H
