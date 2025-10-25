// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_circularqueue_cxx, name)

TEST(make_free) {
  auto cq = cbn::CircularQueue<i32>::make(5);
  carbon_should_be(5, cq.capacity);
  carbon_should_be(sizeof(typeof(cq)::value_type), cq.stride);
  carbon_should_be(0, cq.size);
  carbon_should_not_be_p(0, cq.items);
  cq.Free();
  carbon_should_be(0, cq.capacity);
  carbon_should_be(0, cq.stride);
  carbon_should_be(0, cq.size);
  carbon_should_be_p(0, cq.items);
  return CARBON_OK;
}

TEST(push_element) {
  auto cq = cbn::CircularQueue<i32>::make(5);
  typeof(cq)::value_type i = 7;
  cq.Push(i);
  carbon_should_be(1, cq.size);
  carbon_should_be(i, cq[0]);
  cq.Free();
  return CARBON_OK;
}

TEST(pop_element) {
  auto cq = cbn::CircularQueue<i32>::make(5);
  typeof(cq)::value_type x = 1, i = 7;
  cq.Push(x);
  cq.Push(i);
  carbon_should_be(2, cq.size);
  auto j = cq.Pop();
  carbon_should_be(x, j);
  carbon_should_be(1, cq.size);
  cq.Free();
  return CARBON_OK;
}

#undef TEST
