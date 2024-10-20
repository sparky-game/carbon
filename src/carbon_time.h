// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_TIME_H_
#define CARBON_TIME_H_

double carbon_time_get(void);

void carbon_time_sleep(unsigned long long ms);

#endif  // CARBON_TIME_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_time.c"
#endif  // CARBON_IMPLEMENTATION
