// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_strlist, create_destroy) {
  CBN_StrList ul = carbon_strlist_create(false);
  carbon_should_be(0, ul.size);
  carbon_should_be(1, ul.capacity);
  carbon_should_not_be_p(0, ul.items);
  carbon_strlist_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_strlist, push_pop_single) {
  CBN_StrList ul = carbon_strlist_create(false);
  carbon_strlist_push(&ul, "hello");
  carbon_should_be(1, ul.size);
  carbon_should_be_true(carbon_strlist_contains(&ul, "hello"));
  carbon_should_be_false(carbon_strlist_contains(&ul, "bye"));
  carbon_strlist_pop(&ul, "hello");
  carbon_should_be(0, ul.size);
  carbon_should_be_false(carbon_strlist_contains(&ul, "hello"));
  carbon_should_be_false(carbon_strlist_contains(&ul, "bye"));
  carbon_strlist_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_strlist, push_pop_multiple) {
  CBN_StrList ul = carbon_strlist_create(false);
  carbon_strlist_push(&ul, "hello");
  carbon_strlist_push(&ul, "bye");
  carbon_strlist_push(&ul, "world");
  carbon_strlist_push(&ul, "seaman");
  carbon_strlist_push(&ul, "sparky");
  carbon_strlist_push(&ul, "carbon");
  carbon_should_be(6, ul.size);
  carbon_strlist_pop(&ul, "seaman");
  carbon_strlist_pop(&ul, "hello");
  carbon_should_be(4, ul.size);
  carbon_strlist_destroy(&ul);
  return CARBON_OK;
}
