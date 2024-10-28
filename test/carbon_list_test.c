// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_list, create_destroy) {
  CBN_List ul = carbon_list_create();
  carbon_should_be(0, ul.size);
  carbon_should_be(1, ul.capacity);
  carbon_should_not_be_p(0, ul.items);
  carbon_list_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_list, push_pop_single) {
  CBN_List ul = carbon_list_create();
  carbon_list_push(&ul, "hello");
  carbon_should_be(1, ul.size);
  carbon_should_be_true(carbon_list_contains(&ul, "hello"));
  carbon_should_be_false(carbon_list_contains(&ul, "bye"));
  carbon_list_pop(&ul, "hello");
  carbon_should_be(0, ul.size);
  carbon_should_be_false(carbon_list_contains(&ul, "hello"));
  carbon_should_be_false(carbon_list_contains(&ul, "bye"));
  carbon_list_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_list, push_pop_multiple) {
  CBN_List ul = carbon_list_create();
  carbon_list_push(&ul, "hello");
  carbon_list_push(&ul, "bye");
  carbon_list_push(&ul, "world");
  carbon_list_push(&ul, "seaman");
  carbon_list_push(&ul, "sparky");
  carbon_list_push(&ul, "carbon");
  carbon_should_be(6, ul.size);
  carbon_list_pop(&ul, "seaman");
  carbon_list_pop(&ul, "hello");
  carbon_should_be(4, ul.size);
  carbon_list_destroy(&ul);
  return CARBON_OK;
}
