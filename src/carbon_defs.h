/*
**  $$=========================$$
**  ||       Definitions       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_NAME "SPARKY Carbon"
#define CARBON_VERSION "v" CARBON_VERSION_RAW

#undef true
#define true 1
#undef false
#define false 0
#define CARBON_OK true
#define CARBON_KO false

#define CARBON_QUOTE(x) #x
#define CARBON_PASTE(x, y) x ## y
#define CARBON_EXPAND_AND_QUOTE(x) CARBON_QUOTE(x)
#define CARBON_EXPAND_AND_PASTE(x, y) CARBON_PASTE(x, y)
#define CARBON_MACRO_IS_EMPTY(x) ((0 - x - 1) == 1 && (x - 0) != -2)
#define CARBON_NOTUSED(x) (void)(x)
#define CARBON_UNUSED(x) CARBON_NOTUSED(x)
#define CARBON_ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[0]))
#define CARBON_ARRAY_SHIFT(xs, xs_sz) (CARBON_ASSERT((xs_sz) > 0 && "Array is empty"), --(xs_sz), *(xs)++)
#define CARBON_SHIFT_ARGS(argc, argv) CARBON_ARRAY_SHIFT(argv, argc)

#define CARBON_UNREACHABLE CARBON_ASSERT(false && "unreachable")
#define CARBON_NOTIMPLEMENTED CARBON_ASSERT(false && "not yet implemented")
#define CARBON_STATIC_NOTIMPLEMENTED CARBON_STATIC_ASSERT(false, "not yet implemented")

#ifdef __cplusplus
#define CARBON_TYPE_OF(x) decltype(x)
#define CARBON_TYPE_IS_SAME(T, U) std::is_same_v<T, U>
#else
#define CARBON_TYPE_OF(x) __typeof__(x)
#define CARBON_TYPE_IS_SAME(T, U) __builtin_types_compatible_p(T, U)
#endif

#ifdef __cplusplus
#define CARBON_TEMPLATE_CLASS(name) template <typename T> struct name
#else
#define CARBON_TEMPLATE_CLASS(name) struct name
#endif

#ifdef __cplusplus
#define CARBON_API extern "C"
#else
#define CARBON_API extern
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CARBON_INLINE __attribute__((always_inline)) static inline
#define CARBON_NOINLINE __attribute__((noinline))
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_INLINE __forceinline
#define CARBON_NOINLINE __declspec(noinline)
#else
#define CARBON_INLINE static inline
#define CARBON_NOINLINE
#endif

#if !defined(__cplusplus) && (defined(__GNUC__) || defined(__clang__))
#define CARBON_STATIC_ASSERT _Static_assert
#else
#define CARBON_STATIC_ASSERT static_assert
#endif

#if !defined(CARBON_VERSION_MAJOR) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_MAJOR) || !defined(CARBON_VERSION_MINOR) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_MINOR) || !defined(CARBON_VERSION_PATCH) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_PATCH) || !defined(CARBON_VERSION_EXTRA)
#error Version information not valid
#elif CARBON_VERSION_PATCH != 0
#define CARBON_VERSION_RAW                                              \
  CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)                         \
    "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)                   \
    "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_PATCH) CARBON_VERSION_EXTRA
#elif CARBON_VERSION_PATCH == 0
#define CARBON_VERSION_RAW                                              \
  CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)                         \
    "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR) CARBON_VERSION_EXTRA
#endif

#if defined(__amd64__) || defined(_M_AMD64)
#define CARBON_CPU_ARCH "amd64"
#elif defined(__aarch64__)
#define CARBON_CPU_ARCH "aarch64"
#elif defined(__wasm32__)
#define CARBON_CPU_ARCH "wasm32"
#else
#error CPU architecture is not supported
#endif

#if defined(__linux__)
#define CARBON_TARGET_OS "linux"
#elif defined(__APPLE__)
#define CARBON_TARGET_OS "macos"
#elif defined(__FreeBSD__)
#define CARBON_TARGET_OS "freebsd"
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_TARGET_OS "windows-msvc"
#elif defined(_WIN32) && defined(__MINGW64__)
#define CARBON_TARGET_OS "windows-mingw"
#elif defined(__wasm__)
#define CARBON_TARGET_OS "web"
#else
#error Target OS is not supported
#endif

#if defined(__clang__)
#define CARBON_C_COMPILER "clang"
#define CARBON_CXX_COMPILER "clang++"
#define CARBON_COMPILER_VERSION __VERSION__
#elif defined(__GNUC__)
#define CARBON_C_COMPILER "gcc"
#define CARBON_CXX_COMPILER "g++"
#define CARBON_COMPILER_VERSION __VERSION__
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_C_COMPILER "cl.exe"
#define CARBON_CXX_COMPILER "cl.exe"
#define CARBON_COMPILER_VERSION _MSC_FULL_VER
#elif defined(_WIN32) && defined(__MINGW64__)
#define CARBON_C_COMPILER "x86_64-w64-mingw32-gcc"
#define CARBON_CXX_COMPILER "x86_64-w64-mingw32-g++"
#define CARBON_COMPILER_VERSION __VERSION__
#else
#warning Unrecognized compiler, using defaults.
#define CARBON_C_COMPILER "cc"
#define CARBON_CXX_COMPILER "c++"
#define CARBON_COMPILER_VERSION __VERSION__
#endif
#ifdef __cplusplus
#define CARBON_COMPILER CARBON_CXX_COMPILER
#else
#define CARBON_COMPILER CARBON_C_COMPILER
#endif

#ifdef __cplusplus
namespace std {
  template <typename T>
  struct always_false : std::false_type {};
}
#endif

// Local Variables:
// mode: c++
// End:
