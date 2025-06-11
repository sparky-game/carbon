// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_list, name)

TEST(create_destroy) {
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

TEST(push_element) {
  CBN_List l = carbon_list_create(sizeof(i32));
  i32 i = 7;
  carbon_list_push(&l, &i);
  carbon_should_be(1, l.size);
  carbon_should_be(i, carbon_list_at(i32, l, 0));
  carbon_list_destroy(&l);
  return CARBON_OK;
}

TEST(pop_element) {
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

TEST(find_element) {
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

TEST(remove_element) {
  CBN_List l = carbon_list_create(sizeof(i32));
  i32 i = 1, j = 7, k = 3;
  carbon_list_push(&l, &i);
  carbon_list_push(&l, &j);
  carbon_list_push(&l, &k);
  carbon_should_be(3, l.size);
  carbon_should_be(i, carbon_list_at(i32, l, 0));
  carbon_should_be(j, carbon_list_at(i32, l, 1));
  carbon_should_be(k, carbon_list_at(i32, l, 2));
  carbon_list_remove(&l, 0);
  carbon_should_be(2, l.size);
  carbon_should_be(j, carbon_list_at(i32, l, 0));
  carbon_should_be(k, carbon_list_at(i32, l, 1));
  carbon_list_remove(&l, 1);
  carbon_should_be(1, l.size);
  carbon_should_be(j, carbon_list_at(i32, l, 0));
  carbon_list_destroy(&l);
  return CARBON_OK;
}

#undef TEST
