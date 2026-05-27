// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Chrono : CBN_Chrono_t {
  CBN_Chrono(void) : CBN_Chrono_t{carbon_chrono_start()} {}

  void Update(void) { carbon_chrono_update(this); }

  void Stop(void) { carbon_chrono_stop(this); }

  void Restart(void) { carbon_chrono_restart(this); }

  bool IsRunning(void) const { return carbon_chrono_is_running(this); }
};

#endif
