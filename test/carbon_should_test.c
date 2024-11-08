// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_should, should_be) {
  carbon_should_be(1, 1);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_not_be) {
  carbon_should_not_be(2, 1);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_lt) {
  carbon_should_be_lt(2, 1);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_le) {
  carbon_should_be_le(1, 1);
  carbon_should_be_le(2, 1);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_gt) {
  carbon_should_be_gt(1, 2);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_ge) {
  carbon_should_be_ge(1, 1);
  carbon_should_be_ge(1, 2);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_p) {
  int i = 7;
  int *i_addr = &i;
  carbon_should_be_p(i_addr, &i);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_not_be_p) {
  int i = 7, j = 9;
  carbon_should_not_be_p(0, &i);
  carbon_should_not_be_p(0, &j);
  carbon_should_not_be_p(&i, &j);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_s) {
  const char *s = "Hello, World!";
  carbon_should_be_s("Hello, World!", s);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_not_be_s) {
  const char *s = "Hello, World!";
  carbon_should_not_be_s("Hello, Seaman!", s);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_true) {
  carbon_should_be_true(1 == 1);
  return CARBON_OK;
}

CARBON_TEST(carbon_should, should_be_false) {
  carbon_should_be_false(2 == 1);
  return CARBON_OK;
}
