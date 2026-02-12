// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static void counter(void *arg) {
  usz n = (usz)arg;
  for (usz i = 0; i < n; ++i) {
    cbn::println("[%zu] i = %zu", cbn::co::ID(), i);
    cbn::co::Yield();
  }
}

int main(void) {
  cbn::println("[%zu] Begin", cbn::co::ID());
  cbn::co::Create(counter, (void *) 3);
  cbn::co::Create(counter, (void *) 5);
  cbn::co::Create(counter, (void *) 7);
  cbn::co::Create(counter, (void *) 9);
  while (cbn::co::Alive() > 1) cbn::co::Yield();
  cbn::println("[%zu] End", cbn::co::ID());
}
