// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_list, create_destroy) {
  CBN_List l = carbon_list_create(sizeof(i32));
  carbon_should_be(1, l.capacity);
  carbon_should_be(sizeof(i32), l.stride);
  carbon_should_be(0, l.size);
  carbon_should_not_be_p(0, l.items);
  carbon_list_destroy(&l);
  carbon_should_be(0, l.capacity);
  carbon_should_be(0, l.stride);
  carbon_should_be(0, l.size);
  carbon_should_be_p(0, l.items);
  return CARBON_OK;
}

CARBON_TEST(carbon_list, push_element) {
  CBN_List l = carbon_list_create(sizeof(i32));
  i32 i = 7;
  carbon_list_push(&l, &i);
  carbon_should_be(1, l.size);
  carbon_should_be(i, carbon_list_at(i32, l, 0));
  carbon_list_destroy(&l);
  return CARBON_OK;
}

CARBON_TEST(carbon_list, pop_element) {
  CBN_List l = carbon_list_create(sizeof(i32));
  i32 x = 1, i = 7, j = 0;
  carbon_list_push(&l, &x);
  carbon_list_push(&l, &i);
  carbon_should_be(2, l.size);
  carbon_list_pop(&l, &j);
  carbon_should_be(i, j);
  carbon_should_be(1, l.size);
  carbon_list_destroy(&l);
  return CARBON_OK;
}

CARBON_TEST(carbon_list, find_element) {
  CBN_List l = carbon_list_create(sizeof(i32));
  i32 i = 1, j = 7, k = 3;
  carbon_list_push(&l, &i);
  carbon_list_push(&l, &j);
  carbon_should_be(2, l.size);
  carbon_should_be(0, carbon_list_find(&l, &i));
  carbon_should_be(1, carbon_list_find(&l, &j));
  carbon_should_be(-1, carbon_list_find(&l, &k));
  carbon_list_destroy(&l);
  return CARBON_OK;
}
