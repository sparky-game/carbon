// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static double clock_freq;
static LARGE_INTEGER start_time;

static void clock_setup(void) {
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  clock_freq = 1.0 / (double) freq.QuadPart;
  QueryPerformanceCounter(&start_time);
}
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <sys/time.h>
#endif

double carbon_time_get(void) {
#ifdef _WIN32
  if (!clock_freq) clock_setup();
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (double) now.QuadPart * clock_freq;
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  return now.tv_sec + now.tv_nsec * 1e-9;
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec + now.tv_usec * 1e-6;
#else
#error "`carbon_time_get` not implemented"
#endif
}
