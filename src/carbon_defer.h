/*
**  $$===================$$
**  ||       Defer       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus
template <typename T>
struct carbon_defer__raii {
  carbon_defer__raii(carbon_defer__raii &&) = delete;
  carbon_defer__raii(const carbon_defer__raii &) = delete;
  carbon_defer__raii &operator=(carbon_defer__raii &&) = delete;
  carbon_defer__raii &operator=(const carbon_defer__raii &) = delete;

  ~carbon_defer__raii(void) { m_Cleanup(); }

  template <typename U>
  carbon_defer__raii(U &&f) :  m_Cleanup{std::forward<U>(f)} {}

private:
  T m_Cleanup;
};

template <typename T>
carbon_defer__raii<T> carbon_defer__impl(T &&f) { return {std::forward<T>(f)}; }

/**
 * @brief ...
 */
#define defer__var(x) CARBON_EXPAND_AND_PASTE(x, __LINE__)
#define defer(L) [[maybe_unused]] const auto& defer__var(carbon_defer__obj) = carbon_defer__impl([&]() L)
#else
// TODO: implement the `defer` keyword in C
#endif

// Local Variables:
// mode: c++
// End:
