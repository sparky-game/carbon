/*
**  $$=================================================================================$$
**  ||                                                                                 ||
**  ||               ------                                                            ||
**  ||            ------==+                                                            ||
**  ||         ------=+++++                     _____            _                     ||
**  ||      ------=+++++                       /  __ \          | |                    ||
**  ||    =----=++++=           ------         | /  \/ __ _ _ __| |__   ___  _ __      ||
**  ||    ***++++            ------=++         | |    / _` | '__| '_ \ / _ \| '_ \     ||
**  ||    ***+++          ------=+++++         | \__/\ (_| | |  | |_) | (_) | | | |    ||
**  ||    ***+++      ------==++++**++          \____/\__,_|_|  |_.__/ \___/|_| |_|    ||
**  ||    ***+++     ++--==+++++****++                                                 ||
**  ||    ***+++     ***+++++   ****++               The ultimate high-level           ||
**  ||    ***+++      #*++      ****++                  library for C/C++              ||
**  ||    ***+++                ****++                written in C11/C++14             ||
**  ||    **+----             :---=+++                                                 ||
**  ||    *****=-----      ------=++++            Licensed under AGPL-3.0-only         ||
**  ||      #*****=-----------=+++++                                                   ||
**  ||         ******=-----=+++++                        Wasym A. Alonso               ||
**  ||            ******=+++++                                                         ||
**  ||               ***+++                          wasymatieh01@gmail.com            ||
**  ||                                                                                 ||
**  ||                                        https://github.com/sparky-game/carbon    ||
**  ||                                                                                 ||
**  ||                                                                                 ||
**  $$=================================================================================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_VERSION_MAJOR 0
#define CARBON_VERSION_MINOR 13
#define CARBON_VERSION_PATCH 0
#define CARBON_VERSION_EXTRA "-alpha"

/*
**  Available compile-time options:
**
**    - CARBON_TESTING_ENTRY ------------> Redefines the `main` function, thus making it behave
**                                         as a test suite entrypoint. It's only relevant to
**                                         define it in the translation unit where we're defining
**                                         the `main` function.
**
**    - CARBON_IMPLEMENTATION -----------> Include function definitions in the translation unit
**                                         that this macro has been defined. It's important that
**                                         this macro is ONLY defined ONCE in the entire codebase
**                                         of your program, to avoid multiple symbol definitions
**                                         linker errors.
**
**    - CARBON_IMPLEMENTATION_WINDOWING -> Appart from what `CARBON_IMPLEMENTATION` includes, this
**                                         macro includes windowing-related function definitions.
**                                         As well, it's important that this macro is ONLY defined
**                                         ONCE in the entire codebase of your program, to avoid
**                                         multiple symbol definitions linker errors.
*/

/*
**  $$==========================$$
**  ||       Dependencies       ||
**  $$==========================$$
*/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
// Common
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
// Windows
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <direct.h>
#include <windows.h>
// POSIX
#else
#include <glob.h>
#include <poll.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
// macOS
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

/*
**  $$=========================$$
**  ||       Definitions       ||
**  $$=========================$$
*/
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
#define CARBON_ARRAY_SHIFT(xs, xs_sz) (CARBON_ASSERT((xs_sz) > 0), --(xs_sz), *(xs)++)
#define CARBON_TYPE_OF(x) __typeof__(x)
#define CARBON_SHIFT_ARGS(argc, argv) CARBON_ARRAY_SHIFT(argv, argc)

#define CARBON_UNREACHABLE CARBON_ASSERT(false && "unreachable")
#define CARBON_NOTIMPLEMENTED CARBON_ASSERT(false && "not yet implemented")
#define CARBON_STATIC_NOTIMPLEMENTED CARBON_STATIC_ASSERT(false, "not yet implemented")

#ifdef __cplusplus
#include <type_traits>
#define CARBON_TYPE_IS_SAME(T, U) std::is_same<T, U>::value
#else
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
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)                     \
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
#else
#error CPU architecture is not supported
#endif

#if defined(__linux__)
#define CARBON_TARGET_OS "linux"
#elif defined(__APPLE__)
#define CARBON_TARGET_OS "macos"
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_TARGET_OS "windows-msvc"
#elif defined(_WIN32) && defined(__MINGW64__)
#define CARBON_TARGET_OS "windows-mingw"
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

/*
**  $$===================$$
**  ||       Types       ||
**  $$===================$$
*/
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

typedef u32 uint;
typedef size_t usz;
typedef ssize_t isz;
typedef uintptr_t uptr;

/*
**  $$=====================$$
**  ||       Version       ||
**  $$=====================$$
*/

/**
 * @brief Gets the library API version at runtime.
 * @param major The major component of the version (output argument pointer).
 * @param minor The minor component of the version (output argument pointer).
 * @param patch The patch component of the version (output argument pointer).
 * @return The full version string formatted as `vX.Y.Z[-EXTRA]`.
 */
CARBON_API char *carbon_version(usz *major, usz *minor, usz *patch);

/*
**  $$================================$$
**  ||       Testing Entrypoint       ||
**  $$================================$$
*/
#ifdef CARBON_TESTING_ENTRY
#define main(...)                                       \
  main(int argc, char **argv) {                         \
    carbon_test_manager_argparse(argc, argv);           \
    carbon_test_manager_rebuild(__FILE__, argv);        \
    return carbon_main();                               \
  };                                                    \
  int carbon_main(__VA_ARGS__)
#endif  // CARBON_TESTING_ENTRY

CARBON_API int carbon_main(void);

/*
**  $$====================$$
**  ||       Assert       ||
**  $$====================$$
*/
#define CARBON_ASSERT(x) ((void) ((x) || (carbon_assert_abort(#x, __FILE__, __LINE__, __func__), 0)))

CARBON_API void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func);

/*
**  $$====================$$
**  ||       Memory       ||
**  $$====================$$
*/
#define CARBON_MALLOC(sz)        malloc(sz)
#define CARBON_REALLOC(p, newsz) realloc(p, newsz)
#define CARBON_CALLOC(n, sz)     calloc(n, sz)
#define CARBON_FREE(p)           free(p)

/**
 * @brief Copies a set of bytes from a memory area to another one.
 * @param dst The destination memory area address.
 * @param src The source memory area address.
 * @param n The amount of bytes to copy from `src`.
 * @return The original value of `dst`.
 */
CARBON_API void *carbon_memory_copy(void *dst, const void *src, usz n);

/*
**  $$=======================$$
**  ||       Coroutine       ||
**  $$=======================$$
*/
typedef struct {
  void *rsp;   // Stack pointer
  void *rsbp;  // Stack base pointer
} CBN_Coroutine_CTX;

typedef enum {
  CBN_COROUTINE_SLEEP_MODE_NONE,
  CBN_COROUTINE_SLEEP_MODE_READ,
  CBN_COROUTINE_SLEEP_MODE_WRITE
} CBN_Coroutine_SleepMode;

CARBON_API usz carbon_coroutine_id(void);
CARBON_API usz carbon_coroutine_alive(void);
CARBON_API void carbon_coroutine_go(void (*f)(void *), void *arg);
CARBON_API void carbon_coroutine_yield(void);
CARBON_API void carbon_coroutine_sleep_read(i32 fd);
CARBON_API void carbon_coroutine_sleep_write(i32 fd);
CARBON_API void carbon_coroutine_wakeup(usz id);

/*
**  $$==================$$
**  ||       Math       ||
**  $$==================$$
*/
#define CARBON_E 2.71828182845904523536
#define CARBON_E_0_5 1.64872127070012814684
#define CARBON_E_0_25 1.28402541668774148407
#define CARBON_E_0_125 1.13314845306682631682
#define CARBON_PI 3.14159265358979323846
#define CARBON_PI_2 (0.5 * CARBON_PI)
#define CARBON_2PI (2 * CARBON_PI)
#define CARBON_PI_4 (0.25 * CARBON_PI)
#define CARBON_4PI (4 * CARBON_PI)
#define CARBON_1_PI (1 / CARBON_PI)
#define CARBON_1_2PI (1 / CARBON_2PI)
#define CARBON_SQRT2 1.41421356237309504880
#define CARBON_1_SQRT2 (1 / CARBON_SQRT2)
#define CARBON_SQRT3 1.73205080756887729352
#define CARBON_1_SQRT3 (1 / CARBON_SQRT3)
#define CARBON_LOG2_E (1.4426950408889634074)
#define CARBON_LOG2_10 (3.32192809488736234789)
#define CARBON_I32_MAX 0x7fffffff
#define CARBON_RAND_MAX CARBON_I32_MAX
#define CARBON_PCG_RAND_MAGIC 6364136223846793005ULL
#define CARBON_F32_EPS 1.1920928955078125e-07
#define CARBON_MIN(x, y) (x < y ? x : y)
#define CARBON_MIN3(x, y, z) CARBON_MIN(x, CARBON_MIN(y, z))
#define CARBON_MAX(x, y) (x > y ? x : y)
#define CARBON_MAX3(x, y, z) CARBON_MAX(x, CARBON_MAX(y, z))
#define CARBON_CLAMP(x, min, max) ((x <= min) ? min : (x >= max) ? max : x)
#define CARBON_SIGN(x) (!x ? 0 : x < 0 ? -1 : 1)
#define CARBON_STEP(edge, x) (x < edge ? 0 : 1)
#define CARBON_SWAP(T, x, y) do { T z = x; x = y; y = z; } while (0)
#define CARBON_LERP(a, b, t) (a + (b - a) * t)
// TODO: somehow perform bounds checking `CARBON_ASSERT(0 <= (i) && (i) < (m).rows && 0 <= (j) && (j) < (m).cols && "Matrix index out of bounds")`
#define CARBON_MAT_AT(m, i, j) (m).items[(i) * (m).cols + (j)]
// TODO: somehow perform bounds checking `CARBON_ASSERT(0 <= (i) && (i) < (r).cols && "Row index out of bounds")`
#define CARBON_ROW_AT(r, i) (r).items[(i)]
#define CARBON_MAT_PRINT(m) carbon_math_mat_print(m, #m)
#define CARBON_ROW_PRINT(r) carbon_math_row_print(r, #r)

/**
 * @brief Defines an inline 2D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 */
#define CARBON_VEC2(x, y) (CBN_Vec2){{(f32)(x), (f32)(y)}}

/**
 * @brief Defines an inline 2D vector.
 * @param x The value to assign to both the X and Y fields.
 */
#define CARBON_VEC2_1(x) CARBON_VEC2(x, x)

/**
 * @brief Defines an inline 2D vector (from polar coordinate system).
 * @param r The radial coordinate.
 * @param phi The angular coordinate.
 */
#define CARBON_VEC2_P(r, phi) CARBON_VEC2(r * cos(phi), r * sin(phi))

/**
 * @brief Defines an inline 2D vector whose 2 elements are equal to zero.
 */
#define CARBON_VEC2_ZERO CARBON_VEC2(0, 0)

/**
 * @brief Defines an inline 2D vector which represents -X direction.
 */
#define CARBON_VEC2_LEFT CARBON_VEC2(-1, 0)

/**
 * @brief Defines an inline 2D vector which represents +X direction.
 */
#define CARBON_VEC2_RIGHT CARBON_VEC2(1, 0)

/**
 * @brief Defines an inline 2D vector which represents -Y direction.
 */
#define CARBON_VEC2_DOWN CARBON_VEC2(0, -1)

/**
 * @brief Defines an inline 2D vector which represents +Y direction.
 */
#define CARBON_VEC2_UP CARBON_VEC2(0, 1)

/**
 * @brief Defines an inline 2D vector whose 2 elements are equal to one.
 */
#define CARBON_VEC2_ONE CARBON_VEC2(1, 1)

typedef union CBN_Vec3 CBN_Vec3;  // Forward declaration

/**
 * @brief Represents a 2D vector with two 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec2 {
  f32 items[2];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
  };
#ifdef __cplusplus
  /**
   * @brief carbon_math_vec2_clamp
   * @param min The minimum value (as 2D vector).
   * @param max The maximum value (as 2D vector).
   */
  void Clamp(const CBN_Vec2 &min, const CBN_Vec2 &max);
  /**
   * @brief carbon_math_vec2_floor
   * @return The rounded down 2D vector.
   */
  CBN_Vec2 Floor(void) const;
  /**
   * @brief carbon_math_vec2_len
   * @return The 2D vector's length.
   */
  f32 Length(void) const;
  /**
   * @brief carbon_math_vec2_len_squared
   * @return The 2D vector's length squared.
   */
  f32 LengthSquared(void) const;
  /**
   * @brief carbon_math_vec2_norm
   * @return The normalized 2D vector.
   */
  CBN_Vec2 Normalize(void) const;
  /**
   * @brief carbon_math_vec2_lerp
   * @param v The other 2D vector.
   * @param t A value [0..1] indicating the weight of `v`.
   * @return The interpolated 2D vector.
   */
  CBN_Vec2 Lerp(const CBN_Vec2 &v, f32 t) const;
  /**
   * @brief carbon_math_vec2_rotate
   * @param angle The rotation to apply (in degrees).
   * @return The rotated 2D vector.
   */
  CBN_Vec2 Rotate(f32 angle) const;
  /**
   * @brief carbon_math_vec2_to_cstr
   * @return The serialized 2D vector as `(X, Y)`.
   */
  const char *ToString(void) const;
  // Overloaded Operators
  CBN_Vec2 operator+(const CBN_Vec2 &v) const;
  CBN_Vec2 operator+(const f32 s) const;
  friend CBN_Vec2 operator+(const f32 s, const CBN_Vec2 &v);
  void operator+=(const CBN_Vec2 &v);
  CBN_Vec2 operator-(void) const;
  CBN_Vec2 operator-(const CBN_Vec2 &v) const;
  CBN_Vec2 operator-(const f32 s) const;
  friend CBN_Vec2 operator-(const f32 s, const CBN_Vec2 &v);
  void operator-=(const CBN_Vec2 &v);
  f32 operator*(const CBN_Vec2 &v) const;
  CBN_Vec2 operator*(const f32 s) const;
  friend CBN_Vec2 operator*(const f32 s, const CBN_Vec2 &v);
  void operator*=(const f32 s);
  CBN_Vec2 operator/(const f32 s) const;
  void operator/=(const f32 s);
  // Swizzling Operations
  constexpr auto xx(void)  const;
  constexpr auto xy(void)  const;
  constexpr auto xz(void)  const;
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto yz(void)  const;
  constexpr auto zx(void)  const;
  constexpr auto zy(void)  const;
  constexpr auto zz(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xxz(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto xyz(void) const;
  constexpr auto xzx(void) const;
  constexpr auto xzy(void) const;
  constexpr auto xzz(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yxz(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
  constexpr auto yyz(void) const;
  constexpr auto yzx(void) const;
  constexpr auto yzy(void) const;
  constexpr auto yzz(void) const;
  constexpr auto zxx(void) const;
  constexpr auto zxy(void) const;
  constexpr auto zxz(void) const;
  constexpr auto zyx(void) const;
  constexpr auto zyy(void) const;
  constexpr auto zyz(void) const;
  constexpr auto zzx(void) const;
  constexpr auto zzy(void) const;
  constexpr auto zzz(void) const;
#endif
} CBN_Vec2;

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3(x, y, z) (CBN_Vec3){{(f32)(x), (f32)(y), (f32)(z)}}

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X, Y and Z fields.
 */
#define CARBON_VEC3_1(x) CARBON_VEC3(x, x, x)

/**
 * @brief Defines an inline 3D vector.
 * @param v The 2D vector to assign to the X and Y fields.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3_V(v, z) CARBON_VEC3((v).x, (v).y, z)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to zero.
 */
#define CARBON_VEC3_ZERO CARBON_VEC3(0, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -X direction.
 */
#define CARBON_VEC3_LEFT CARBON_VEC3(-1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents +X direction.
 */
#define CARBON_VEC3_RIGHT CARBON_VEC3(1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -Y direction.
 */
#define CARBON_VEC3_DOWN CARBON_VEC3(0, -1, 0)

/**
 * @brief Defines an inline 3D vector which represents +Y direction.
 */
#define CARBON_VEC3_UP CARBON_VEC3(0, 1, 0)

/**
 * @brief Defines an inline 3D vector which represents -Z direction.
 */
#define CARBON_VEC3_BACK CARBON_VEC3(0, 0, -1)

/**
 * @brief Defines an inline 3D vector which represents +Z direction.
 */
#define CARBON_VEC3_FORWARD CARBON_VEC3(0, 0, 1)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to one.
 */
#define CARBON_VEC3_ONE CARBON_VEC3(1, 1, 1)

/**
 * @brief Represents a 3D vector with three 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec3 {
  f32 items[3];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
    union { f32 z, b, p, w; };
  };
#ifdef __cplusplus
  /**
   * @brief carbon_math_vec3_to_cstr
   * @return The serialized 3D vector as `(X, Y, Z)`.
   */
  const char *ToString(void) const;
  // Overloaded Operators
  CBN_Vec3 operator+(const CBN_Vec3 &v) const;
  CBN_Vec3 operator-(const CBN_Vec3 &v) const;
  f32 operator*(const CBN_Vec3 &v) const;
  // Swizzling Operations
  constexpr auto xx(void)  const;
  constexpr auto xy(void)  const;
  constexpr auto xz(void)  const;
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto yz(void)  const;
  constexpr auto zx(void)  const;
  constexpr auto zy(void)  const;
  constexpr auto zz(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xxz(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto xyz(void) const;
  constexpr auto xzx(void) const;
  constexpr auto xzy(void) const;
  constexpr auto xzz(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yxz(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
  constexpr auto yyz(void) const;
  constexpr auto yzx(void) const;
  constexpr auto yzy(void) const;
  constexpr auto yzz(void) const;
  constexpr auto zxx(void) const;
  constexpr auto zxy(void) const;
  constexpr auto zxz(void) const;
  constexpr auto zyx(void) const;
  constexpr auto zyy(void) const;
  constexpr auto zyz(void) const;
  constexpr auto zzx(void) const;
  constexpr auto zzy(void) const;
  constexpr auto zzz(void) const;
#endif
} CBN_Vec3;

// Vector Swizzling Operations
#define CARBON_VEC_xx(v)  CARBON_VEC2((v).x, (v).x)
#define CARBON_VEC_xy(v)  CARBON_VEC2((v).x, (v).y)
#define CARBON_VEC_xz(v)  CARBON_VEC2((v).x, (v).z)
#define CARBON_VEC_yx(v)  CARBON_VEC2((v).y, (v).x)
#define CARBON_VEC_yy(v)  CARBON_VEC2((v).y, (v).y)
#define CARBON_VEC_yz(v)  CARBON_VEC2((v).y, (v).z)
#define CARBON_VEC_zx(v)  CARBON_VEC2((v).z, (v).x)
#define CARBON_VEC_zy(v)  CARBON_VEC2((v).z, (v).y)
#define CARBON_VEC_zz(v)  CARBON_VEC2((v).z, (v).z)
#define CARBON_VEC_xxx(v) CARBON_VEC3((v).x, (v).x, (v).x)
#define CARBON_VEC_xxy(v) CARBON_VEC3((v).x, (v).x, (v).y)
#define CARBON_VEC_xxz(v) CARBON_VEC3((v).x, (v).x, (v).z)
#define CARBON_VEC_xyx(v) CARBON_VEC3((v).x, (v).y, (v).x)
#define CARBON_VEC_xyy(v) CARBON_VEC3((v).x, (v).y, (v).y)
#define CARBON_VEC_xyz(v) CARBON_VEC3((v).x, (v).y, (v).z)
#define CARBON_VEC_xzx(v) CARBON_VEC3((v).x, (v).z, (v).x)
#define CARBON_VEC_xzy(v) CARBON_VEC3((v).x, (v).z, (v).y)
#define CARBON_VEC_xzz(v) CARBON_VEC3((v).x, (v).z, (v).z)
#define CARBON_VEC_yxx(v) CARBON_VEC3((v).y, (v).x, (v).x)
#define CARBON_VEC_yxy(v) CARBON_VEC3((v).y, (v).x, (v).y)
#define CARBON_VEC_yxz(v) CARBON_VEC3((v).y, (v).x, (v).z)
#define CARBON_VEC_yyx(v) CARBON_VEC3((v).y, (v).y, (v).x)
#define CARBON_VEC_yyy(v) CARBON_VEC3((v).y, (v).y, (v).y)
#define CARBON_VEC_yyz(v) CARBON_VEC3((v).y, (v).y, (v).z)
#define CARBON_VEC_yzx(v) CARBON_VEC3((v).y, (v).z, (v).x)
#define CARBON_VEC_yzy(v) CARBON_VEC3((v).y, (v).z, (v).y)
#define CARBON_VEC_yzz(v) CARBON_VEC3((v).y, (v).z, (v).z)
#define CARBON_VEC_zxx(v) CARBON_VEC3((v).z, (v).x, (v).x)
#define CARBON_VEC_zxy(v) CARBON_VEC3((v).z, (v).x, (v).y)
#define CARBON_VEC_zxz(v) CARBON_VEC3((v).z, (v).x, (v).z)
#define CARBON_VEC_zyx(v) CARBON_VEC3((v).z, (v).y, (v).x)
#define CARBON_VEC_zyy(v) CARBON_VEC3((v).z, (v).y, (v).y)
#define CARBON_VEC_zyz(v) CARBON_VEC3((v).z, (v).y, (v).z)
#define CARBON_VEC_zzx(v) CARBON_VEC3((v).z, (v).z, (v).x)
#define CARBON_VEC_zzy(v) CARBON_VEC3((v).z, (v).z, (v).y)
#define CARBON_VEC_zzz(v) CARBON_VEC3((v).z, (v).z, (v).z)

/**
 * @brief Defines an inline rectangle.
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT(x, y, w, h) (CBN_Rect){(f32)(x), (f32)(y), (f32)(w), (f32)(h)}

/**
 * @brief Defines an inline rectangle.
 * @param v The 2D vector to assign to the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT_V(v, w, h) CARBON_RECT(v.x, v.y, w, h)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE(x, y, sz) CARBON_RECT(x, y, sz, sz)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param v The 2D vector to assign to the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE_V(v, sz) CARBON_RECT_SQUARE(v.x, v.y, sz)

/**
 * @brief Represents the position and size of a rectangle with four 32-bit floating-point (f32) values.
 */
typedef struct CBN_Rect {
  f32 x, y;
  f32 w, h;
#ifdef __cplusplus
  /**
   * @brief carbon_math_rect_contains_point
   * @param p The point to check.
   * @return A boolean value representing whether the point is contained within this rectangle or not.
   */
  u8 Contains(const CBN_Vec2 &p) const;
  /**
   * @brief carbon_math_rect_detect_collision
   * @param r The other rectangle to check with.
   * @return A boolean value representing whether they overlap (also touching).
   */
  u8 Overlaps(const CBN_Rect &r) const;
#endif
} CBN_Rect;

typedef struct {
  f32 *items;
  usz rows;
  usz cols;
} CBN_Matrix;

typedef struct {
  f32 *items;
  usz cols;
} CBN_Row;

CARBON_API void carbon_math_srand(u64 seed);
CARBON_API i32 carbon_math_rand(void);
CARBON_API i32 carbon_math_rand_between(i32 min, i32 max);
CARBON_API f32 carbon_math_randf(void);
CARBON_API void carbon_math_mt19937_64_srand(u64 seed);
CARBON_API u64 carbon_math_mt19937_64_rand(void);
CARBON_API f32 carbon_math_abs(f32 x);
CARBON_API f32 carbon_math_round(f32 x);
CARBON_API f32 carbon_math_floor(f32 x);
CARBON_API f32 carbon_math_ceil(f32 x);
CARBON_API f32 carbon_math_sqrt(f32 x);
CARBON_API f32 carbon_math_fmod(f32 x, f32 y);
CARBON_API f32 carbon_math_pow(f32 x, f32 y);
CARBON_API f32 carbon_math_log2(f32 x);
CARBON_API f32 carbon_math_exp2(f32 x);
CARBON_API f32 carbon_math_ldexp2(f32 x, i32 exp);
CARBON_API f32 carbon_math_log(f32 x);
CARBON_API f32 carbon_math_exp(f32 x);
CARBON_API f32 carbon_math_ldexp(f32 x, i32 exp);
CARBON_API f32 carbon_math_log10(f32 x);
CARBON_API f32 carbon_math_exp10(f32 x);
CARBON_API f32 carbon_math_ldexp10(f32 x, i32 exp);
CARBON_API f32 carbon_math_frexp(f32 x, i32 *exp);
CARBON_API f32 carbon_math_sigmoid(f32 x);
CARBON_API f32 carbon_math_tanh(f32 x);
CARBON_API f32 carbon_math_smoothstep(f32 a, f32 b, f32 t);
CARBON_API i8 carbon_math_cmp(f32 x, f32 y);

/**
 * @brief Concatenates two unsigned integers together (e.g. `10` || `69` :: `1069`).
 * @param x The first unsigned integer.
 * @param y The second unsigned integer.
 * @return The unsigned integer representing their concatenation.
 */
CARBON_API u64 carbon_math_concat(u64 x, u64 y);

CARBON_API f32 carbon_math_sin(f32 x);
CARBON_API f32 carbon_math_cos(f32 x);
CARBON_API f32 carbon_math_asin(f32 x);
CARBON_API f32 carbon_math_atan(f32 x);
CARBON_API f32 carbon_math_atan2(f32 y, f32 x);

CARBON_API CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Restricts a 2D vector between a minimum and a maximum value.
 * @param v The 2D vector.
 * @param min The minimum value (as 2D vector).
 * @param max The maximum value (as 2D vector).
 * @return The restricted 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max);

/**
 * @brief Rounds down the components of the 2D vector.
 * @param v The 2D vector.
 * @return The rounded down 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector squared.
 * @param v The 2D vector.
 * @return The 2D vector's length squared.
 */
CARBON_API f32 carbon_math_vec2_len_squared(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector.
 * @param v The 2D vector.
 * @return The 2D vector's length.
 */
CARBON_API f32 carbon_math_vec2_len(CBN_Vec2 v);

/**
 * @brief Returns a 2D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 2D vector.
 * @return The normalized 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v);

/**
 * @brief Performs a linear interpolation between two 2D vectors based on the given weighting.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t);

/**
 * @brief Scales the 2D vector by the specified scalar value.
 * @param v The 2D vector.
 * @param s The scalar value.
 * @return The scaled 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s);

/**
 * @brief Rotates the 2D vector by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @return The rotated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle);

/**
 * @brief Rotates the 2D vector around a given pivot point by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @param pivot The pivot point around which to rotate the 2D vector.
 * @return The rotated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot);

/**
 * @brief Returns the string representation of the 2D vector using default formatting.
 * @param v The 2D vector.
 * @return The serialized 2D vector as `(X, Y)`.
 */
CARBON_API char *carbon_math_vec2_to_cstr(CBN_Vec2 v);

/**
 * @brief Returns the string representation of the 3D vector using default formatting.
 * @param v The 3D vector.
 * @return The serialized 3D vector as `(X, Y, Z)`.
 */
CARBON_API char *carbon_math_vec3_to_cstr(CBN_Vec3 v);

/**
 * @brief Determines whether the specified point is contained within a rectangle.
 * @param r The rectangle.
 * @param p The point to check.
 * @return A boolean value representing whether the point is contained within this rectangle or not.
 */
CARBON_API u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p);

/**
 * @brief Determines whether the rectangles collide/overlap/intersect with each other.
 * @param r1 The first rectangle.
 * @param r2 The second rectangle.
 * @return A boolean value representing whether they overlap (also touching).
 */
CARBON_API u8 carbon_math_rect_detect_collision(CBN_Rect r1, CBN_Rect r2);

CARBON_API CBN_Matrix carbon_math_mat_create(usz rows, usz cols);
CARBON_API void carbon_math_mat_destroy(CBN_Matrix *m);
CARBON_API void carbon_math_mat_fill(CBN_Matrix m, f32 x);
CARBON_API void carbon_math_mat_rand(CBN_Matrix m, f32 min, f32 max);
CARBON_API CBN_Row carbon_math_mat_row(CBN_Matrix m, usz row);
CARBON_API void carbon_math_mat_copy(CBN_Matrix dst, CBN_Matrix src);
CARBON_API void carbon_math_mat_add(CBN_Matrix dst, CBN_Matrix m);
CARBON_API void carbon_math_mat_dot(CBN_Matrix dst, CBN_Matrix a, CBN_Matrix b);
CARBON_API void carbon_math_mat_map(CBN_Matrix m, f32 (*f)(f32));
CARBON_API void carbon_math_mat_print(CBN_Matrix m, const char *name);
CARBON_API CBN_Row carbon_math_row_create(usz cols);
CARBON_API void carbon_math_row_destroy(CBN_Row *r);
CARBON_API CBN_Matrix carbon_math_row_to_mat(CBN_Row r);
CARBON_API CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols);
CARBON_API void carbon_math_row_fill(CBN_Row r, f32 x);
CARBON_API void carbon_math_row_rand(CBN_Row r, f32 min, f32 max);
CARBON_API void carbon_math_row_copy(CBN_Row dst, CBN_Row src);
CARBON_API void carbon_math_row_print(CBN_Row r, const char *name);

/*
**  $$====================$$
**  ||       Crypto       ||
**  $$====================$$
*/

typedef struct {
  u64 hash[25];
  u64 msg[24];
  u16 rest;
} CBN_Keccak256;

/**
 * @brief Encodes binary data to a newly-allocated Base64 string.
 *
 * The caller of this function is responsible for freeing this memory using `CARBON_FREE`
 * or equivalent.
 *
 * @param in The input binary data to be encoded.
 * @param in_size The size of the input binary data in bytes.
 * @param out_size The size of the resulting Base64 string (output argument pointer).
 * @return The resulting Base64 string.
 */
CARBON_API char *carbon_crypto_base64_encode(const u8 *in, const usz in_size, usz *out_size);

/**
 * @brief Decodes a Base64 string to newly-allocated binary data buffer.
 *
 * The caller of this function is responsible for freeing this memory using `CARBON_FREE`
 * or equivalent.
 *
 * @param in The input Base64 string to be decoded.
 * @param out_size The size of the resulting binary data in bytes (output argument pointer).
 * @return The resulting binary data buffer.
 */
CARBON_API u8 *carbon_crypto_base64_decode(const u8 *in, usz *out_size);

/**
 * @brief Computes the CRC32 checksum of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The 32-bit CRC32 checksum of the input binary data.
 */
CARBON_API u32 carbon_crypto_crc32(const u8 *in, const usz in_size);

/**
 * @brief Computes the Keccak-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @param out The resulting Keccak-256 hash as a 32-byte buffer (output argument pointer).
 */
CARBON_API void carbon_crypto_keccak256(const u8 *in, const usz in_size, u8 *out);

/**
 * @brief Converts a 32-byte buffer containing a Keccak-256 hash to its HEX string representation.
 *
 * The resulting HEX string buffer (`out_cstr`) must be at least 65 characters long (32*2 + 1).
 *
 * @param hash The Keccak-256 hash as a 32-byte buffer.
 * @param out_cstr The resulting HEX string representation of the Keccak-256 hash (output argument pointer).
 */
CARBON_API void carbon_crypto_keccak256_to_hex_cstr(const u8 *hash, char *out_cstr);

/**
 * @brief Computes the Keccak-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The resulting Keccak-256 hash as a HEX string.
 */
CARBON_API char *carbon_crypto_keccak256_as_hex_cstr(const u8 *in, const usz in_size);

/*
**  $$=====================$$
**  ||       Logging       ||
**  $$=====================$$
*/
#define CARBON_COLOR_RESET   "\033[0m"
#define CARBON_COLOR_RED     "\033[1;31m"
#define CARBON_COLOR_GREEN   "\033[1;32m"
#define CARBON_COLOR_YELLOW  "\033[1;33m"
#define CARBON_COLOR_MAGENTA "\033[1;35m"
#define CARBON_COLOR_CYAN    "\033[1;36m"

#define carbon_log_debug(msg, ...) carbon_println(CARBON_COLOR_CYAN "[^] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_info(msg, ...) carbon_println(CARBON_COLOR_YELLOW "[*] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_warn(msg, ...) carbon_println(CARBON_COLOR_MAGENTA "[?] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_error(msg, ...) carbon_eprintln(CARBON_COLOR_RED "[!] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)

#ifndef __cplusplus
#define carbon_log__var_to_spec(x)              \
  _Generic((x),                                 \
           usz: "%zu",                          \
           isz: "%zd",                          \
           u8: "%hhu",                          \
           i8: "%hhd",                          \
           u16: "%hu",                          \
           i16: "%hd",                          \
           u32: "%u",                           \
           i32: "%d",                           \
           u64: "%llu",                         \
           i64: "%lld",                         \
           f32: "%f",                           \
           f64: "%lf",                          \
           char: "%c",                          \
           char *: "%s",                        \
           const char *: "%s",                  \
           default: "<'Stuff' At %p>")
#else
template <typename T> constexpr const char *carbon_log__var_to_spec(T)  { return "<'Stuff' At %p>"; }
template <> constexpr const char *carbon_log__var_to_spec(usz)          { return "%zu"; }
template <> constexpr const char *carbon_log__var_to_spec(isz)          { return "%zd"; }
template <> constexpr const char *carbon_log__var_to_spec(u8)           { return "%hhu"; }
template <> constexpr const char *carbon_log__var_to_spec(i8)           { return "%hhd"; }
template <> constexpr const char *carbon_log__var_to_spec(u16)          { return "%hu"; }
template <> constexpr const char *carbon_log__var_to_spec(i16)          { return "%hd"; }
template <> constexpr const char *carbon_log__var_to_spec(u32)          { return "%u"; }
template <> constexpr const char *carbon_log__var_to_spec(i32)          { return "%d"; }
template <> constexpr const char *carbon_log__var_to_spec(u64)          { return "%llu"; }
template <> constexpr const char *carbon_log__var_to_spec(i64)          { return "%lld"; }
template <> constexpr const char *carbon_log__var_to_spec(f32)          { return "%f"; }
template <> constexpr const char *carbon_log__var_to_spec(f64)          { return "%lf"; }
template <> constexpr const char *carbon_log__var_to_spec(char)         { return "%c"; }
template <> constexpr const char *carbon_log__var_to_spec(char *)       { return "%s"; }
template <> constexpr const char *carbon_log__var_to_spec(const char *) { return "%s"; }
#endif

#define $(x) (CBN_Log_BoxedVar){carbon_log__var_to_spec(x), (void *) &x}

#define carbon_print(msg, ...) carbon_log_print(stdout, msg, ##__VA_ARGS__)
#define carbon_eprint(msg, ...) carbon_log_print(stderr, msg, ##__VA_ARGS__)
#define carbon_println(msg, ...) carbon_print(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_eprintln(msg, ...) carbon_eprint(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)

#define carbon_cprint(color, msg, ...) carbon_print(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_ceprint(color, msg, ...) carbon_eprint(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_cprintln(color, msg, ...) carbon_println(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_ceprintln(color, msg, ...) carbon_eprintln(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)

typedef struct {
  const char *spec;
  void *var;
} CBN_Log_BoxedVar;

CARBON_API void carbon_log_print(FILE *stream, const char *fmt, ...);

/*
**  $$====================$$
**  ||       Should       ||
**  $$====================$$
*/
#define CARBON_COMPARE(expr, msg, ...)                                  \
  if ((expr)) {                                                         \
    carbon_ceprintln(CARBON_COLOR_RED, "%s:%d :: FAILED -> " msg,       \
                     __FILE__, __LINE__, ##__VA_ARGS__);                \
    return CARBON_KO;                                                   \
  }

#define CARBON_COMPARE_OP(T, expected, actual, op, msg, ...)            \
  CARBON_COMPARE((T) (expected) op (T) (actual), msg, ##__VA_ARGS__)

#define carbon_should_be_true(actual) CARBON_COMPARE(!(actual), "got 'false', expected 'true'")

#define carbon_should_be_false(actual) CARBON_COMPARE(actual, "got 'true', expected 'false'")

#define carbon_should_be(expected, actual)              \
  CARBON_COMPARE_OP(i32, expected, actual, !=,          \
                    "got '%d', expected '%d'",          \
                    (i32) (actual), (i32) (expected))

#define carbon_should_be_u64(expected, actual)          \
  CARBON_COMPARE_OP(u64, expected, actual, !=,          \
                    "got '%llu', expected '%llu'",      \
                    (u64) (actual), (u64) (expected))

#define carbon_should_be_f(expected, actual)                    \
  CARBON_COMPARE(carbon_math_abs(expected - actual) > 1e-6,     \
                 "got '%f', expected '%f'",                     \
                 (f32) (actual), (f32) (expected))

#define carbon_should_not_be(expected, actual)          \
  CARBON_COMPARE_OP(i32, expected, actual, ==,          \
                    "got '%d == %d', expected not to",  \
                    (i32) (actual), (i32) (expected))

#define carbon_should_be_lt(expected, actual)                           \
  CARBON_COMPARE_OP(i32, expected, actual, <=,                          \
                    "got '%d >= %d', expected '%d < %d'",               \
                    (i32) (actual), (i32) (expected), (i32) (actual), (i32) (expected))

#define carbon_should_be_le(expected, actual)                           \
  CARBON_COMPARE_OP(i32, expected, actual, <,                           \
                    "got '%d > %d', expected '%d <= %d'",               \
                    (i32) (actual), (i32) (expected), (i32) (actual), (i32) (expected))

#define carbon_should_be_gt(expected, actual)                           \
  CARBON_COMPARE_OP(i32, expected, actual, >=,                          \
                    "got '%d <= %d', expected '%d > %d'",               \
                    (i32) (actual), (i32) (expected), (i32) (actual), (i32) (expected))

#define carbon_should_be_ge(expected, actual)                           \
  CARBON_COMPARE_OP(i32, expected, actual, >,                           \
                    "got '%d < %d', expected '%d >= %d'",               \
                    (i32) (actual), (i32) (expected), (i32) (actual), (i32) (expected))

#define carbon_should_be_p(expected, actual)                    \
  CARBON_COMPARE_OP(void *, expected, actual, !=,               \
                    "got '%p', expected '%p'",                  \
                    (void *) (actual), (void *) (expected))

#define carbon_should_not_be_p(expected, actual)                \
  CARBON_COMPARE_OP(void *, expected, actual, ==,               \
                    "got '%p == %p', expected not to",          \
                    (void *) (actual), (void *) (expected))

#define carbon_should_be_s(expected, actual)                            \
  CARBON_COMPARE(carbon_string_cmp((char *) (expected), (char *) (actual)), \
                 "got '%s', expected '%s'",                             \
                 (char *) (actual), (char *) (expected))

#define carbon_should_not_be_s(expected, actual)                        \
  CARBON_COMPARE(!carbon_string_cmp((char *) (expected), (char *) (actual)), \
                 "got '%s', expected '%s'",                             \
                 (char *) (actual), (char *) (expected))

/*
**  $$==================$$
**  ||       Time       ||
**  $$==================$$
*/
typedef struct {
  u64 timestamp;
  u64 random;
} CBN_SnowflakeComponents;

CARBON_API f64 carbon_time_get(void);
CARBON_API char *carbon_time_get_iso8601(void);

/**
 * @brief Returns the current timestamp in the `YYYYmmddHHMMSS` format.
 *
 * The aforementioned format consists of: (YYYY = year; mm = month; dd = day; HH = hour; MM = minute; SS = second).
 *
 * @return The formatted timestamp as 64-bit uint number, e.g. `20250511205156`.
 */
CARBON_API u64 carbon_time_get_yyyymmddhhmmss(void);

CARBON_API void carbon_time_sleep(u64 ms);
CARBON_API u64 carbon_time_snowflake_get(void);
CARBON_API CBN_SnowflakeComponents carbon_time_snowflake_parse(u64 snowflake);

/*
**  $$===================$$
**  ||       Clock       ||
**  $$===================$$
*/
typedef struct {
  f64 initial;
  f64 elapsed;
} CBN_Clock;

CARBON_API CBN_Clock carbon_clock_start(void);
CARBON_API void carbon_clock_update(CBN_Clock *c);
CARBON_API void carbon_clock_stop(CBN_Clock *c);

/*
**  $$==================$$
**  ||       List       ||
**  $$==================$$
*/

/**
 * @brief Represents a sequence container that encapsulates dynamic size arrays.
 *
 * In C++, this is a templated class/struct, which means it's not a type by itself,
 * until it gets instantiated with the needed template arguments. The type `CBN_List`
 * is an alias for `CBN_List_t<void *>`, which provides full-compatibility with the
 * C API, even through C++ templates.
 *
 * @param T Type information of what will be stored in the container.
 */
CARBON_TEMPLATE_CLASS(CBN_List_t) {
  void *items;
  usz capacity;
  usz stride;
  usz size;
#ifdef __cplusplus
  using value_type = T;
  using iterator = T *;
  using const_iterator = const T *;
  /**
   * @brief carbon_list_create
   * @return The list container.
   */
  static CBN_List_t make(void);
  /**
   * @brief carbon_list_destroy
   */
  void Free(void);
  /**
   * @brief carbon_list_push
   * @param value The value to append.
   */
  void Push(const value_type &value);
  /**
   * @brief carbon_list_pop
   * @return The value of the element popped out.
   */
  T Pop(void);
  /**
   * @brief carbon_list_find
   * @param value The value of the element to check.
   * @return The index of the provided element, or -1 if not present.
   */
  isz Find(const value_type &value) const;
  /**
   * @brief carbon_list_remove
   * @param idx The index of the element to remove.
   */
  void Remove(usz idx);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void);
  /**
   * @brief Returns a constant iterator to the beginning.
   * @return The const_iterator.
   */
  const_iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void);
  /**
   * @brief Returns a constant iterator to the end.
   * @return The const_iterator.
   */
  const_iterator end(void) const;
  // Overloaded Operators
  T &operator[](usz idx);
  const T &operator[](usz idx) const;
#endif
};

#ifdef __cplusplus
using CBN_List = CBN_List_t<void *>;
#else
typedef struct CBN_List_t CBN_List;
#endif

#define carbon_list_at_raw(T, l, i) ((T *) (l).items)[(i)]
#define carbon_list_at(T, l, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (l).size && "List index out of bounds"), CARBON_ASSERT(sizeof(T) == (l).stride && "List type doesn't match"), carbon_list_at_raw(T, l, i))
#define carbon_list_foreach(T, l) for (struct { usz i; T var; } it = {0, carbon_list_at(T, l, 0)}; it.i < (l).size; ++it.i, it.i < (l).size ? it.var = carbon_list_at(T, l, it.i) : it.var)

/**
 * @brief Create a new list container.
 * @param stride Size in bytes of the elements the list will hold.
 * @return The list container.
 */
CARBON_API CBN_List carbon_list_create(usz stride);

/**
 * @brief Destroy a list container.
 * @param l The list container.
 */
CARBON_API void carbon_list_destroy(CBN_List *l);

/**
 * @brief Appends a copy of the value to the end of the list.
 * @param l The list container.
 * @param value The value to append.
 */
CARBON_API void carbon_list_push(CBN_List *l, void *value);

/**
 * @brief Removes the last element from the list.
 * @param l The list container.
 * @param out_value The value of the element popped out.
 */
CARBON_API void carbon_list_pop(CBN_List *l, void *out_value);

/**
 * @brief Obtains the index of the provided element, or -1 if not present.
 * @param l The list container.
 * @param value The value of the element to check.
 * @return The index of the provided element, or -1 if not present.
 */
CARBON_API isz carbon_list_find(const CBN_List *l, const void *value);

/**
 * @brief Removes the element specified by the provided index from the list.
 * @param l The list container.
 * @param idx The index of the element to remove.
 */
CARBON_API void carbon_list_remove(CBN_List *l, usz idx);

// Because `CBN_List_t` is a templated class/struct, the definitions of all its
// member functions need to be available at compile-time, not link-time; thus,
// we define them here for the template instantiation to work properly.
#ifdef __cplusplus
/**
 * @brief CBN_List_t<T>::make
 */
template <typename T>
CBN_List_t<T> CBN_List_t<T>::make(void) {
  auto l = carbon_list_create(sizeof(T));
  return *(CBN_List_t<T> *) &l;
}
/**
 * @brief CBN_List_t<T>.Free
 */
template <typename T>
void CBN_List_t<T>::Free(void) {
  carbon_list_destroy((CBN_List *) this);
}
/**
 * @brief CBN_List_t<T>.Push
 */
template <typename T>
void CBN_List_t<T>::Push(const value_type &value) {
  carbon_list_push((CBN_List *) this, (void *) &value);
}
/**
 * @brief CBN_List_t<T>.Pop
 */
template <typename T>
T CBN_List_t<T>::Pop(void) {
  T x;
  carbon_list_pop(this, &x);
  return x;
}
/**
 * @brief CBN_List_t<T>.Find
 */
template <typename T>
isz CBN_List_t<T>::Find(const value_type &value) const {
  return carbon_list_find(this, value);
}
/**
 * @brief CBN_List_t<T>.Remove
 */
template <typename T>
void CBN_List_t<T>::Remove(usz idx) {
  carbon_list_remove(this, idx);
}
/**
 * @brief CBN_List_t<T>.begin
 */
template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).begin());
}
/**
 * @brief CBN_List_t<T>.begin (const)
 */
template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::begin(void) const {
  return static_cast<const_iterator>(items);
}
/**
 * @brief CBN_List_t<T>.end
 */
template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::end(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).end());
}
/**
 * @brief CBN_List_t<T>.end (const)
 */
