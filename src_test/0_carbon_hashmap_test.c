// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_hashmap, name)

TEST(create_destroy) {
  CBN_HashMap hm = carbon_hashmap_create(4, sizeof(i32));
  carbon_should_be(4, hm.capacity);
  carbon_should_be(sizeof(i32), hm.stride);
  carbon_should_not_be_p(0, hm.buckets);
  carbon_hashmap_destroy(&hm);
  carbon_should_be(0, hm.capacity);
  carbon_should_be(0, hm.stride);
  carbon_should_be_p(0, hm.buckets);
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

TEST(set_get_multiple) {
  CBN_HashMap hm = carbon_hashmap_create(4, sizeof(i32));
  i32 i = 7, x = 0, j = 69, y = 0, k = 420, z = 0;
  carbon_hashmap_set(&hm, "seven", &i);
  carbon_hashmap_set(&hm, "sixty nine", &j);
  carbon_hashmap_set(&hm, "four hundred and twenty", &k);
  carbon_hashmap_get(&hm, "sixty nine", &y);
  carbon_should_be(j, y);
  carbon_hashmap_get(&hm, "four hundred and twenty", &z);
  carbon_should_be(k, z);
  carbon_hashmap_get(&hm, "seven", &x);
  carbon_should_be(i, x);
  carbon_hashmap_destroy(&hm);
  return CARBON_OK;
}

#undef TEST
