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
**  ||    *****=-----      ------=++++             Licensed under BSD-3-Clause         ||
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

// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_NAME "SPARKY Carbon"

#define CARBON_VERSION_MAJOR 0
#define CARBON_VERSION_MINOR 7
// TODO: handle patch version number as well
// #define CARBON_VERSION_PATCH
#define CARBON_VERSION_EXTRA "alpha"

#define CARBON_VERSION                                  \
  "v" CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)     \
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)     \
  "-" CARBON_VERSION_EXTRA

#if defined(__amd64__) || defined(_M_AMD64)
#define CARBON_CPU_ARCH "amd64"
#endif

#if defined(__linux__)
#define CARBON_TARGET_OS "linux"
#elif defined(_WIN32)
#define CARBON_TARGET_OS "windows"
#endif

#if defined(__clang__)
#define CARBON_COMPILER "clang"
#elif defined(__GNUC__)
#define CARBON_COMPILER "gcc"
#elif defined(_WIN32) && defined(_MSC_VER)
#define CARBON_COMPILER "cl.exe"
#else
#define CARBON_COMPILER "cc"
#endif

/*
**  $$======================$$
**  ||       Settings       ||
**  $$======================$$
*/
#define CARBON_MALLOC(sz)        malloc(sz)
#define CARBON_REALLOC(p, newsz) realloc(p, newsz)
#define CARBON_CALLOC(n, sz)     calloc(n, sz)
#define CARBON_FREE(p)           free(p)
#define CARBON_ASSERT(x)         assert(x)

/*
**  $$==========================$$
**  ||       Dependencies       ||
**  $$==========================$$
*/
#include <time.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#else
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
#define CARBON_NOTUSED(v) (void)(v)
#if defined(__GNUC__) || defined(__clang__)
#define CARBON_STATIC_ASSERT _Static_assert
#else
#define CARBON_STATIC_ASSERT static_assert
#endif
#ifdef __cplusplus
#define CARBON_API extern "C"
#else
#define CARBON_API extern
#endif

/*
**  $$===================$$
**  ||       Types       ||
**  $$===================$$
*/
typedef size_t usz;
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
**  $$==================$$
**  ||       Math       ||
**  $$==================$$
*/
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
#define CARBON_MIN(x, y) (x < y ? x : y)
#define CARBON_MAX(x, y) (x > y ? x : y)
#define CARBON_CLAMP(x, min, max) ((x <= min) ? min : (x >= max) ? max : x)
#define CARBON_SIGN(x) (!x ? 0 : x < 0 ? -1 : 1)
#define CARBON_STEP(edge, x) (x < edge ? 0 : 1)
#define CARBON_SWAP(T, x, y) do { T z = x; x = y; y = z; } while (0)
#define CARBON_LERP(a, b, t) (a + (b - a) * t)

CARBON_API f32 carbon_math_smoothstep(f32 a, f32 b, f32 t);

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
#define CARBON_WARNING(msg, ...) CARBON_INFO_COLOR(CARBON_COLOR_MAGENTA, "[?] " msg, ##__VA_ARGS__)
#define CARBON_ERROR_RAW(msg, ...) fprintf(stderr, msg, ##__VA_ARGS__)
#define CARBON_ERROR_PREFIX(prefix, msg, ...) CARBON_ERROR_RAW(CARBON_COLOR_RED prefix "" msg CARBON_COLOR_RESET "\n", ##__VA_ARGS__)
#define CARBON_ERROR_ASS(msg, ...) CARBON_ERROR_PREFIX("", msg, ##__VA_ARGS__)
#define CARBON_ERROR(msg, ...) CARBON_ERROR_PREFIX("[!] ", msg, ##__VA_ARGS__)

/*
**  $$========================$$
**  ||       Assertions       ||
**  $$========================$$
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

#define carbon_should_be_s(expected, actual)                    \
  CARBON_COMPARE(carbon_string_cmp((expected), (actual)),       \
                 "got '%s', expected '%s'",                     \
                 (actual), (expected))

#define carbon_should_not_be_s(expected, actual)                \
  CARBON_COMPARE(!carbon_string_cmp((expected), (actual)),      \
                 "got '%s', expected '%s'",                     \
                 (actual), (expected))

/*
**  $$==================$$
**  ||       Time       ||
**  $$==================$$
*/
CARBON_API f64 carbon_time_get(void);
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
**  $$========================$$
**  ||       Filesystem       ||
**  $$========================$$
*/
CARBON_API u8 carbon_fs_exists(const char *file);
CARBON_API u8 carbon_fs_is_regular_file(const char *file);
CARBON_API u8 carbon_fs_is_directory(const char *file);
CARBON_API u8 carbon_fs_rename(const char *old, const char *new);
CARBON_API i32 carbon_fs_mtime(const char *file);
CARBON_API void carbon_fs_copy(const char *from, const char *to, u8 recursive);
CARBON_API u8 carbon_fs_remove(const char *file);
CARBON_API u8 carbon_fs_remove_all(const char *file);
CARBON_API u8 carbon_fs_change_directory(const char *path);
CARBON_API u8 carbon_fs_create_directory(const char *path);
CARBON_API u8 carbon_fs_create_directories(const char *path);
CARBON_API char *carbon_fs_get_bin_directory(void);
CARBON_API char **carbon_fs_pattern_match(const char *pattern, usz *out_count);

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
**  $$=====================$$
**  ||       StrList       ||
**  $$=====================$$
*/
typedef struct {
  char **items;
  usz size;
  usz capacity;
  u8 unique;
} CBN_StrList;

CARBON_API CBN_StrList carbon_strlist_create(u8 unique);
CARBON_API void carbon_strlist_destroy(CBN_StrList *sl);
CARBON_API void carbon_strlist_push(CBN_StrList *sl, const char *s);
CARBON_API void carbon_strlist_pop(CBN_StrList *sl, const char *s);
CARBON_API u8 carbon_strlist_contains(CBN_StrList *sl, const char *s);

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
CARBON_API void carbon_test_manager_register_s(CBN_Suite *s, CBN_TestFunc test_func, char *name, char *filename);
CARBON_API void carbon_test_manager_register(CBN_TestFunc test_func, char *name, char *filename);
CARBON_API u8 carbon_test_manager_run_s(CBN_Suite *s);
CARBON_API u8 carbon_test_manager_run(void);

/*
**  $$===================$$
**  ||       JUnit       ||
**  $$===================$$
*/
#define CARBON_JUNIT_XML_OUT_FILENAME      "carbon_results.xml"
#define CARBON_JUNIT_TESTCASE_NAME_MAX_LEN 512

typedef struct {
  f64 time;
  usz tests;
  usz failures;
} CBN_JUnitTestsuite;

typedef struct {
  char name[CARBON_JUNIT_TESTCASE_NAME_MAX_LEN];
  u8 has_failed;
} CBN_JUnitTestcase;

CARBON_API void carbon_junit_output(CBN_JUnitTestsuite *junit_ts, CBN_JUnitTestcase *junit_tcs, const char *out_filename);

/*
**  $$=============================$$
**  ||       Implementations       ||
**  $$=============================$$
*/
#ifdef CARBON_IMPLEMENTATION
#include "src/carbon_math.c"
#include "src/carbon_crypto.c"
#include "src/carbon_time.c"
#include "src/carbon_clock.c"
#include "src/carbon_fs.c"
#include "src/carbon_hashmap.c"
#include "src/carbon_string.c"
#include "src/carbon_strlist.c"
#include "src/carbon_test_manager.c"
#include "src/carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION
