// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Chrono CBN_Chrono::make(void) {
  return carbon_chrono_start();
}

void CBN_Chrono::Update(void) {
  carbon_chrono_update(this);
}

void CBN_Chrono::Stop(void) {
  carbon_chrono_stop(this);
}

void CBN_Chrono::Restart(void) {
  carbon_chrono_restart(this);
}

bool CBN_Chrono::IsRunning(void) const {
  return carbon_chrono_is_running(this);
}
