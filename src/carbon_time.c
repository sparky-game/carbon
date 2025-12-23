// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_TIME_GET_ISO8601_FMT         "%Y-%m-%dT%H:%M:%S%z"
#define CARBON_TIME_GET_ISO8601_MAX_LEN     30
#define CARBON_TIME_GET_ISO8601_MAX_BUFFERS 4

f64 carbon_time_get(void) {
#ifdef _WIN32
  static f64 clock_freq;
  static LARGE_INTEGER start_time;
  if (!clock_freq) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clock_freq = 1.0 / (f64) freq.QuadPart;
    QueryPerformanceCounter(&start_time);
  }
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (f64) now.QuadPart * clock_freq;
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  return now.tv_sec + now.tv_nsec * 1e-9;
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec + now.tv_usec * 1e-6;
#else
#error `carbon_time_get` not implemented for this platform
#endif
}

char *carbon_time_get_iso8601(void) {
  static usz i = 0;
  static char ts[CARBON_TIME_GET_ISO8601_MAX_BUFFERS][CARBON_TIME_GET_ISO8601_MAX_LEN];
  char *t = ts[i];
  time_t now = time(0);
  struct tm *now_info = localtime(&now);
  strftime(t, CARBON_TIME_GET_ISO8601_MAX_LEN, CARBON_TIME_GET_ISO8601_FMT, now_info);
  ++i;
  if (i >= CARBON_TIME_GET_ISO8601_MAX_BUFFERS) i = 0;
  return t;
}

u64 carbon_time_get_yyyymmddhhmmss(void) {
  time_t now = time(0);
  struct tm *now_info = localtime(&now);
  const u64 fmt[] = {
    (u64) now_info->tm_year + 1900,  // YYYY
    (u64) now_info->tm_mon + 1,      // mm
    (u64) now_info->tm_mday,         // dd
    (u64) now_info->tm_hour,         // HH
    (u64) now_info->tm_min,          // MM
    (u64) now_info->tm_sec           // SS
  };
  u64 result = fmt[0];
  for (usz i = 1; i < CARBON_ARRAY_LEN(fmt); ++i) {
    if (fmt[i] < 10) result *= 10;
    result = carbon_math_concat(result, fmt[i]);
  }
  return result;
}

void carbon_time_sleep(u64 ms) {
#ifdef _WIN32
  Sleep(ms);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = ms / 1e3;
  ts.tv_nsec = (ms % (u64) 1e3) * 1e6;
  nanosleep(&ts, 0);
#else
  sleep(ms / 1e3);
#endif
}

u64 carbon_time_snowflake_get(void) {
  u64 timestamp = (u64) carbon_time_get() & ((1ULL << 42) - 1);
  carbon_rng_mt19937_64_srand(timestamp);
  u64 random = carbon_rng_mt19937_64_rand() % (1ULL << 22);
  /*
  ** [1][TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT][RRRRRRRRRRRRRRRRRRRRRR]
  **  ^  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~
  **  |                                           ^                       ^
  **  |                                           |                       |------- 22-bit random number at pos. [21..0]
  **  |                                           |
  **  |                                           |------- 42-bit Timestamp at pos. [62..22]
  **  |
  **  |------- Unused Most-Significant-Bit (MSB) set to 1 at pos. 63
  **                               (1ULL << 60 == 2^60 == 19 digits)
  */
  return (1ULL << 60) | (timestamp << 22) | random;
}

CBN_SnowflakeComponents carbon_time_snowflake_parse(u64 snowflake) {
  return (CBN_SnowflakeComponents) {
    .timestamp = ((snowflake >> 22) & ((1ULL << 42) - 1)) & ((1ULL << 32) - 1),
    .random    = snowflake & ((1ULL << 22) - 1)
  };
}
