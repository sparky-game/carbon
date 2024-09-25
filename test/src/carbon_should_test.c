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


#include <carbon.h>
#include <carbon_should_test.h>

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

void carbon_should_test_register(void) {
  CARBON_REGISTER_TEST(carbon_should_test_should_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_not_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_lt);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_le);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_gt);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_ge);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_p);
  CARBON_REGISTER_TEST(carbon_should_test_should_not_be_p);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_s);
  CARBON_REGISTER_TEST(carbon_should_test_should_not_be_s);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_true);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_false);
}
