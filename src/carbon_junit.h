/*
 * BSD Carbon --- A simple C/C++ unit testing framework
 * Copyright (C) 2024 Wasym A. Alonso
 *
 * This file is part of Carbon.
 *
 * Carbon is free software: you can redistribute it and/or modify
 * it under the terms of the BSD 3-Clause "New" or "Revised" License
 * as published by The Regents of the University of California.
 *
 * Carbon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * BSD 3-Clause "New" or "Revised" License for more details.
 *
 * You should have received a copy of the BSD 3-Clause "New" or
 * "Revised" License along with Carbon.
 * If not, see <https://opensource.org/license/BSD-3-Clause>.
 */


#ifndef CARBON_JUNIT_H_
#define CARBON_JUNIT_H_

#define CARBON_JUNIT_TESTCASE_NAME_MAX_LEN 512

typedef struct {
  double time;
  size_t tests;
  size_t failures;
} carbon_junit_testsuite;

typedef struct {
  char name[CARBON_JUNIT_TESTCASE_NAME_MAX_LEN];
  unsigned char has_failed;
} carbon_junit_testcase;

void carbon_junit_output(carbon_junit_testsuite *junit_ts, carbon_junit_testcase *junit_tcs, const char *out_filename);

#endif  // CARBON_JUNIT_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION
