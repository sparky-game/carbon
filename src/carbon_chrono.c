// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Chrono carbon_chrono_start(void) {
  return (CBN_Chrono) {
    .initial = carbon_time_get(),
    .elapsed = 0
  };
}

void carbon_chrono_update(CBN_Chrono *c) {
  if (!c->initial) return;
  c->elapsed = carbon_time_get() - c->initial;
}

void carbon_chrono_stop(CBN_Chrono *c) {
  c->initial = 0;
}

void carbon_chrono_restart(CBN_Chrono *c) {
  *c = carbon_chrono_start();
}
