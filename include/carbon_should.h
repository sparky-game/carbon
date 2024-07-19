/*
 * BSD Carbon --- A simple C/C++ unit testing framework
 * Copyright (C) 2024 Wasym A. Alonso
 *
 * This file is part of Carbon.
 *
 * Carbon is free software: you can redistribute it and/or modify
 * it under the terms of the BSD 3-Clause "New" or "Revised" License
 * as published by The Regents of the University of California.
 *
 * Carbon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * BSD 3-Clause "New" or "Revised" License for more details.
 *
 * You should have received a copy of the BSD 3-Clause "New" or
 * "Revised" License along with Carbon.
 * If not, see <https://opensource.org/license/BSD-3-Clause>.
 */


#pragma once

#include <string.h>

#define carbon_should_be(expected, actual)                              \
  {                                                                     \
    if ((int) (expected) != (int) (actual)) {                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d', expected '%d'\n",      \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_not_be(expected, actual)                          \
  {                                                                     \
    if ((int) (expected) == (int) (actual)) {                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d == %d', expected not to\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_lt(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) <= (int) (actual)) {                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d >= %d', expected '%d < %d'\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_le(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) < (int) (actual)) {                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d > %d', expected '%d <= %d'\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_gt(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) >= (int) (actual)) {                           \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d <= %d', expected '%d > %d'\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_ge(expected, actual)                           \
  {                                                                     \
    if ((int) (expected) > (int) (actual)) {                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d < %d', expected '%d >= %d'\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (int) (actual),                                      \
                   (int) (expected),                                    \
                   (int) (actual),                                      \
                   (int) (expected));                                   \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_p(expected, actual)                            \
  {                                                                     \
    if ((void *) (expected) != (void *) (actual)) {                     \
      CARBON_ERROR("%s:%d :: FAILED -> got '%p', expected '%p'\n",      \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (void *) (actual),                                   \
                   (void *) (expected));                                \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_not_be_p(expected, actual)                        \
  {                                                                     \
    if ((void *) (expected) == (void *) (actual)) {                     \
      CARBON_ERROR("%s:%d :: FAILED -> got '%p == %p', expected not to\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (void *) (actual),                                   \
                   (void *) (expected));                                \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_s(expected, actual)                            \
  {                                                                     \
    if (strcmp((expected), (actual))) {                                 \
      CARBON_ERROR("%s:%d :: FAILED -> got '%s', expected '%s'\n",      \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_not_be_s(expected, actual)                        \
  {                                                                     \
    if (!strcmp((expected), (actual))) {                                \
      CARBON_ERROR("%s:%d :: FAILED -> got '%s == %s', expected not to\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_true(actual)                                   \
  {                                                                     \
    if (!(actual)) {                                                    \
      CARBON_ERROR("%s:%d :: FAILED -> got 'false', expected 'true'\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return 0;                                                         \
    }                                                                   \
  }

#define carbon_should_be_false(actual)                                  \
  {                                                                     \
    if ((actual)) {                                                     \
      CARBON_ERROR("%s:%d :: FAILED -> got 'true', expected 'false'\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return 0;                                                         \
    }                                                                   \
  }
