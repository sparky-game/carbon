// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#undef TEST
#define TEST(x) CARBON_TEST(carbon_string, x)

TEST(starts_with) {
  const char *s = "Hello, World!";
  carbon_should_be_true(cbn::str::StartsWith(s, "Hello"));
  carbon_should_be_false(cbn::str::StartsWith(s, "Привет"));
  return true;
}

TEST(ends_with) {
  const char *s = "Hello, World!";
  carbon_should_be_true(cbn::str::EndsWith(s, "World!"));
  carbon_should_be_false(cbn::str::EndsWith(s, "Мир!"));
  return true;
}

TEST(to_upper) {
  carbon_should_be('H', cbn::str::ToUpper('h'));
  carbon_should_be('W', cbn::str::ToUpper('w'));
  carbon_should_be(69, cbn::str::ToUpper(69));
  carbon_should_be('7', cbn::str::ToUpper('7'));
  return true;
}

TEST(lev_dist) {
  carbon_should_be(2, cbn::str::LevDist("add", "daddy"));
  carbon_should_be(2, cbn::str::LevDist("daddy", "add"));
  return true;
}