template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::end(void) const {
  return static_cast<const_iterator>(items) + size;
}
/**
 * @brief CBN_List_t<T>[idx]
 */
template <typename T>
T &CBN_List_t<T>::operator[](usz idx) {
  return const_cast<T &>(static_cast<const CBN_List_t &>(*this)[idx]);
}
/**
 * @brief CBN_List_t<T>[idx] (const)
 */
template <typename T>
const T &CBN_List_t<T>::operator[](usz idx) const {
  CARBON_ASSERT(0 <= idx && idx < size && "List index out of bounds");
  CARBON_ASSERT(sizeof(T) == stride && "List type doesn't match");
  return ((T *) items)[idx];
}
#endif  // __cplusplus

/*
**  $$=====================$$
**  ||       HashMap       ||
**  $$=====================$$
*/
typedef struct {
  void *items;
  usz capacity;
  usz stride;
} CBN_HashMap;

CARBON_API CBN_HashMap carbon_hashmap_create(usz capacity, usz stride);
CARBON_API void carbon_hashmap_destroy(CBN_HashMap *hm);
CARBON_API void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value);
CARBON_API void carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value);

/*
**  $$====================$$
**  ||       String       ||
**  $$====================$$
*/
CARBON_API usz carbon_string_len(const char *s);
CARBON_API i32 carbon_string_cmp(const char *s1, const char *s2);
CARBON_API i32 carbon_string_cmp_n(const char *s1, const char *s2, usz size);
CARBON_API char *carbon_string_dup(const char *s);
CARBON_API char *carbon_string_fmt(const char *s, ...);
CARBON_API void carbon_string_strip_substr(char *s, const char *sub);
CARBON_API u8 carbon_string_starts_with_substr(const char *s, const char *sub);
CARBON_API u8 carbon_string_ends_with_substr(const char *s, const char *sub);
CARBON_API u8 carbon_string_is_number(const char *s);
CARBON_API u8 carbon_string_has_char(const char *s, char c);

