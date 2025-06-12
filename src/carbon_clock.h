/*
**  $$===================$$
**  ||       Clock       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct {
  f64 initial;
  f64 elapsed;
} CBN_Clock;

CARBON_API CBN_Clock carbon_clock_start(void);
CARBON_API void carbon_clock_update(CBN_Clock *c);
CARBON_API void carbon_clock_stop(CBN_Clock *c);
