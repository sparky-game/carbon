// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_CLOCK_H_
#define CARBON_CLOCK_H_

typedef struct {
  double initial;
  double elapsed;
} Clock;

Clock carbon_clock_start(void);

void carbon_clock_update(Clock *c);

void carbon_clock_stop(Clock *c);

#endif  // CARBON_CLOCK_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_clock.c"
#endif  // CARBON_IMPLEMENTATION
