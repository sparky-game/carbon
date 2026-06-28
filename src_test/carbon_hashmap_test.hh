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
  i32 x = 0;
  x = hm["seven"];
  carbon_should_be(i, x);
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
  i32 x = 0, y = 0, z = 0;
  y = hm["sixty nine"];
  carbon_should_be(j, y);
  z = hm["four hundred and twenty"];
  carbon_should_be(k, z);
  x = hm["seven"];
  carbon_should_be(i, x);
  carbon_should_be(3, hm.size);
  return true;
}

TEST(set_get_nested) {
  cbn::HashMap<const char *, cbn::HashMap<const char *, i32>> hm;
  if (auto it = hm.Set("Dexterity", typeof(hm)::value_type{})) {
    it->Set("Acrobatics", 2);
    it->Set("Stealth", 4);
  }
  if (auto it = hm.Set("Strength", typeof(hm)::value_type{})) {
    it->Set("Athletics", -1);
  }
  carbon_should_be(2, hm.size);
  carbon_should_be(2, hm["Dexterity"].size);
  carbon_should_be(2, hm["Dexterity"]["Acrobatics"]);
  carbon_should_be(4, hm["Dexterity"]["Stealth"]);
  carbon_should_be(1, hm["Strength"].size);
  carbon_should_be(-1, hm["Strength"]["Athletics"]);
  return true;
}
