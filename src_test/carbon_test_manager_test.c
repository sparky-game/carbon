// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static u8 test_dummy(void) {
  carbon_should_be_true(1 == 1);
  return CARBON_OK;
}

#define TEST(name) CARBON_TEST(carbon_test_manager, name)

TEST(suite_creation) {
  CBN_Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return CARBON_OK;
}

TEST(registration) {
  CBN_Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);

  carbon_test_manager_register_s(&s, test_dummy, "test_dummy", __FILE__);
  carbon_should_not_be_p(0, s.tests);
  carbon_should_be(1, s.n);
  carbon_should_be_s("test_dummy", s.tests[0].name);

  carbon_test_manager_cleanup(&s);
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return CARBON_OK;
}

#undef TEST
