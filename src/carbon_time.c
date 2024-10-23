// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static f64 clock_freq;
static LARGE_INTEGER start_time;

static void clock_setup(void) {
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  clock_freq = 1.0 / (f64) freq.QuadPart;
  QueryPerformanceCounter(&start_time);
}
#else
#include <time.h>
#include <unistd.h>
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <sys/time.h>
#endif
#endif

f64 carbon_time_get(void) {
#ifdef _WIN32
  if (!clock_freq) clock_setup();
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

void carbon_time_sleep(u64 ms) {
#ifdef _WIN32
  Sleep(ms);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = ms / 1e3;
  ts.tv_nsec = (ms % (u64) 1e3) * 1e6;
  nanosleep(&ts, 0);
#else
#warning The compiled implementation of `carbon_time_sleep` uses only second-level resolution. To get a high precision implementation, change to a different C standard
  sleep(ms / 1e3);
#endif
}
