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
#include <carbon_test_manager_test.h>

static unsigned char test_dummy(void) {
  carbon_should_be_true(1 == 1);
  return 1;
}

static unsigned char carbon_test_manager_test_suite_creation(void) {
  Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return 1;
}

static unsigned char carbon_test_manager_test_registration(void) {
  Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);

  carbon_test_manager_register_s(&s, test_dummy, "test_dummy");
  carbon_should_not_be_p(0, s.tests);
  carbon_should_be(1, s.n);
  carbon_should_be_s("test_dummy", s.tests[0].name);

  carbon_test_manager_cleanup_s(&s);
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return 1;
}

void carbon_test_manager_test_register(void) {
  CARBON_REGISTER_TEST(carbon_test_manager_test_suite_creation);
  CARBON_REGISTER_TEST(carbon_test_manager_test_registration);
}