/**
 * @brief Gets the Levenshtein distance between two strings.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return The Levenshtein distance between s1 and s2.
 */
CARBON_API usz carbon_string_lev_dist(const char *s1, const char *s2);

/*
**  $$=====================$$
**  ||       StrView       ||
**  $$=====================$$
*/

typedef struct CBN_StrBuilder CBN_StrBuilder;  // Forward declaration

/**
 * @brief Represents a view into a string of characters, whilst not owning the actual memory.
 */
typedef struct CBN_StrView {
  const char *data;
  usz size;
#ifdef __cplusplus
  /**
   * @brief carbon_strview_from_buf
   * @param data The pointer to the char buffer.
   * @param size The size of the char buffer.
   * @return The StrView representing the provided char buffer.
   */
  static CBN_StrView make(const char *data, usz size);
  /**
   * @brief carbon_strview_from_cstr
   * @param s The pointer to the standard C char array.
   * @return The StrView representing the provided C-style string.
   */
  static CBN_StrView make(const char *s);
  /**
   * @brief carbon_strview_from_strbuilder
   * @param sb The StrBuilder holding the data.
   * @return The StrView representing the provided StrBuilder object.
   */
  static CBN_StrView make(const CBN_StrBuilder &sb);
  /**
   * @brief carbon_strview_to_cstr
   * @return A pointer to a null-terminated char array with same data.
   */
  const char *c_str(void) const;
  /**
   * @brief carbon_strview_trim_both
   * @return The StrView that remains after all whitespace characters are removed from the start and the end.
   */
  CBN_StrView Trim(void) const;
  /**
   * @brief carbon_strview_trim_left
   * @return The StrView that remains after all whitespace characters are removed from the start.
   */
  CBN_StrView TrimStart(void) const;
  /**
   * @brief carbon_strview_trim_right
   * @return The StrView that remains after all whitespace characters are removed from the end.
   */
  CBN_StrView TrimEnd(void) const;
  /**
   * @brief carbon_strview_chop_by_space
   * @return The StrView containing the data from the beginning up to the first occurrence of the whitespace character.
   */
  CBN_StrView Chop(void);
  /**
   * @brief carbon_strview_chop
   * @param c The char delimiter to search for.
   * @return The StrView containing the data from the beginning up to the first occurrence of the delimiter.
   */
  CBN_StrView Chop(char c);
  /**
   * @brief carbon_string_lev_dist
   * @param sv The other StrView.
   * @return The Levenshtein distance between both StrViews.
   */
  usz LevDist(const CBN_StrView &sv) const;
  // Overloaded Operators
  bool operator==(const CBN_StrView &sv) const;
  bool operator==(const char *s) const;
  friend bool operator==(const char *s, const CBN_StrView &sv);
  bool operator!=(const CBN_StrView &sv) const;
  bool operator!=(const char *s) const;
  friend bool operator!=(const char *s, const CBN_StrView &sv);
#endif
} CBN_StrView;

