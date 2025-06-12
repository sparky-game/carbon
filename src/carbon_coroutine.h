/*
**  $$=======================$$
**  ||       Coroutine       ||
**  $$=======================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct {
  void *rsp;   // Stack pointer
  void *rsbp;  // Stack base pointer
} CBN_Coroutine_CTX;

typedef enum {
  CBN_COROUTINE_SLEEP_MODE_NONE,
  CBN_COROUTINE_SLEEP_MODE_READ,
  CBN_COROUTINE_SLEEP_MODE_WRITE
} CBN_Coroutine_SleepMode;

CARBON_API usz carbon_coroutine_id(void);
CARBON_API usz carbon_coroutine_alive(void);
CARBON_API void carbon_coroutine_go(void (*f)(void *), void *arg);
CARBON_API void carbon_coroutine_yield(void);
CARBON_API void carbon_coroutine_sleep_read(i32 fd);
CARBON_API void carbon_coroutine_sleep_write(i32 fd);
CARBON_API void carbon_coroutine_wakeup(usz id);
