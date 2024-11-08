// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_Clock carbon_clock_start(void) {
  return (CBN_Clock) {
    .initial = carbon_time_get(),
    .elapsed = 0
  };
}

void carbon_clock_update(CBN_Clock *c) {
  if (!c->initial) return;
  c->elapsed = carbon_time_get() - c->initial;
}

void carbon_clock_stop(CBN_Clock *c) {
  c->initial = 0;
}
