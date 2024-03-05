#pragma once

#define carbon_should_be(expected, actual)                              \
  {                                                                     \
    if ((expected) != (actual)) {                                        \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d', expected '%d'\n",      \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return false;                                                     \
    }                                                                   \
  }

#define carbon_should_not_be(expected, actual)                          \
  {                                                                     \
    if ((expected) == (actual)) {                                        \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d == %d', expected not to\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   (actual),                                            \
                   (expected));                                         \
      return false;                                                     \
    }                                                                   \
  }

#define carbon_should_be_true(actual)                                   \
  {                                                                     \
    if ((actual) != true) {                                              \
      CARBON_ERROR("%s:%d :: FAILED -> got 'false', expected 'true'\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return false;                                                     \
    }                                                                   \
  }

#define carbon_should_be_false(actual)                                  \
  {                                                                     \
    if ((actual) != false) {                                             \
      CARBON_ERROR("%s:%d :: FAILED -> got 'true', expected 'false'\n", \
                   __FILE__,                                            \
                   __LINE__);                                           \
      return false;                                                     \
    }                                                                   \
  }
