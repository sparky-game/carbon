// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_string, starts_with_substr) {
  const char *s = "Hello, World!";
  carbon_should_be_true(carbon_string_starts_with_substr(s, "Hello"));
  carbon_should_be_false(carbon_string_starts_with_substr(s, "Привет"));
  return CARBON_OK;
}

CARBON_TEST(carbon_string, ends_with_substr) {
  const char *s = "Hello, World!";
  carbon_should_be_true(carbon_string_ends_with_substr(s, "World!"));
  carbon_should_be_false(carbon_string_ends_with_substr(s, "Мир!"));
  return CARBON_OK;
}
