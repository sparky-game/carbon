/*
**  $$===================$$
**  ||       Types       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef unsigned char u8;
typedef signed char i8;
CARBON_STATIC_ASSERT(sizeof(u8) == 1 && sizeof(i8) == 1, "Expected u8/i8 to be 1 byte");
typedef unsigned short u16;
typedef signed short i16;
CARBON_STATIC_ASSERT(sizeof(u16) == 2 && sizeof(i16) == 2, "Expected u16/i16 to be 2 bytes");
typedef unsigned int u32;
typedef signed int i32;
CARBON_STATIC_ASSERT(sizeof(u32) == 4 && sizeof(i32) == 4, "Expected u32/i32 to be 4 bytes");
typedef unsigned long long u64;
typedef signed long long i64;
CARBON_STATIC_ASSERT(sizeof(u64) == 8 && sizeof(i64) == 8, "Expected u64/i64 to be 8 bytes");
typedef float f32;
CARBON_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes");
typedef double f64;
CARBON_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes");
typedef long double flong;
CARBON_STATIC_ASSERT(sizeof(flong) >= sizeof(f64), "Expected flong to be at least 8 bytes long");

typedef u32 uint;
typedef size_t usz;
typedef ssize_t isz;
typedef uintptr_t uptr;

#ifdef __cplusplus
namespace cbn {
  template <typename... Ts>
  using Tuple = std::tuple<Ts...>;
  template <typename T>
  using Opt = std::optional<T>;
  template <typename T>
  using Scope = std::unique_ptr<T>;
  template <typename T>
  using Ref = std::shared_ptr<T>;
  template <typename T, typename... Ts>
  concept AllowedTypes = (std::same_as<T, Ts> or ...);
}
#endif

// Local Variables:
// mode: c++
// End:
