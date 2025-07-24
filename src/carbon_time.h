/*
**  $$==================$$
**  ||       Time       ||
**  $$==================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct {
  u64 timestamp;
  u64 random;
} CBN_SnowflakeComponents;

/**
 * @brief Returns a timestamp in seconds obtained from a monotonic clock.
 *
 * If a high-resolution monotonic clock (1e9) is not available,
 * a realtime system-wide clock (1e6) is used instead.
 *
 * @return The obtained timestamp in seconds.
 */
CARBON_API f64 carbon_time_get(void);

CARBON_API char *carbon_time_get_iso8601(void);

/**
 * @brief Returns the current timestamp in the `YYYYmmddHHMMSS` format.
 *
 * The aforementioned format consists of:
 * (YYYY = year; mm = month; dd = day; HH = hour; MM = minute; SS = second).
 *
 * @return The formatted timestamp as 64-bit uint number, e.g. `20250511205156`.
 */
CARBON_API u64 carbon_time_get_yyyymmddhhmmss(void);

CARBON_API void carbon_time_sleep(u64 ms);
CARBON_API u64 carbon_time_snowflake_get(void);
CARBON_API CBN_SnowflakeComponents carbon_time_snowflake_parse(u64 snowflake);
