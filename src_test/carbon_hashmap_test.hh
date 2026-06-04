// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#undef TEST
#define TEST(x) CARBON_TEST(carbon_hashmap, x)

TEST(creation) {
  cbn::HashMap<i32> hm;
  carbon_should_be(0, hm.capacity);
  carbon_should_be(sizeof(typeof(hm)::value_type), hm.stride);
  carbon_should_be(0, hm.size);
  carbon_should_be_p(0, hm.buckets);
  return true;
}

TEST(set_get) {
  cbn::HashMap<i32> hm;
  i32 i = 7, j = 0;
  hm.Set("seven", i);
  j = *hm.Get("seven");
  carbon_should_be(i, j);
  return true;
}

TEST(set_get_multiple) {
  cbn::HashMap<i32> hm;
  i32 i = 7, x = 0, j = 69, y = 0, k = 420, z = 0;
  hm.Set("seven", i);
  hm.Set("sixty nine", j);
  hm.Set("four hundred and twenty", k);
  carbon_should_be(3, hm.size);
  y = *hm.Get("sixty nine");
  carbon_should_be(j, y);
  z = *hm.Get("four hundred and twenty");
  carbon_should_be(k, z);
  x = *hm.Get("seven");
  carbon_should_be(i, x);
  carbon_should_be(3, hm.size);
  return true;
}
