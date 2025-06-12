// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static void inc_int(int *x) {
  ++(*x);
}

// Expected to pass
CARBON_TEST(testing_suite_example, inc_int_ok) {
  int a = 1, b = 0;
  carbon_should_not_be(a, b);
  inc_int(&b);
  carbon_should_be(a, b);
  return CARBON_OK;
}

// Expected to fail
CARBON_TEST(testing_suite_example, inc_int_ko) {
  int a = 1, b = 0;
  carbon_should_not_be(a, b);
  inc_int(&b);
  carbon_should_not_be(a, b);
  return CARBON_OK;
}

int main(void) {
  return CARBON_RUN_ALL();
}
