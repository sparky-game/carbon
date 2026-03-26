// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#undef TEST
#define TEST(x) CARBON_TEST(carbon_list, x)

TEST(new_free) {
  cbn::List<i32> l;
  carbon_should_be(0, l.capacity);
  carbon_should_be(sizeof(typeof(l)::value_type), l.stride);
  carbon_should_be(0, l.size);
  carbon_should_be_p(0, l.items);
  return true;
}

TEST(push_element) {
  cbn::List<i32> l;
  typeof(l)::value_type i = 7;
  l.Push(i);
  carbon_should_be(1, l.size);
  carbon_should_be(i, l[0]);
  return true;
}

TEST(pop_element) {
  cbn::List<i32> l;
  typeof(l)::value_type x = 1, i = 7;
  l.Push(x);
  l.Push(i);
  carbon_should_be(2, l.size);
  auto j = l.PopBack();
  carbon_should_be(i, j);
  carbon_should_be(1, l.size);
  return true;
}

TEST(find_element) {
  cbn::List<i32> l;
  typeof(l)::value_type i = 1, j = 7, k = 3;
  l.Push(i);
  l.Push(j);
  carbon_should_be(2, l.size);
  carbon_should_be(0, l.Find(i));
  carbon_should_be(1, l.Find(j));
  carbon_should_be(-1, l.Find(k));
  carbon_should_be(-1, l.Find(777));
  return true;
}

TEST(remove_element) {
  cbn::List<i32> l;
  typeof(l)::value_type i = 1, j = 7, k = 3;
  l.Push(i);
  l.Push(j);
  l.Push(k);
  carbon_should_be(3, l.size);
  carbon_should_be(i, l[0]);
  carbon_should_be(j, l[1]);
  carbon_should_be(k, l[2]);
  l.Remove(0);
  carbon_should_be(2, l.size);
  carbon_should_be(j, l[0]);
  carbon_should_be(k, l[1]);
  l.Remove(1);
  carbon_should_be(1, l.size);
  carbon_should_be(j, l[0]);
  return true;
}
