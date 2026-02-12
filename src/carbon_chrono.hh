/*
  $$====================$$
  ||       Chrono       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a chrono/clock suitable for measuring time intervals.
 */
CBNDEF_T(CBN_Chrono) {
  f64 initial;
  f64 elapsed;
};
CBNDEF_AKA(cbn::time, Chrono, CBN_Chrono);

/**
 * @brief Creates and starts the chrono.
 * @return The Chrono object.
 */
CBNDEF CBN_Chrono carbon_chrono_start(void);

/**
 * @brief Updates the chrono by accumulating the elapsed time.
 * @param c The Chrono object.
 */
CBNDEF void carbon_chrono_update(CBN_Chrono *c);

/**
 * @brief Stops the chrono by zeroing the initial time.
 * @param c The Chrono object.
 */
CBNDEF void carbon_chrono_stop(CBN_Chrono *c);

/**
 * @brief Restarts the chrono.
 * @param c The Chrono object.
 */
CBNDEF void carbon_chrono_restart(CBN_Chrono *c);

/**
 * @brief Checks whether the chrono is running or not.
 * @param c The Chrono object.
 * @return A boolean value indicating the state of the chrono.
 */
CBNDEF u8 carbon_chrono_is_running(const CBN_Chrono *c);

#ifdef __cplusplus
struct CBN_Chrono : CBN_Chrono_t {
  static CBN_Chrono make(void) {
    return carbon_chrono_start();
  }

  void Update(void) {
    carbon_chrono_update(this);
  }

  void Stop(void) {
    carbon_chrono_stop(this);
  }

  void Restart(void) {
    carbon_chrono_restart(this);
  }

  bool IsRunning(void) const {
    return carbon_chrono_is_running(this);
  }
};
#endif
