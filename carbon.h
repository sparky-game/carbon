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
**  ||    ***+++     ***+++++   ****++                 The most lightweight            ||
**  ||    ***+++      #*++      ****++               C/C++ testing framework           ||
**  ||    ***+++                ****++               written in pure C (C99)           ||
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
#define CARBON_VERSION_MINOR 9
// TODO: handle patch version number as well
// #define CARBON_VERSION_PATCH
#define CARBON_VERSION_EXTRA "alpha"

/*
**  Available compile-time options:
**    - CARBON_IMPLEMENTATION -> Include function definitions in the translation unit
**                               that this macro has been defined. It's important that
**                               this macro is ONLY defined ONCE in the entire codebase
**                               of your program, to avoid multiple symbol definitions
**                               linker errors.
**    - CARBON_NO_TESTING -----> By default, Carbon behaves as a test suite entrypoint.
**                               If wanted to not redefine the `main` function, and
**                               just act as a standard program (using Carbon as a
**                               libc extension or add-on), define this macro EACH time
**                               you include this header file.
**    - CARBON_USE_WINDOWING --> Enable extra functionality to work with native windows
**                               and make GUI applications [WIP].
*/

/*
**  $$======================$$
**  ||       Settings       ||
**  $$======================$$
*/
#define CARBON_MALLOC(sz)        malloc(sz)
#define CARBON_REALLOC(p, newsz) realloc(p, newsz)
#define CARBON_CALLOC(n, sz)     calloc(n, sz)
#define CARBON_FREE(p)           free(p)

/*
**  $$==========================$$
**  ||       Dependencies       ||
**  $$==========================$$
*/
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
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <direct.h>
#include <windows.h>
#elif defined(__APPLE__)
#include <glob.h>
#include <mach-o/dyld.h>
#else
#include <glob.h>
#include <sys/wait.h>
#endif

/*
**  $$=========================$$
**  ||       Definitions       ||
**  $$=========================$$
*/
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
#define CARBON_NOTUSED(x) (void)(x)
#define CARBON_ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[0]))

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

#define CARBON_NAME "SPARKY Carbon"

#define CARBON_VERSION                                  \
  "v" CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)     \
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)     \
  "-" CARBON_VERSION_EXTRA

#if defined(__amd64__) || defined(_M_AMD64)
#define CARBON_CPU_ARCH "amd64"
#else
#error CPU architecture is not supported
#endif

#if defined(__linux__)
#define CARBON_TARGET_OS "linux"
#elif defined(__APPLE__)
#define CARBON_TARGET_OS "macos"
#elif defined(_WIN32)
#define CARBON_TARGET_OS "windows"
#else
#error Target OS is not supported
#endif

#if defined(__clang__)
#define CARBON_C_COMPILER "clang"
#define CARBON_CXX_COMPILER "clang++"
#elif defined(__GNUC__)
#define CARBON_C_COMPILER "gcc"
#define CARBON_CXX_COMPILER "g++"
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_C_COMPILER "cl.exe"
#define CARBON_CXX_COMPILER "cl.exe"
#else
#define CARBON_C_COMPILER "cc"
#define CARBON_CXX_COMPILER "c++"
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
typedef size_t usz;
typedef ssize_t isz;
typedef uintptr_t uptr;
typedef unsigned char u8;
CARBON_STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte");
typedef signed char i8;
CARBON_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte");
typedef unsigned short u16;
CARBON_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes");
typedef signed short i16;
CARBON_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes");
typedef unsigned int u32;
CARBON_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes");
typedef signed int i32;
CARBON_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes");
typedef unsigned long long u64;
CARBON_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes");
typedef signed long long i64;
CARBON_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes");
typedef float f32;
CARBON_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes");
typedef double f64;
CARBON_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes");

/*
**  $$========================$$
**  ||       Entrypoint       ||
**  $$========================$$
*/
#ifndef CARBON_NO_TESTING
#define main(...)                                       \
  main(int argc, char **argv) {                         \
    carbon_test_manager_argparse(argc, argv);           \
    carbon_test_manager_rebuild(argv[0], __FILE__);     \
    return carbon_main();                               \
  };                                                    \
  int carbon_main(__VA_ARGS__)
#endif  // CARBON_NO_TESTING

CARBON_API int carbon_main(void);

/*
**  $$====================$$
**  ||       Assert       ||
**  $$====================$$
*/
#define CARBON_ASSERT(x) ((void) ((x) || (carbon_assert_abort(#x, __FILE__, __LINE__, __func__), 0)))

CARBON_API void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func);

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
#define CARBON_MAX(x, y) (x > y ? x : y)
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

typedef union {
  f32 items[2];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
  };
} CBN_Vec2;

typedef union {
  f32 items[3];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
    union { f32 z, b, p, w; };
  };
} CBN_Vec3;

typedef struct {
  f32 x, y;
  f32 w, h;
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
CARBON_API int carbon_math_rand(void);
CARBON_API f32 carbon_math_randf(void);
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
CARBON_API f32 carbon_math_sigmoid(f32 x);
CARBON_API f32 carbon_math_tanh(f32 x);
CARBON_API f32 carbon_math_smoothstep(f32 a, f32 b, f32 t);
CARBON_API i8 carbon_math_cmp(f32 x, f32 y);
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
CARBON_API CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle);
CARBON_API CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot);
CARBON_API u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p);
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

#ifdef __cplusplus
CBN_Vec2 operator+(const CBN_Vec2 &u, const CBN_Vec2 &v);
CBN_Vec3 operator+(const CBN_Vec3 &u, const CBN_Vec3 &v);
CBN_Vec2 operator-(const CBN_Vec2 &u, const CBN_Vec2 &v);
CBN_Vec3 operator-(const CBN_Vec3 &u, const CBN_Vec3 &v);
f32 operator*(const CBN_Vec2 &u, const CBN_Vec2 &v);
f32 operator*(const CBN_Vec3 &u, const CBN_Vec3 &v);
#endif

/*
**  $$====================$$
**  ||       Crypto       ||
**  $$====================$$
*/
CARBON_API char *carbon_crypto_base64_encode(const u8 *in, const usz in_size, usz *out_size);
CARBON_API u8 *carbon_crypto_base64_decode(const u8 *in, usz *out_size);
CARBON_API u32 carbon_crypto_crc32(const u8 *in, const usz in_size);

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
#define CARBON_INFO_RAW(msg, ...) printf(msg, ##__VA_ARGS__)
#define CARBON_INFO_COLOR(color, msg, ...) CARBON_INFO_RAW(color msg CARBON_COLOR_RESET "\n", ##__VA_ARGS__)
#define CARBON_INFO(msg, ...) CARBON_INFO_RAW(msg "\n", ##__VA_ARGS__)
#define CARBON_WARNING(msg, ...) CARBON_INFO_COLOR(CARBON_COLOR_MAGENTA, "[?] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " (%s) :: " msg, __func__, ##__VA_ARGS__)
#define CARBON_ERROR_RAW(msg, ...) fprintf(stderr, msg, ##__VA_ARGS__)
#define CARBON_ERROR_PREFIX(prefix, msg, ...) CARBON_ERROR_RAW(CARBON_COLOR_RED prefix "" msg CARBON_COLOR_RESET "\n", ##__VA_ARGS__)
#define CARBON_ERROR_ASS(msg, ...) CARBON_ERROR_PREFIX("", msg, ##__VA_ARGS__)
#define CARBON_ERROR(msg, ...) CARBON_ERROR_PREFIX("[!] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " (%s) :: ", msg, __func__, ##__VA_ARGS__)

/*
**  $$====================$$
**  ||       Should       ||
**  $$====================$$
*/
#define CARBON_COMPARE(expr, msg, ...)                          \
  if ((expr)) {                                                 \
    CARBON_ERROR_ASS("%s:%d :: FAILED -> " msg,                 \
                     __FILE__, __LINE__, ##__VA_ARGS__);        \
    return CARBON_KO;                                           \
  }

#define CARBON_COMPARE_OP(T, expected, actual, op, msg, ...)            \
  CARBON_COMPARE((T) (expected) op (T) (actual), msg, ##__VA_ARGS__)

#define carbon_should_be_true(actual) CARBON_COMPARE(!(actual), "got 'false', expected 'true'")

#define carbon_should_be_false(actual) CARBON_COMPARE(actual, "got 'true', expected 'false'")

#define carbon_should_be(expected, actual)              \
  CARBON_COMPARE_OP(i32, expected, actual, !=,          \
                    "got '%d', expected '%d'",          \
                    (i32) (actual), (i32) (expected))

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
CARBON_API f64 carbon_time_get(void);
CARBON_API char *carbon_time_get_iso8601(void);
CARBON_API void carbon_time_sleep(u64 ms);

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
#define carbon_list_at(T, l, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (l).size && "List index out of bounds"), CARBON_ASSERT(sizeof(T) == (l).stride && "List type doesn't match"), ((T *) (l).items)[(i)])
#define carbon_list_foreach(T, l) for (struct { usz i; T var; } it = {0, carbon_list_at(T, l, 0)}; it.i < (l).size; ++it.i, it.i < (l).size ? it.var = carbon_list_at(T, l, it.i) : it.var)

typedef struct {
  void *items;
  usz capacity;
  usz stride;
  usz size;
} CBN_List;

CARBON_API CBN_List carbon_list_create(usz stride);
CARBON_API void carbon_list_destroy(CBN_List *l);
CARBON_API void carbon_list_push(CBN_List *l, void *value);
CARBON_API void carbon_list_pop(CBN_List *l, void *out_value);

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
CARBON_API void carbon_hashmap_get(CBN_HashMap *hm, const char *key, void *out_value);

/*
**  $$====================$$
**  ||       String       ||
**  $$====================$$
*/
CARBON_API i32 carbon_string_cmp(const char *s1, const char *s2);
CARBON_API char *carbon_string_dup(const char *s);
CARBON_API char *carbon_string_fmt(const char *s, ...);
CARBON_API void carbon_string_strip_substr(char *s, const char *sub);

/*
**  $$========================$$
**  ||       StrBuilder       ||
**  $$========================$$
*/
typedef struct {
  char *items;
  usz size;
  usz capacity;
} CBN_StrBuilder;

CARBON_API void carbon_strbuilder_add_buf(CBN_StrBuilder *sb, const char *data, usz size);
CARBON_API void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s);
CARBON_API void carbon_strbuilder_add_null(CBN_StrBuilder *sb);
CARBON_API void carbon_strbuilder_free(CBN_StrBuilder *sb);

/*
**  $$=====================$$
**  ||       StrView       ||
**  $$=====================$$
*/
typedef struct {
  const char *data;
  usz size;
} CBN_StrView;

CARBON_API CBN_StrView carbon_strview_from_buf(const char *data, usz size);
CARBON_API CBN_StrView carbon_strview_from_cstr(const char *s);
CARBON_API char *carbon_strview_to_cstr(CBN_StrView sv);
CARBON_API CBN_StrView carbon_strview_from_strbuilder(CBN_StrBuilder *sb);
CARBON_API CBN_StrView carbon_strview_trim_left(CBN_StrView sv);
CARBON_API CBN_StrView carbon_strview_trim_right(CBN_StrView sv);
CARBON_API CBN_StrView carbon_strview_trim_both(CBN_StrView sv);
CARBON_API CBN_StrView carbon_strview_chop(CBN_StrView *sv, char c);
CARBON_API u8 carbon_strview_are_equal(CBN_StrView x, CBN_StrView y);

/*
**  $$=====================$$
**  ||       StrList       ||
**  $$=====================$$
*/
#define carbon_strlist_at(sl, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (sl).size && "StrList index out of bounds"), ((sl).items)[(i)])
#define carbon_strlist_foreach(sl) for (struct { usz i; CBN_StrView sv; } it = {0, carbon_strview_from_cstr(carbon_strlist_at(sl, 0))}; it.i < (sl).size; ++it.i, it.i < (sl).size ? it.sv = carbon_strview_from_cstr(carbon_strlist_at(sl, it.i)) : it.sv)

typedef struct {
  char **items;
  usz size;
  usz capacity;
  u8 unique;
} CBN_StrList;

CARBON_API CBN_StrList carbon_strlist_create(u8 unique);
CARBON_API CBN_StrList carbon_strlist_from_splitted_cstr(const char *s, const char *delim);
CARBON_API void carbon_strlist_destroy(CBN_StrList *sl);
CARBON_API void carbon_strlist_push(CBN_StrList *sl, const char *s);
CARBON_API void carbon_strlist_pop(CBN_StrList *sl, const char *s);
CARBON_API u8 carbon_strlist_contains(CBN_StrList *sl, const char *s);

/*
**  $$========================$$
**  ||       Filesystem       ||
**  $$========================$$
*/
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
CARBON_API char **carbon_fs_pattern_match(const char *pattern, usz *out_count);
CARBON_API u32 carbon_fs_get_file_size(const char *file);
CARBON_API u8 carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file);
CARBON_API CBN_List carbon_fs_read_img_from_file(const char *file);
CARBON_API f32 *carbon_fs_read_img_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs);
CARBON_API void carbon_fs_destroy_img(CBN_List *img);

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

/*
**  $$=======================$$
**  ||       Windowing       ||
**  $$=======================$$
*/
CARBON_API void carbon_win_open(u16 width, u16 height, const char *title);
CARBON_API void carbon_win_close(void);
CARBON_API void carbon_win_update(CBN_DrawCanvas dc);
CARBON_API u8 carbon_win_shouldclose(void);

/*
**  $$=========================$$
**  ||       TestManager       ||
**  $$=========================$$
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

CARBON_API void carbon_test_manager_argparse(int argc, char **argv);
CARBON_API void carbon_test_manager_rebuild(const char *bin_file, const char *src_file);
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
**  $$=============================$$
**  ||       Implementations       ||
**  $$=============================$$
*/
#ifdef CARBON_IMPLEMENTATION
#include "src/carbon_assert.c"
#include "src/carbon_math.c"
#include "src/carbon_math_ops.cc"
#include "src/carbon_crypto.c"
#include "src/carbon_time.c"
#include "src/carbon_clock.c"
#include "src/carbon_list.c"
#include "src/carbon_hashmap.c"
#include "src/carbon_string.c"
#include "src/carbon_strbuilder.c"
#include "src/carbon_strview.c"
#include "src/carbon_strlist.c"
#include "src/carbon_fs.c"
#include "src/carbon_nn.c"
#include "src/carbon_drawcanvas.c"
#ifdef CARBON_USE_WINDOWING
#include "src/carbon_win_xlib.c"
#endif  // CARBON_USE_WINDOWING
#include "src/carbon_test_manager.c"
#include "src/carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION

// Local Variables:
// mode: c++
// End:
