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
CBNDEF f64 carbon_time_get(void);

/**
 * @brief Returns the current timestamp in the ISO-8601 format.
 *
 * The aforementioned format (`YYYY-mm-ddTHH:MM:SSZ`) consists of:
 * (YYYY = year; mm = month; dd = day; HH = hour; MM = minute; SS = second).
 * (T = date and hour separator; Z = UTC timezone specifier, i.e. +00:00).
 *
 * @return The formatted timestamp as a C-string (static circular storage).
 */
CBNDEF char *carbon_time_get_iso8601(void);

/**
 * @brief Returns the current timestamp in the `YYYYmmddHHMMSS` format.
 *
 * The aforementioned format consists of:
 * (YYYY = year; mm = month; dd = day; HH = hour; MM = minute; SS = second).
 *
 * @return The formatted timestamp as 64-bit uint number, e.g. `20250511205156`.
 */
CBNDEF u64 carbon_time_get_yyyymmddhhmmss(void);

CBNDEF void carbon_time_sleep(u64 ms);
CBNDEF u64 carbon_time_snowflake_get(void);
CBNDEF CBN_SnowflakeComponents carbon_time_snowflake_parse(u64 snowflake);
