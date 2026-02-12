/*
  $$====================$$
  ||       Should       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_COMPARE(expr, msg, ...)                            \
  if ((expr)) {                                                   \
    carbon_ceprintln(CARBON_COLOR_RED, "%s:%d :: FAILED -> " msg, \
                     __FILE__, __LINE__, ##__VA_ARGS__);          \
    return false;                                                 \
  }

#define CARBON_COMPARE_OP(T, expected, actual, op, msg, ...)          \
  CARBON_COMPARE((T) (expected) op (T) (actual), msg, ##__VA_ARGS__)

#define carbon_should_be_true(actual) CARBON_COMPARE(!(actual), "got 'false', expected 'true'")

#define carbon_should_be_false(actual) CARBON_COMPARE(actual, "got 'true', expected 'false'")

#define carbon_should_be(expected, actual)            \
  CARBON_COMPARE_OP(i32, expected, actual, !=,        \
                    "got '%d', expected '%d'",        \
                    (i32) (actual), (i32) (expected))

#define carbon_should_be_u64(expected, actual)        \
  CARBON_COMPARE_OP(u64, expected, actual, !=,        \
                    "got '%llu', expected '%llu'",    \
                    (u64) (actual), (u64) (expected))

#define carbon_should_be_f(expected, actual)                      \
  CARBON_COMPARE(carbon_math_abs(expected - actual) > CARBON_EPS, \
                 "got '%f', expected '%f'",                       \
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

#define carbon_should_be_p(expected, actual)                \
  CARBON_COMPARE_OP(void *, expected, actual, !=,           \
                    "got '%p', expected '%p'",              \
                    (void *) (actual), (void *) (expected))

#define carbon_should_not_be_p(expected, actual)            \
  CARBON_COMPARE_OP(void *, expected, actual, ==,           \
                    "got '%p == %p', expected not to",      \
                    (void *) (actual), (void *) (expected))

#define carbon_should_be_s(expected, actual)                            \
  CARBON_COMPARE(carbon_string_cmp((char *) (expected), (char *) (actual)), \
                 "got '%s', expected '%s'",                             \
                 (char *) (actual), (char *) (expected))

#define carbon_should_not_be_s(expected, actual)                        \
  CARBON_COMPARE(!carbon_string_cmp((char *) (expected), (char *) (actual)), \
                 "got '%s', expected '%s'",                             \
                 (char *) (actual), (char *) (expected))
