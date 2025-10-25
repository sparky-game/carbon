/*
**  $$===================$$
**  ||       Defer       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

/**
 * @brief ...
 */
#define defer [[maybe_unused]] const auto carbon_defer__var = carbon_defer__adder + [&]()
#define carbon_defer__var CARBON_PASTE(carbon_defer__, __LINE__)
template <typename T>
struct carbon_defer__raii {
  carbon_defer__raii(carbon_defer__raii &&) = delete;
  carbon_defer__raii(const carbon_defer__raii &) = delete;
  carbon_defer__raii &operator=(carbon_defer__raii &&) = delete;
  carbon_defer__raii &operator=(const carbon_defer__raii &) = delete;
  ~carbon_defer__raii(void) { func(); }
  template <typename U>
  carbon_defer__raii(U &&f) : func{std::forward<U>(f)} {}
private:
  T func;
};
template <typename T>
carbon_defer__raii<T> carbon_defer__impl(T &&f) { return {std::forward<T>(f)}; }
struct carbon_defer__adder_t {
  template <typename T>
  carbon_defer__raii<std::decay_t<T>> operator+(T &&f) const noexcept {
    return carbon_defer__impl(std::forward<T>(f));
  }
};
[[maybe_unused]] inline constexpr carbon_defer__adder_t carbon_defer__adder {};

#else

// TODO: implement the `defer` keyword in C

#endif

// Local Variables:
// mode: c++
// End:
