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

#ifdef __linux__
#define CARBON_TARGET_OS "linux"
#endif

/*
**  $$======================$$
**  ||       Settings       ||
**  $$======================$$
*/
#define CARBON_MALLOC(sz)        malloc(sz)
#define CARBON_REALLOC(p, newsz) realloc(p, newsz)
#define CARBON_FREE(p)           free(p)

/*
**  $$==========================$$
**  ||       Dependencies       ||
**  $$==========================$$
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif

/*
**  $$=========================$$
**  ||       Definitions       ||
**  $$=========================$$
*/
#define CARBON_OK 1
#define CARBON_KO 0
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
#define CARBON_API
#endif

/*
**  $$===================$$
**  ||       Types       ||
**  $$===================$$
*/
typedef size_t usz;
typedef uintptr_t uptr;
typedef unsigned char u8;
CARBON_STATIC_ASSERT(sizeof(u8)  == 1, "Expected u8 to be 1 byte");
typedef unsigned int u32;
CARBON_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes");
typedef signed int i32;
CARBON_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes");
typedef unsigned long long u64;
CARBON_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes");
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
#endif

CARBON_API int carbon_main(void);

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
} Clock;

CARBON_API Clock carbon_clock_start(void);
CARBON_API void carbon_clock_update(Clock *c);
CARBON_API void carbon_clock_stop(Clock *c);

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
**  ||       UniqueList       ||
**  $$========================$$
*/
typedef struct {
  char **items;
  usz size;
  usz capacity;
} UniqueList;

CARBON_API UniqueList carbon_uniquelist_create(void);
CARBON_API void carbon_uniquelist_destroy(UniqueList *ul);
CARBON_API void carbon_uniquelist_push(UniqueList *ul, const char *s);
CARBON_API void carbon_uniquelist_pop(UniqueList *ul, const char *s);
CARBON_API u8 carbon_uniquelist_contains(UniqueList *ul, const char *s);

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

typedef u8 (*TestFunc)(void);

typedef struct {
  TestFunc f;
  char *name;
  char *filename;
} Test;

typedef struct {
  Test *tests;
  size_t n;
  UniqueList files;
} Suite;

typedef struct {
  char *output;
} CmdArgs;

CARBON_API void carbon_test_manager_argparse(int argc, char **argv);
CARBON_API void carbon_test_manager_rebuild(const char *bin_file, const char *src_file);
CARBON_API Suite carbon_test_manager_spawn(void);
CARBON_API Test *carbon_test_manager_alloc(Suite *s);
CARBON_API void carbon_test_manager_cleanup(Suite *s);
CARBON_API void carbon_test_manager_register_s(Suite *s, TestFunc test_func, char *name, char *filename);
CARBON_API void carbon_test_manager_register(TestFunc test_func, char *name, char *filename);
CARBON_API u8 carbon_test_manager_run_s(Suite *s);
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
} JUnitTestsuite;

typedef struct {
  char name[CARBON_JUNIT_TESTCASE_NAME_MAX_LEN];
  u8 has_failed;
} JUnitTestcase;

CARBON_API void carbon_junit_output(JUnitTestsuite *junit_ts, JUnitTestcase *junit_tcs, const char *out_filename);

/*
**  $$=============================$$
**  ||       Implementations       ||
**  $$=============================$$
*/
#ifdef CARBON_IMPLEMENTATION
#include "src/carbon_time.c"
#include "src/carbon_clock.c"
#include "src/carbon_fs.c"
#include "src/carbon_string.c"
#include "src/carbon_uniquelist.c"
#include "src/carbon_test_manager.c"
#include "src/carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION
