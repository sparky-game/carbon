// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_strlist, create_destroy) {
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

CARBON_TEST(carbon_strlist, push_pop_single) {
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

CARBON_TEST(carbon_strlist, push_pop_multiple) {
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
