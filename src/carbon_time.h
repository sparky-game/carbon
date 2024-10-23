// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_TIME_H_
#define CARBON_TIME_H_

f64 carbon_time_get(void);

void carbon_time_sleep(u64 ms);

#endif  // CARBON_TIME_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_time.c"
#endif  // CARBON_IMPLEMENTATION
