// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

#define carbon_defer__var CARBON_PASTE(carbon_defer__, __LINE__)

template <typename T>
struct carbon_defer__raii {
  carbon_defer__raii(carbon_defer__raii &&) = delete;
  carbon_defer__raii(const carbon_defer__raii &) = delete;
  carbon_defer__raii &operator=(carbon_defer__raii &&) = delete;
  carbon_defer__raii &operator=(const carbon_defer__raii &) = delete;
  inline ~carbon_defer__raii(void) { func(); }
  template <typename U>
  carbon_defer__raii(U &&f) noexcept : func{cbn::meta::Forward<U>(f)} {}
private:
  T func;
};

template <typename T>
carbon_defer__raii<T> carbon_defer__impl(T &&f) noexcept {
  return {cbn::meta::Forward<T>(f)};
}

struct carbon_defer__adder_t {
  template <typename T>
  carbon_defer__raii<cbn::meta::Decay_t<T>> operator+(T &&f) const noexcept {
    return carbon_defer__impl(cbn::meta::Forward<T>(f));
  }
};

[[maybe_unused]] inline constexpr carbon_defer__adder_t carbon_defer__adder {};

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
