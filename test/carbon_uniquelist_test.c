// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_uniquelist, create_destroy) {
  UniqueList ul = carbon_uniquelist_create();
  carbon_should_be(0, ul.size);
  carbon_should_be(1, ul.capacity);
  carbon_should_not_be_p(0, ul.items);
  carbon_uniquelist_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_uniquelist, push_pop_single) {
  UniqueList ul = carbon_uniquelist_create();
  carbon_uniquelist_push(&ul, "hello");
  carbon_should_be(1, ul.size);
  carbon_should_be_true(carbon_uniquelist_contains(&ul, "hello"));
  carbon_should_be_false(carbon_uniquelist_contains(&ul, "bye"));
  carbon_uniquelist_pop(&ul, "hello");
  carbon_should_be(0, ul.size);
  carbon_should_be_false(carbon_uniquelist_contains(&ul, "hello"));
  carbon_should_be_false(carbon_uniquelist_contains(&ul, "bye"));
  carbon_uniquelist_destroy(&ul);
  return CARBON_OK;
}

CARBON_TEST(carbon_uniquelist, push_pop_multiple) {
  UniqueList ul = carbon_uniquelist_create();
  carbon_uniquelist_push(&ul, "hello");
  carbon_uniquelist_push(&ul, "bye");
  carbon_uniquelist_push(&ul, "world");
  carbon_uniquelist_push(&ul, "seaman");
  carbon_uniquelist_push(&ul, "sparky");
  carbon_uniquelist_push(&ul, "carbon");
  carbon_should_be(6, ul.size);
  carbon_uniquelist_pop(&ul, "seaman");
  carbon_uniquelist_pop(&ul, "hello");
  carbon_should_be(4, ul.size);
  carbon_uniquelist_destroy(&ul);
  return CARBON_OK;
}
