// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_TIME_GET_ISO8601_FMT         "%Y-%m-%dT%H:%M:%S%z"
#define CARBON_TIME_GET_ISO8601_MAX_LEN     30
#define CARBON_TIME_GET_ISO8601_MAX_BUFFERS 4

f64 carbon_time_get(void) {
#ifdef CARBON_TARGET_OS_WINDOWS
  static f64 clock_freq;
  static LARGE_INTEGER start;
  if (!clock_freq) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clock_freq = 1/(f64)freq.QuadPart;
    QueryPerformanceCounter(&start);
  }
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (f64)(now.QuadPart - start.QuadPart) * clock_freq;
#else
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  return now.tv_sec + now.tv_nsec * CARBON_SECS_PER_NANO;
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
    (u64)now_info->tm_year + 1900,  // YYYY
    (u64)now_info->tm_mon + 1,      // mm
    (u64)now_info->tm_mday,         // dd
    (u64)now_info->tm_hour,         // HH
    (u64)now_info->tm_min,          // MM
    (u64)now_info->tm_sec           // SS
  };
  u64 result = fmt[0];
  for (usz i = 1; i < CARBON_ARRAY_LEN(fmt); ++i) {
    if (fmt[i] < 10) result *= 10;
    result = carbon_math_concat(result, fmt[i]);
  }
  return result;
}

void carbon_time_sleep(u64 ms) {
#ifdef CARBON_TARGET_OS_WINDOWS
  Sleep(ms);
#else
  struct timespec ts;
  ts.tv_sec = ms/CARBON_MILLIS_PER_SEC;
  ts.tv_nsec = (ms % CARBON_MILLIS_PER_SEC) * CARBON_MICROS_PER_SEC;
  nanosleep(&ts, 0);
#endif
}

u64 carbon_time_snowflake(void) {
  u64 timestamp = (u64)(carbon_time_get() * CARBON_MILLIS_PER_SEC) & ((1ULL << 42) - 1);
  u64 random = carbon_rng_mt1993764() & ((1ULL << 22) - 1);
  return (1ULL << 63) | (timestamp << 22) | random;
}

u64 carbon_time_snowflake_ts(u64 snowflake) {
  return (snowflake >> 22) & ((1ULL << 42) - 1);
}
