// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_strlist, name)

TEST(create_destroy) {
  CBN_StrList sl = carbon_strlist_create(false);
  carbon_should_be(0, sl.size);
  carbon_should_be(1, sl.capacity);
  carbon_should_not_be_p(0, sl.items);
  carbon_strlist_destroy(&sl);
  carbon_should_be(0, sl.size);
  carbon_should_be(0, sl.capacity);
  carbon_should_be_p(0, sl.items);
  return CARBON_OK;
}

TEST(push_pop_single) {
  CBN_StrList sl = carbon_strlist_create(false);
  carbon_strlist_push(&sl, "hello");
  carbon_should_be(1, sl.size);
  carbon_should_be_true(carbon_strlist_contains(&sl, "hello"));
  carbon_should_be_false(carbon_strlist_contains(&sl, "bye"));
  carbon_strlist_pop(&sl, "hello");
  carbon_should_be(0, sl.size);
  carbon_should_be_false(carbon_strlist_contains(&sl, "hello"));
  carbon_should_be_false(carbon_strlist_contains(&sl, "bye"));
  carbon_strlist_destroy(&sl);
  return CARBON_OK;
}

TEST(push_pop_multiple) {
  CBN_StrList sl = carbon_strlist_create(false);
  carbon_strlist_push(&sl, "hello");
  carbon_strlist_push(&sl, "bye");
  carbon_strlist_push(&sl, "world");
  carbon_strlist_push(&sl, "seaman");
  carbon_strlist_push(&sl, "sparky");
  carbon_strlist_push(&sl, "carbon");
  carbon_should_be(6, sl.size);
  carbon_strlist_pop(&sl, "seaman");
  carbon_strlist_pop(&sl, "hello");
  carbon_should_be(4, sl.size);
  carbon_strlist_destroy(&sl);
  return CARBON_OK;
}

TEST(split_cstr) {
  const char *s = "apple,banana,orange";
  CBN_StrList s_splitted = carbon_strlist_from_splitted_cstr(s, ",");
  carbon_should_be(3, s_splitted.size);
  carbon_should_be_s("apple", carbon_strlist_at(s_splitted, 0));
  carbon_should_be_s("banana", carbon_strlist_at(s_splitted, 1));
  carbon_should_be_s("orange", carbon_strlist_at(s_splitted, 2));
  carbon_strlist_destroy(&s_splitted);
  return CARBON_OK;
}

#undef TEST
