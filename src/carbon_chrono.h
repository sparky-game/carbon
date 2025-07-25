/*
**  $$====================$$
**  ||       Chrono       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a chrono/clock suitable for measuring time intervals.
 */
typedef struct CBN_Chrono {
  f64 initial;
  f64 elapsed;
#ifdef __cplusplus
  /**
   * @brief carbon_chrono_start
   * @return The Chrono object.
   */
  static CBN_Chrono make(void);
  /**
   * @brief carbon_chrono_update
   */
  void Update(void);
  /**
   * @brief carbon_chrono_stop
   */
  void Stop(void);
  /**
   * @brief carbon_chrono_restart
   */
  void Restart(void);
#endif
} CBN_Chrono;

/**
 * @brief Creates and starts the chrono.
 * @return The Chrono object.
 */
CARBON_API CBN_Chrono carbon_chrono_start(void);

/**
 * @brief Updates the chrono by accumulating the elapsed time.
 * @param c The Chrono object.
 */
CARBON_API void carbon_chrono_update(CBN_Chrono *c);

/**
 * @brief Stops the chrono by zeroing the initial time.
 * @param c The Chrono object.
 */
CARBON_API void carbon_chrono_stop(CBN_Chrono *c);

/**
 * @brief Restarts the chrono.
 * @param c The Chrono object.
 */
CARBON_API void carbon_chrono_restart(CBN_Chrono *c);
