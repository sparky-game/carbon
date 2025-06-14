// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_hashmap, name)

TEST(create_destroy) {
  CBN_HashMap hm = carbon_hashmap_create(4, sizeof(i32));
  carbon_should_be(4, hm.capacity);
  carbon_should_be(sizeof(i32), hm.stride);
  carbon_should_not_be_p(0, hm.items);
  carbon_hashmap_destroy(&hm);
  carbon_should_be(0, hm.capacity);
  carbon_should_be(0, hm.stride);
  carbon_should_be_p(0, hm.items);
  return CARBON_OK;
}

TEST(set_get) {
  CBN_HashMap hm = carbon_hashmap_create(4, sizeof(i32));
  i32 i = 7, j = 0;
  carbon_hashmap_set(&hm, "seven", &i);
  carbon_hashmap_get(&hm, "seven", &j);
  carbon_should_be(i, j);
  carbon_hashmap_destroy(&hm);
  return CARBON_OK;
}

#undef TEST
