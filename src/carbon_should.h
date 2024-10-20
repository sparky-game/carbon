// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <string.h>

#define carbon_should_be(expected, actual)                              \
  {                                                                     \
    if ((int) (expected) != (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d', expected '%d'" CARBON_COLOR_RESET " \n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_not_be(expected, actual)                          \
  {                                                                     \
    if ((int) (expected) == (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d == %d', expected not to" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_lt(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) <= (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d >= %d', expected '%d < %d'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_le(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) < (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d > %d', expected '%d <= %d'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_gt(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) >= (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d <= %d', expected '%d > %d'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_ge(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) > (int) (actual)) {                            \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%d < %d', expected '%d >= %d'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_p(expected, actual)                            \
  {                                                                     \
    if ((void *) (expected) != (void *) (actual)) {                      \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%p', expected '%p'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (void *) (actual),                                   \
                   (void *) (expected));                                \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_not_be_p(expected, actual)                        \
  {                                                                     \
    if ((void *) (expected) == (void *) (actual)) {                      \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%p == %p', expected not to" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (void *) (actual),                                   \
                   (void *) (expected));                                \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_s(expected, actual)                            \
  {                                                                     \
    if (strcmp((expected), (actual))) {                                 \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%s', expected '%s'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_not_be_s(expected, actual)                        \
  {                                                                     \
    if (!strcmp((expected), (actual))) {                                \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got '%s == %s', expected not to" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_true(actual)                                   \
  {                                                                     \
    if (!(actual)) {                                                    \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got 'false', expected 'true'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return CARBON_KO;                                                 \
    }                                                                   \
  }

#define carbon_should_be_false(actual)                                  \
  {                                                                     \
    if ((actual)) {                                                     \
      CARBON_ERROR(CARBON_COLOR_RED "%s:%d :: FAILED -> got 'true', expected 'false'" CARBON_COLOR_RESET "\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return CARBON_KO;                                                 \
    }                                                                   \
  }
