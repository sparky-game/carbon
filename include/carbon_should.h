#pragma once

#define carbon_should_be(expected, actual)                              \
  {                                                                     \
    if (expected != actual) {                                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d', expected '%d'\n",      \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   actual,                                              \
                   expected);                                           \
      return false;                                                     \
    }                                                                   \
  }

#define carbon_should_not_be(expected, actual)                          \
  {                                                                     \
    if (expected == actual) {                                            \
      CARBON_ERROR("%s:%d :: FAILED -> got '%d == %d', expected not to\n", \
                   __FILE__,                                            \
                   __LINE__,                                            \
                   actual,                                              \
                   expected);                                           \
      return false;                                                     \
    }                                                                   \
  }