/**
 * @brief Creates a StrView from a char buffer pointer and its size.
 * @param data The pointer to the char buffer.
 * @param size The size of the char buffer.
 * @return The StrView representing the provided char buffer.
 */
CARBON_API CBN_StrView carbon_strview_from_buf(const char *data, usz size);

/**
 * @brief Creates a StrView from a null-terminated C-style string.
 * @param s The pointer to the standard C char array.
 * @return The StrView representing the provided C-style string.
 */
CARBON_API CBN_StrView carbon_strview_from_cstr(const char *s);

/**
 * @brief Creates a StrView from a StrBuilder object holding some data.
 * @param sb The StrBuilder holding the data.
 * @return The StrView representing the provided StrBuilder object.
 */
CARBON_API CBN_StrView carbon_strview_from_strbuilder(const CBN_StrBuilder *sb);

/**
 * @brief Returns a non-modifiable standard C char array version of the string.
 * @param sv The StrView holding the data.
 * @return A pointer to a null-terminated char array with same data.
 */
CARBON_API char *carbon_strview_to_cstr(CBN_StrView sv);

/**
 * @brief Removes all the leading whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the start.
 */
CARBON_API CBN_StrView carbon_strview_trim_left(CBN_StrView sv);

/**
 * @brief Removes all the trailing whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the end.
 */
CARBON_API CBN_StrView carbon_strview_trim_right(CBN_StrView sv);

/**
 * @brief Removes all leading and trailing whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the start and the end.
 */
CARBON_API CBN_StrView carbon_strview_trim_both(CBN_StrView sv);

/**
 * @brief Returns the leading portion of the current StrView just before the first occurrence of a delimiter.
 *
 * The StrView is taken as a pointer, because it's modified to point to the data after the first occurrence of the delimiter.
 *
 * @param sv The StrView holding the data.
 * @param c The char delimiter to search for.
 * @return The StrView containing the data from the beginning up to the first occurrence of the delimiter.
 */
CARBON_API CBN_StrView carbon_strview_chop(CBN_StrView *sv, char c);

/**
 * @brief Returns the leading portion of the current StrView just before the first occurrence of a whitespace character.
 *
 * The StrView is taken as a pointer, because it's modified to point to the data after the first occurrence of the whitespace character.
 *
 * @param sv The StrView holding the data.
 * @return The StrView containing the data from the beginning up to the first occurrence of the whitespace character.
 */
CARBON_API CBN_StrView carbon_strview_chop_by_space(CBN_StrView *sv);

/**
 * @brief Compares two StrViews and returns whether they're equal.
 * @param x The first StrView.
 * @param y The second StrView.
 * @return A boolean value representing the equality between them.
 */
CARBON_API u8 carbon_strview_are_equal(CBN_StrView x, CBN_StrView y);

CARBON_API u8 carbon_strview_starts_with(CBN_StrView sv, CBN_StrView sub);
CARBON_API u8 carbon_strview_ends_with(CBN_StrView sv, CBN_StrView sub);

/*
**  $$========================$$
**  ||       StrBuilder       ||
**  $$========================$$
*/

/**
 * @brief Represents a mutable string of characters.
 */
typedef struct CBN_StrBuilder {
  char *items;
  usz size;
  usz capacity;
#ifdef __cplusplus
  /**
   * @brief carbon_fs_read_entire_file
   *
   * It may throw a `std::runtime_error` if there was a problem reading the file.
   *
   * @param file The filename to read the data from.
   * @return The StrBuilder object where the read data will be stored.
   */
  static CBN_StrBuilder make(const char *file);
  /**
   * @brief carbon_strbuilder_free
   */
  void Free(void);
  /**
   * @brief carbon_strbuilder_add_null
   */
  void Push(void);
  /**
   * @brief carbon_strbuilder_add_cstr
   * @param s The string to append.
   */
  void Push(const char *s);
  /**
   * @brief carbon_strbuilder_add_strview
   * @param sv The StrView to append.
   */
  void Push(const CBN_StrView &sv);
  /**
   * @brief StrView::make(CBN_StrBuilder const&)
   * @return The StrView representing the current StrBuilder object.
   */
  CBN_StrView ToString(void) const;
  // Overloaded Operators
  CBN_StrBuilder &operator<<(const char *s);
  CBN_StrBuilder &operator<<(const CBN_StrView &sv);
#endif
} CBN_StrBuilder;

/**
 * @brief Appends a copy of the StrView to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param sv The StrView to append.
 */
CARBON_API void carbon_strbuilder_add_strview(CBN_StrBuilder *sb, CBN_StrView sv);

/**
 * @brief Appends a copy of the string to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param s The string to append.
 */
CARBON_API void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s);

/**
 * @brief Appends an empty string (null terminator) to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 */
CARBON_API void carbon_strbuilder_add_null(CBN_StrBuilder *sb);

/**
 * @brief Frees (deallocates) and cleans up the CBN_StrBuilder object.
 * @param sb The StrBuilder object.
 */
CARBON_API void carbon_strbuilder_free(CBN_StrBuilder *sb);

/*
**  $$=====================$$
**  ||       StrList       ||
**  $$=====================$$
*/

/**
 * @brief Represents a CBN_List containter specifically for strings.
 */
typedef struct CBN_StrList {
  char **items;
  usz size;
  usz capacity;
  u8 unique;
#ifdef __cplusplus
  using iterator = const char **;
  using const_iterator = const char * const *;
  /**
   * @brief carbon_strlist_create(unique = false)
   * @return The new StrList, configured to hold repeated strings.
   */
  static CBN_StrList make(void);
  /**
   * @brief carbon_strlist_from_splitted_cstr
   * @param s The C-style string to split.
   * @param delim The delimiter as a C-style string.
   * @return The StrList containing the resulting strings.
   */
  static const CBN_StrList make(const char *s, const char *delim);
  /**
   * @brief carbon_strlist_create(unique = true)
   * @return The new StrList, configured to hold only unique strings.
   */
  static CBN_StrList make_unique(void);
  /**
   * @brief carbon_strlist_destroy
   */
  void Free(void);
  /**
   * @brief carbon_strlist_push
   * @param s The C-style string to append.
   */
  void Push(const char *s);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void);
  /**
   * @brief Returns a constant iterator to the beginning.
   * @return The const_iterator.
   */
  const_iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void);
  /**
   * @brief Returns a constant iterator to the end.
   * @return The const_iterator.
   */
  const_iterator end(void) const;
#endif
} CBN_StrList;

#define carbon_strlist_at(sl, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (sl).size && "StrList index out of bounds"), ((sl).items)[(i)])
#define carbon_strlist_foreach(sl) for (struct { usz i; CBN_StrView sv; } it = {0, carbon_strview_from_cstr(carbon_strlist_at(sl, 0))}; it.i < (sl).size; ++it.i, it.i < (sl).size ? it.sv = carbon_strview_from_cstr(carbon_strlist_at(sl, it.i)) : it.sv)

/**
 * @brief Create a new StrList container, ready to hold strings.
 * @param unique A boolean to configure whether we want the StrList to hold repeated strings or not.
 * @return The new StrList, configured to your liking.
 */
