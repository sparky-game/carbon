// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

Clock carbon_clock_start(void) {
  return (Clock) {
    .initial = carbon_time_get(),
    .elapsed = 0
  };
}

void carbon_clock_update(Clock *c) {
  if (!c->initial) return;
  c->elapsed = carbon_time_get() - c->initial;
}

void carbon_clock_stop(Clock *c) {
  c->initial = 0;
}
