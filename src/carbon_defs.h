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

#if !defined(__cplusplus) && __STDC_VERSION__ != 201112L
#error C11 standard is needed
#endif
#if defined(__cplusplus) && __cplusplus != 202002L
#error C++20 standard is needed
#endif

#if !defined(__cplusplus)
#undef true
#define true 1
#undef false
#define false 0
#endif

#if !defined(__cplusplus) && (defined(__GNUC__) || defined(__clang__))
#define static_assert _Static_assert
#endif

#ifdef __cplusplus
#define typeof(x) decltype(x)
#define restrict __restrict
#else
#define typeof(x) __typeof__(x)
#endif

#define CARBON_OK true
#define CARBON_KO false

#define CARBON_QUOTE(x) CARBON_QUOTE__2_ELECTRIC_BOOGALOO(x)
#define CARBON_QUOTE__2_ELECTRIC_BOOGALOO(x) #x
#define CARBON_PASTE(x, y) CARBON_PASTE__2_ELECTRIC_BOOGALOO(x, y)
#define CARBON_PASTE__2_ELECTRIC_BOOGALOO(x, y) x ## y
#define CARBON_MACRO_IS_EMPTY(x) ((0 - x - 1) == 1 && (x - 0) != -2)
#define CARBON_NOTUSED(x) (void)(x)
#define CARBON_UNUSED(x) CARBON_NOTUSED(x)
#define CARBON_ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[0]))
#define CARBON_ARRAY_SHIFT(xs, xs_sz) (CBN_ASSERT((xs_sz) > 0 && "Array is empty"), --(xs_sz), *(xs)++)
#define CARBON_SHIFT_ARGS(argc, argv) CARBON_ARRAY_SHIFT(argv, argc)

/**
 * @brief Swaps the values of a pair of variables between each other.
 * @param x The first variable.
 * @param y The second variable.
 */
#define CARBON_SWAP(x, y) { x ^= y; y ^= x; x ^= y; }

#if defined(__has_builtin) && !defined(CARBON_NO_BUILTIN)
#define CARBON_HAS_BUILTIN __has_builtin
#else
#define CARBON_HAS_BUILTIN(...) false
#endif

#define CARBON_UNREACHABLE CBN_ASSERT(false && "unreachable")
#define CARBON_STATIC_UNREACHABLE static_assert(false, "unreachable")
#define CARBON_NOTIMPLEMENTED CBN_ASSERT(false && "not yet implemented")
#define CARBON_STATIC_NOTIMPLEMENTED static_assert(false, "not yet implemented")

#ifdef __cplusplus
#define CARBON_TYPE_IS_SAME(T, U) cbn::meta::Same_v<T, U>
#else
#define CARBON_TYPE_IS_SAME(T, U) __builtin_types_compatible_p(T, U)
#endif

/**
 * @brief ...
 */
#ifdef __cplusplus
#define CBNDEF extern "C"
#else
#define CBNDEF extern
#endif

#ifdef __cplusplus
#define CBNDEF_T(ns, alias, type) namespace ns {using alias = type;}
#define CBNDEF_FN(ns, alias, fn) namespace ns {constexpr auto alias = fn;}
#else
#define CBNDEF_T(...)
#define CBNDEF_FN(...)
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

#if !defined(CARBON_VERSION_MAJOR) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_MAJOR) || !defined(CARBON_VERSION_MINOR) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_MINOR) || !defined(CARBON_VERSION_PATCH) || CARBON_MACRO_IS_EMPTY(CARBON_VERSION_PATCH) || !defined(CARBON_VERSION_EXTRA)
#error Version information not valid
#elif CARBON_VERSION_PATCH != 0
#define CARBON_VERSION_RAW                                      \
  CARBON_QUOTE(CARBON_VERSION_MAJOR)                            \
  "." CARBON_QUOTE(CARBON_VERSION_MINOR)                        \
  "." CARBON_QUOTE(CARBON_VERSION_PATCH) CARBON_VERSION_EXTRA
#elif CARBON_VERSION_PATCH == 0
#define CARBON_VERSION_RAW                                      \
  CARBON_QUOTE(CARBON_VERSION_MAJOR)                            \
  "." CARBON_QUOTE(CARBON_VERSION_MINOR) CARBON_VERSION_EXTRA
#endif

#if defined(__amd64__) || defined(_M_AMD64)
#define CARBON_CPU_ARCH "amd64"
#elif defined(__aarch64__)
#define CARBON_CPU_ARCH "aarch64"
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
#else
#error Target OS is not supported
#endif

#define CARBON_COMPILER_PRAGMA(x) _Pragma(#x)
#if defined(_WIN32) && defined(__MINGW64__)
#define CARBON_C_COMPILER "gcc.exe"
#define CARBON_CXX_COMPILER "g++.exe"
#define CARBON_COMPILER_VERSION __VERSION__
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_C_COMPILER "cl.exe"
#define CARBON_CXX_COMPILER "cl.exe"
#define CARBON_COMPILER_VERSION _MSC_FULL_VER
#elif defined(__clang__)
#define CARBON_C_COMPILER "clang"
#define CARBON_CXX_COMPILER "clang++"
#define CARBON_COMPILER_VERSION __VERSION__
#define CARBON_COMPILER_DIAG_BEGIN CARBON_COMPILER_PRAGMA(clang diagnostic push)
#define CARBON_COMPILER_DIAG_IGNORE(w) CARBON_COMPILER_PRAGMA(clang diagnostic ignored w)
#define CARBON_COMPILER_DIAG_END CARBON_COMPILER_PRAGMA(clang diagnostic pop)
#elif defined(__GNUC__)
#define CARBON_C_COMPILER "gcc"
#define CARBON_CXX_COMPILER "g++"
#define CARBON_COMPILER_VERSION __VERSION__
#define CARBON_COMPILER_DIAG_BEGIN CARBON_COMPILER_PRAGMA(GCC diagnostic push)
#define CARBON_COMPILER_DIAG_IGNORE(w) CARBON_COMPILER_PRAGMA(GCC diagnostic ignored w)
#define CARBON_COMPILER_DIAG_END CARBON_COMPILER_PRAGMA(GCC diagnostic pop)
#else
#warning Unrecognized compiler, using defaults
#define CARBON_C_COMPILER "cc"
#define CARBON_CXX_COMPILER "c++"
#define CARBON_COMPILER_VERSION __VERSION__
#endif
#ifdef __cplusplus
#define CARBON_COMPILER CARBON_CXX_COMPILER
#else
#define CARBON_COMPILER CARBON_C_COMPILER
#endif

// Local Variables:
// mode: c++
// End:
