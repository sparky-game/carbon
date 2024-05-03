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

static unsigned char carbon_should_test_should_be(void) {
  carbon_should_be(1, 1);
  return 1;
}

static unsigned char carbon_should_test_should_not_be(void) {
  carbon_should_not_be(2, 1);
  return 1;
}

static unsigned char carbon_should_test_should_be_true(void) {
  carbon_should_be_true(1 == 1);
  return 1;
}

static unsigned char carbon_should_test_should_be_false(void) {
  carbon_should_be_false(2 == 1);
  return 1;
}

void carbon_should_test_register(void) {
  CARBON_REGISTER_TEST(carbon_should_test_should_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_not_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_true);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_false);
}
