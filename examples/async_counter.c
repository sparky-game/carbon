// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_IMPLEMENTATION
#include "../carbon.h"

static void counter(void *arg) {
  usz n = (usz) arg;
  for (usz i = 0; i < n; ++i) {
    carbon_log_info("[%zu] i = %zu", carbon_coroutine_id(), i);
    carbon_coroutine_yield();
  }
}

int main(void) {
  carbon_log_info("[%zu] Hello, World!", carbon_coroutine_id());
  carbon_coroutine_go(counter, (void *) 3);
  carbon_coroutine_go(counter, (void *) 5);
  carbon_coroutine_go(counter, (void *) 7);
  carbon_coroutine_go(counter, (void *) 9);
  while (carbon_coroutine_alive() > 1) carbon_coroutine_yield();
  carbon_log_info("[%zu] Done", carbon_coroutine_id());
  return 0;
}
