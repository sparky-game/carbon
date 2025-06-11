// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_circularqueue, name)

TEST(create_destroy) {
  CBN_CircularQueue cq = carbon_circularqueue_create(5, sizeof(i32));
  carbon_should_be(5, cq.capacity);
  carbon_should_be(sizeof(i32), cq.stride);
  carbon_should_be(0, cq.size);
  carbon_should_not_be_p(0, cq.items);
  carbon_circularqueue_destroy(&cq);
  carbon_should_be(0, cq.capacity);
  carbon_should_be(0, cq.stride);
  carbon_should_be(0, cq.size);
  carbon_should_be_p(0, cq.items);
  return CARBON_OK;
}

TEST(push_element) {
  CBN_CircularQueue cq = carbon_circularqueue_create(5, sizeof(i32));
  i32 i = 7;
  carbon_circularqueue_push(&cq, &i);
  carbon_should_be(1, cq.size);
  carbon_should_be(i, carbon_circularqueue_at(i32, cq, 0));
  carbon_circularqueue_destroy(&cq);
  return CARBON_OK;
}

TEST(pop_element) {
  CBN_CircularQueue cq = carbon_circularqueue_create(5, sizeof(i32));
  i32 x = 1, i = 7, j = 0;
  carbon_circularqueue_push(&cq, &x);
  carbon_circularqueue_push(&cq, &i);
  carbon_should_be(2, cq.size);
  carbon_circularqueue_pop(&cq, &j);
  carbon_should_be(x, j);
  carbon_should_be(1, cq.size);
  carbon_circularqueue_destroy(&cq);
  return CARBON_OK;
}

#undef TEST