CARBON_API CBN_StrList carbon_strlist_create(u8 unique);

/**
 * @brief Splits a C-style string into a StrList, based on a delimiter.
 * @param s The C-style string to split.
 * @param delim The delimiter as a C-style string.
 * @return The StrList containing the resulting strings.
 */
CARBON_API CBN_StrList carbon_strlist_from_splitted_cstr(const char *s, const char *delim);

/**
 * @brief Destroy a StrList container.
 * @sl The StrList container.
 */
CARBON_API void carbon_strlist_destroy(CBN_StrList *sl);

/**
 * @brief Appends a copy of the string to the end of the container.
 * @param sl The StrList container.
 * @param s The pointer to the standard C char array.
 */
CARBON_API void carbon_strlist_push(CBN_StrList *sl, const char *s);

CARBON_API void carbon_strlist_pop(CBN_StrList *sl, const char *s);
CARBON_API u8 carbon_strlist_contains(CBN_StrList *sl, const char *s);

/*
**  $$========================$$
**  ||       DrawCanvas       ||
**  $$========================$$
*/
#define CARBON_DRAWCANVAS_AT(dc, i, j) (dc).pixels[(j) * (dc).stride + (i)]

typedef struct {
  u32 *pixels;
  usz width;
  usz height;
  usz stride;
} CBN_DrawCanvas;

CARBON_API CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height);
CARBON_API void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc);
CARBON_API void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color);
CARBON_API void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);
CARBON_API void carbon_drawcanvas_rect(CBN_DrawCanvas dc, CBN_Rect r, u32 color);
CARBON_API void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color);

/**
 * @brief Transforms HSV color into RGB (A always set to 0xff).
 * @param h Hue, in [0..360] range.
 * @param s Saturation, in [0..1] range.
 * @param v Value, in [0..1] range.
 * @return 32-bit RGBA color value (A always set to 0xff).
 */
CARBON_API u32 carbon_drawcanvas_hsv_to_rgb(f32 h, f32 s, f32 v);

/*
**  $$========================$$
**  ||       Filesystem       ||
**  $$========================$$
*/
#define carbon_fs_pattern_match_foreach(pmf) for (struct { usz i; char *f; } it = {0, (pmf).files[0]}; it.i < (pmf).count; ++it.i, it.i < (pmf).count ? it.f = (pmf).files[it.i] : it.f)

typedef enum {
  CBN_FILE_FORMAT_PNG,
  CBN_FILE_FORMAT_BMP,
  CBN_FILE_FORMAT_TGA,
  CBN_FILE_FORMAT_JPG,
} CBN_FileFormat;

typedef struct CBN_PatternMatchedFiles {
  char **files;
  usz count;
#ifdef __cplusplus
  using iterator = const char **;
  using const_iterator = const char * const *;
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void);
  /**
   * @brief Returns a constant iterator to the beginning.
   * @return The const_iterator.
   */
  const_iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void);
  /**
   * @brief Returns a constant iterator to the end.
   * @return The const_iterator.
   */
  const_iterator end(void) const;
#endif
} CBN_PatternMatchedFiles;

/**
 * @brief Represents the needed metadata for an image object definition.
 */
typedef struct {
  usz width;
  usz height;
  usz channels;
} CBN_Image_Metadata;

/**
 * @brief Represents an image object as a linear buffer of 8-bit per channel RGBA bytes.
 */
typedef struct CBN_Image {
  u8 *data;
  CBN_Image_Metadata metadata;
#ifdef __cplusplus
  /**
   * @brief carbon_fs_read_img_from_file
   * @param file The filename to read the image from.
   * @return The image object.
   */
  static CBN_Image make(const char *file);
  /**
   * @brief carbon_fs_img_32bit_to_8bit
   * @return The image object.
   */
  static CBN_Image make(const CBN_DrawCanvas &dc);
  /**
   * @brief carbon_fs_destroy_img
   */
  void Free(void);
  /**
   * @brief carbon_fs_write_img_to_file
   * @param fmt The format of the image file to write to.
   * @param file The filename to write the image to.
   * @return A boolean value representing the success of the operation.
   */
  u8 Save(CBN_FileFormat fmt, const char *file) const;
#endif
} CBN_Image;

CARBON_API u8 carbon_fs_exists(const char *file);
CARBON_API u8 carbon_fs_is_regular_file(const char *file);
CARBON_API u8 carbon_fs_is_directory(const char *file);
CARBON_API u8 carbon_fs_rename(const char *oldie, const char *newie);
CARBON_API i32 carbon_fs_mtime(const char *file);
CARBON_API void carbon_fs_copy(const char *from, const char *to, u8 recursive);
CARBON_API u8 carbon_fs_remove(const char *file);
CARBON_API u8 carbon_fs_remove_all(const char *file);
CARBON_API u8 carbon_fs_change_directory(const char *path);
CARBON_API u8 carbon_fs_create_directory(const char *path);
CARBON_API u8 carbon_fs_create_directories(const char *path);
CARBON_API char *carbon_fs_get_bin_directory(void);
CARBON_API CBN_PatternMatchedFiles carbon_fs_pattern_match(const char *pattern);
CARBON_API u32 carbon_fs_get_file_size(const char *file);

/**
 * @brief Reads all the text data from the given file into a StrBuilder object.
 * @param sb The StrBuilder object where the read data will be stored.
 * @param file The filename to read the data from.
 * @return A boolean value representing the success of the operation.
 */
CARBON_API u8 carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file);

/**
 * @brief Reads an image from disk.
 * @param file The filename to read the image from.
 * @return The image object.
 */
CARBON_API CBN_Image carbon_fs_read_img_from_file(const char *file);

CARBON_API CBN_List carbon_fs_read_img_from_file_as_tensor(const char *file);

/**
 * @brief Reads an image from disk as a linear buffer of 8-bit per channel RGBA bytes.
 * @param file The filename to read the image from.
 * @param out_width The width of the read image (output argument pointer).
 * @param out_height The height of the read image (output argument pointer).
 * @param out_chs The number of channels of the read image (output argument pointer).
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CARBON_API u8 *carbon_fs_read_img_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs);

CARBON_API CBN_List carbon_fs_img_tensorize(u8 *pixels, usz width, usz height, usz chs);

CARBON_API u8 *carbon_fs_img_linearize(CBN_List *img);

/**
 * @brief Converts a linear buffer of 32-bit RGBA pixels to a linear buffer of 8-bit per channel RGBA bytes.
 *
 * This function allocates memory in the heap using `CARBON_MALLOC`; thus, it's important to keep in mind the
 * lifetime of that allocation and free it accordingly using `CARBON_FREE`.
 *
 * @param pixels The pointer to the 32-bit RGBA linear buffer of pixels.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CARBON_API u8 *carbon_fs_img_32bit_to_8bit(const u32 *pixels, const usz width, const usz height);

/**
 * @brief Writes an image to disk.
 * @param img The image object.
 * @param fmt The format of the image file to write to.
 * @param file The filename to write the image to.
 * @return A boolean value representing the success of the operation.
 */
CARBON_API u8 carbon_fs_write_img_to_file(const CBN_Image *img, CBN_FileFormat fmt, const char *file);

CARBON_API u8 carbon_fs_write_tensor_img_to_file(CBN_List *img, CBN_FileFormat fmt, const char *file);

CARBON_API u8 carbon_fs_write_img_to_file_linearly(u8 *pixels, CBN_FileFormat fmt, usz width, usz height, usz chs, const char *file);

/**
 * @brief Destroy an image object.
 * @param img The image object.
 */
CARBON_API void carbon_fs_destroy_img(CBN_Image *img);

CARBON_API void carbon_fs_destroy_tensor_img(CBN_List *img);

/*
**  $$=====================$$
**  ||       Network       ||
**  $$=====================$$
*/

/**
 * @brief Translate via DNS lookup a domain name to IP address format (IPv4 or IPv6).
 * @param domain The domain name to resolve.
 * @return A string representing the resolved IP address.
 */
CARBON_API char *carbon_net_resolve_dns_to_ip(const char *domain);

/*
**  $$=======================$$
**  ||       NeuralNet       ||
**  $$=======================$$
*/
#define CARBON_NN_IN(nn) (CARBON_ASSERT((nn).arch_count > 0), (nn).as[0])
#define CARBON_NN_OUT(nn) (CARBON_ASSERT((nn).arch_count > 0), (nn).as[(nn).arch_count - 1])
#define CARBON_NN_SIGMOID_DX(x) ((x) * (1 - (x)))
#define CARBON_NN_PRINT(nn) carbon_nn_print(nn, #nn)

typedef struct {
  usz *arch;
  usz arch_count;
  CBN_Matrix *ws;
  CBN_Row *bs;
  CBN_Row *as;
} CBN_NeuralNet;

CARBON_API CBN_NeuralNet carbon_nn_create(usz *arch, usz arch_count);
CARBON_API void carbon_nn_destroy(CBN_NeuralNet *nn);
CARBON_API void carbon_nn_fill(CBN_NeuralNet nn, f32 x);
CARBON_API void carbon_nn_zero(CBN_NeuralNet nn);
CARBON_API void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max);
CARBON_API void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max);
CARBON_API void carbon_nn_forward(CBN_NeuralNet nn);
CARBON_API f32 carbon_nn_cost(CBN_NeuralNet nn, CBN_Matrix m);
CARBON_API CBN_NeuralNet carbon_nn_backprop(CBN_NeuralNet nn, CBN_Matrix m);
CARBON_API void carbon_nn_learn(CBN_NeuralNet nn, CBN_NeuralNet g, f32 lr);
CARBON_API void carbon_nn_fit(CBN_NeuralNet nn, usz iters, CBN_Matrix train, f32 lr);
CARBON_API void carbon_nn_print(CBN_NeuralNet nn, const char *name);

