/*
  $$==================$$
  ||       Time       ||
  $$==================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Returns a timestamp in seconds obtained from a monotonic clock.
 *
 * If a high-resolution monotonic clock (1e9) is not available,
 * a realtime system-wide clock (1e6) is used instead.
 *
 * @return The obtained timestamp in seconds.
 */
CBNDEF f64 carbon_time_get(void);
CBNDEF_AKA(cbn::time, Get, carbon_time_get);
CBNDEF_AKA(cbn::time, now, Get);

/**
 * @brief Returns the current timestamp in the ISO-8601 format.
 *
 * The aforementioned format (`YYYY-mm-ddTHH:MM:SSZ`) consists of:
 * (YYYY = year; mm = month; dd = day; HH = hour; MM = minute; SS = second).
 * (T = date and hour separator; Z = timezone offset, i.e. +00:00).
 *
 * @return The formatted timestamp as a C-string (in static local cyclic memory buffer).
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

/**
 * @brief Suspends the execution of the calling thread for a specified duration.
 * @param ms The amount of time to sleep.
 */
CBNDEF void carbon_time_sleep(u64 ms);

/**
 * @brief Generates a pseudo-random 64-bit snowflake-like unique identifier.
 *
 * [1][TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT][RRRRRRRRRRRRRRRRRRRRRR]
 *  ^  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~
 *  |                                           ^                       ^
 *  |                                           |                       |------- 22-bit random payload [bits 21..0]
 *  |                                           |
 *  |                                           |------- 42-bit Timestamp (ms) [bits 62..22]
 *  |
 *  |------- Unused Most-Significant-Bit (MSB) set to 1 [bit 63]
 *
 * @return The new 64-bit snowflake ID.
 */
CBNDEF u64 carbon_time_snowflake(void);
CBNDEF_AKA(cbn::time, Snowflake, carbon_time_snowflake);

/**
 * @brief Extracts the timestamp (ms) from a snowflake ID.
 * @param snowflake The 64-bit snowflake ID to parse.
 * @return The timestamp (ms).
 */
CBNDEF u64 carbon_time_snowflake_ts(u64 snowflake);
