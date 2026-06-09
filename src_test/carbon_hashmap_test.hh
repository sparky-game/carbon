// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#undef TEST
#define TEST(x) CARBON_TEST(carbon_hashmap, x)

TEST(creation) {
  cbn::HashMap<const char *, i32> hm;
  carbon_should_be(0, hm.capacity);
  carbon_should_be(sizeof(typeof(hm)::key_type), hm.k_stride);
  carbon_should_be(sizeof(typeof(hm)::value_type), hm.v_stride);
  carbon_should_be(0, hm.size);
  carbon_should_be_p(0, hm.buckets);
  return true;
}

TEST(set_get) {
  cbn::HashMap<const char *, i32> hm;
  i32 i = 7;
  hm.Set("seven", i);
  carbon_should_be(1, hm.size);
  i32 x = 0, xx = 0;
  x = hm.Get("seven");
  carbon_should_be(i, x);
  xx = hm["seven"];
  carbon_should_be(i, xx);
  carbon_should_be(1, hm.size);
  return true;
}

TEST(set_get_multiple) {
  cbn::HashMap<const char *, i32> hm;
  i32 i = 7, j = 69, k = 420;
  hm.Set("seven", i);
  hm.Set("sixty nine", j);
  hm.Set("four hundred and twenty", k);
  carbon_should_be(3, hm.size);
  i32 x = 0, xx = 0, y = 0, yy = 0, z = 0, zz = 0;
  y = hm.Get("sixty nine");
  carbon_should_be(j, y);
  yy = hm["sixty nine"];
  carbon_should_be(j, yy);
  z = hm.Get("four hundred and twenty");
  carbon_should_be(k, z);
  zz = hm["four hundred and twenty"];
  carbon_should_be(k, zz);
  x = hm.Get("seven");
  carbon_should_be(i, x);
  xx = hm["seven"];
  carbon_should_be(i, xx);
  carbon_should_be(3, hm.size);
  return true;
}