/*
**  $$=======================$$
**  ||       Windowing       ||
**  $$=======================$$
*/
CARBON_API void carbon_win_open(u16 width, u16 height, const char *title);
CARBON_API void carbon_win_close(void);
CARBON_API void carbon_win_set_max_fps(u32 fps);

/**
 * @brief Set the window icon to a specific image object.
 *
 * The windowing module holds the ownership of the image object passed through.
 * This means you mustn't worry about managing its memory, it's the window's
 * responsability when it closes.
 *
 * @param img The image object of the icon.
 */
CARBON_API void carbon_win_set_icon(CBN_Image img);

CARBON_API f64 carbon_win_get_deltatime(void);
CARBON_API void carbon_win_update(CBN_DrawCanvas dc);
CARBON_API u8 carbon_win_shouldclose(void);

/*
**  $$==========================$$
**  ||       Test Manager       ||
**  $$==========================$$
*/
#define CARBON_RUN_ALL carbon_test_manager_run
#define CARBON_REGISTER_TEST(f) carbon_test_manager_register(f, CARBON_EXPAND_AND_QUOTE(f), __FILE__)
#define CARBON_TEST_FQN(ctx_name, unit_name) ctx_name ## _test_ ## unit_name
#define CARBON_TEST_DECL(ctx_name, unit_name) static u8 CARBON_TEST_FQN(ctx_name, unit_name)(void)
#define CARBON_TEST_REG_DECL(ctx_name, unit_name)                       \
  __attribute__((constructor)) static void CARBON_EXPAND_AND_PASTE(register_, CARBON_TEST_FQN(ctx_name, unit_name))(void)

#define CARBON_TEST(ctx_name, unit_name)                        \
  CARBON_TEST_DECL(ctx_name, unit_name);                        \
  CARBON_TEST_REG_DECL(ctx_name, unit_name) {                   \
    CARBON_REGISTER_TEST(CARBON_TEST_FQN(ctx_name, unit_name)); \
  }                                                             \
  CARBON_TEST_DECL(ctx_name, unit_name)

typedef u8 (*CBN_TestFunc)(void);

typedef struct {
  CBN_TestFunc f;
  char *name;
  char *filename;
} CBN_Test;

typedef struct {
  CBN_Test *tests;
  usz n;
  CBN_StrList files;
} CBN_Suite;

typedef struct {
  u8 no_output;
  char *output;
} CBN_CmdArgs;

CARBON_API void carbon_test_manager_argparse(int argc, char * const *argv);
CARBON_API void carbon_test_manager_rebuild(const char *src_file, char * const *host_argv);
CARBON_API CBN_Suite carbon_test_manager_spawn(void);
CARBON_API CBN_Test *carbon_test_manager_alloc(CBN_Suite *s);
CARBON_API void carbon_test_manager_cleanup(CBN_Suite *s);
CARBON_API void carbon_test_manager_register_s(CBN_Suite *s, CBN_TestFunc test_func, const char *name, const char *filename);
CARBON_API void carbon_test_manager_register(CBN_TestFunc test_func, const char *name, const char *filename);
CARBON_API u8 carbon_test_manager_run_s(CBN_Suite *s);
CARBON_API u8 carbon_test_manager_run(void);

/*
**  $$===================$$
**  ||       JUnit       ||
**  $$===================$$
*/
#define CARBON_JUNIT_XML_OUT_FILENAME "carbon_results.xml"

typedef struct {
  char *name;
  u8 has_failed;
} CBN_JUnitTestcase;

CARBON_API void carbon_junit_output(const CBN_List junit_tcs, const char *out_filename, const usz failed, const f64 runtime);

/*
**  $$======================================$$
**  ||       Sparky Asset Pack (SKAP)       ||
**  $$======================================$$
*/
#define CARBON_SKAP_DECL_FILE_MAX_LINE_LEN 256
#define CARBON_SKAP_DECL_FILE_MAX_PATH_LEN 127
#define CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN 63

typedef enum {
  CARBON_SKAP_ASSET_TYPE_IMAGE,
  CARBON_SKAP_ASSET_TYPE_COUNT
} CBN_SKAP_AssetType;

typedef struct {
  CBN_SKAP_AssetType type;
  union {
    CBN_Image_Metadata as_img;
  };
} CBN_SKAP_AssetMetadata;

typedef struct {
  char name[CARBON_SKAP_DECL_FILE_MAX_LINE_LEN];
  usz blob_offset;
  usz blob_size;
  u32 checksum;
  CBN_SKAP_AssetMetadata metadata;
} CBN_SKAP_AssetIdx;

typedef struct {
  char path[CARBON_SKAP_DECL_FILE_MAX_PATH_LEN + 1];
  char type[CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN + 1];
  CBN_StrList assets;
} CBN_SKAP_AssetGroup;

typedef struct {
  char signature[4];
  u8 fmt_ver;
  u64 build_ver;
} CBN_SKAP_Header;

typedef struct {
  FILE *fd;
  usz blob_section_start_pos;
  CBN_SKAP_Header header;
  usz type_counters[CARBON_SKAP_ASSET_TYPE_COUNT];
  CBN_HashMap idxs[CARBON_SKAP_ASSET_TYPE_COUNT];
} CBN_SKAP_Handle;

/**
 * @brief Creates a SKAP from a declarations (DECL) file.
 *
 * A DECL file is a custom text format, which enables an ergonomic and convinient way of
 * listing and declaring all asset files wanted to package in a SKAP (as a shipping method).
 * Full format specification: <https://gist.github.com/iWas-Coder/bc6269afb2b191d1e78faf6a7ec52c54>.
 *
 * @param decl The filename of the DECL file in disk.
 * @param skap The filename that the resulting SKAP file should have in disk.
 * @return A boolean value indicating whether it created the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_create(const char *decl, const char *skap);

/**
 * @brief Opens a SKAP from disk.
 *
 * This procedure doesn't load the entire SKAP, just the indices and its metadata and global info.
 * To obtain any stored asset from it, it must be done so by requesting it via the `carbon_skap_lookup`
 * function call, providing all necessary info to be able to locate it (i.e. type and name).
 *
 * @param skap The filename of the SKAP in disk.
 * @param out_handle The handle info object of the opened SKAP (output argument pointer).
 * @return A boolean value indicating whether it opened the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_open(const char *skap, CBN_SKAP_Handle *out_handle);

/**
 * @brief Closes a previously opened SKAP.
 * @param handle The handle info object of the opened SKAP.
 */
CARBON_API void carbon_skap_close(CBN_SKAP_Handle *handle);

/**
 * @brief Prints out the metadata or global info of a SKAP.
 * @param handle The handle info object of the opened SKAP.
 */
CARBON_API void carbon_skap_print(const CBN_SKAP_Handle *handle);

/**
 * @brief Requests the retrieval or acquisition of an asset from a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @param asset_type The type of the requested asset.
 * @param asset_name The name of the requested asset.
 * @param out_blob The retrieved asset from the SKAP (output argument pointer).
 * @return A boolean value indicating whether it retrieved the requested asset from the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_lookup(const CBN_SKAP_Handle *handle, const CBN_SKAP_AssetType asset_type, const char *asset_name, void *out_blob);

/**
 * @brief Counts how many assets are stored in a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @return The number of assets stored in the opened SKAP.
 */
CARBON_API usz carbon_skap_count(const CBN_SKAP_Handle *handle);

/**
 * @brief Counts how many assets of a specific type are stored in a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @param type The asset type you're interested in.
 * @return The number of assets of the specified type stored in the opened SKAP.
 */
CARBON_API usz carbon_skap_count_of(const CBN_SKAP_Handle *handle, const CBN_SKAP_AssetType type);

/*
**  $$================================$$
**  ||       Namespaced Aliases       ||
**  $$================================$$
*/
#ifdef __cplusplus
namespace cbn {
  using Vec2 = CBN_Vec2;
  using Vec3 = CBN_Vec3;
  using Rect = CBN_Rect;
  template <typename T>
  using List = CBN_List_t<T>;
  using StrBuilder = CBN_StrBuilder;
  using StrView = CBN_StrView;
}
#endif

/*
**  $$=============================$$
**  ||       Implementations       ||
**  $$=============================$$
*/
#ifdef CARBON_IMPLEMENTATION
#include "src/carbon_version.c"
#include "src/carbon_assert.c"
#include "src/carbon_memory.c"
#include "src/carbon_coroutine.c"
#include "src/carbon_math.c"
#include "src/carbon_math.cc"
#include "src/carbon_crypto.c"
#include "src/carbon_log.c"
#include "src/carbon_time.c"
#include "src/carbon_clock.c"
#include "src/carbon_list.c"
#include "src/carbon_hashmap.c"
#include "src/carbon_string.c"
#include "src/carbon_strbuilder.c"
#include "src/carbon_strbuilder.cc"
#include "src/carbon_strview.c"
#include "src/carbon_strview.cc"
#include "src/carbon_strlist.c"
#include "src/carbon_strlist.cc"
#include "src/carbon_drawcanvas.c"
#include "src/carbon_fs.c"
#include "src/carbon_fs.cc"
#include "src/carbon_net.c"
#include "src/carbon_nn.c"
#ifdef CARBON_IMPLEMENTATION_WINDOWING
#include "src/carbon_win.c"
#endif  // CARBON_IMPLEMENTATION_WINDOWING
#include "src/carbon_test_manager.c"
#include "src/carbon_junit.c"
#include "src/carbon_skap.c"
#endif  // CARBON_IMPLEMENTATION

// Local Variables:
// mode: c++
// End:
