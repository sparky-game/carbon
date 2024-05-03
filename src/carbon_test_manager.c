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


#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <time.h>
#include <stdlib.h>

static Suite test_suite = {0};

Suite carbon_test_manager_spawn(void) {
  return (Suite) {0};
}

Test *carbon_test_manager_alloc(Suite *s) {
  Test *p = 0;
  size_t size = sizeof(Test);
  if (!s->tests) {
    p = malloc(size);
    if (!p) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_alloc :: failed to allocate memory (%zuB)\n", size);
      exit(1);
    }
  }
  else {
    size *= s->n;
    Test *prev_p = s->tests;
    p = realloc(s->tests, size);
    if (!p) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_alloc :: failed to reallocate memory (%zuB)\n", size);
      free(prev_p);
      exit(1);
    }
  }
  return p;
}

void carbon_test_manager_register_s(Suite *s, TestFunc test_func, char *name) {
  ++s->n;
  s->tests = carbon_test_manager_alloc(s);
  s->tests[s->n - 1] = (Test) {
    .f = test_func,
    .name = name
  };
}

void carbon_test_manager_register(TestFunc test_func, char *name) {
  carbon_test_manager_register_s(&test_suite, test_func, name);
}

void carbon_test_manager_cleanup_s(Suite *s) {
  if (!s->tests || !s->n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_cleanup_s :: Suite `s` has not been initialized\n");
    return;
  }
  free(s->tests);
  *s = (Suite) {0};
  s = 0;
}

void carbon_test_manager_cleanup(void) {
  carbon_test_manager_cleanup_s(&test_suite);
}

unsigned char carbon_test_manager_run(void) {
  CARBON_INFO("*** BSD Carbon (%s) ***\n", CARBON_VERSION);
  CARBON_INFO("---------------------------------------\n");
  if (!test_suite.tests || !test_suite.n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_run :: `test_suite` has not been initialized\n");
    return 1;
  }
  size_t passed = 0, failed = 0;
  clock_t total_time_start = clock();
  for (size_t i = 0; i < test_suite.n; ++i) {
    unsigned char result = test_suite.tests[i].f();
    if (result) {
      CARBON_INFO("(%zu/%zu) %s :: PASSED\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++passed;
    }
    else {
      CARBON_ERROR("(%zu/%zu) %s :: FAILED\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++failed;
    }
  }
  clock_t total_time_stop = clock();
  double total_time = (double) (total_time_stop - total_time_start) / CLOCKS_PER_SEC;
  if (failed) {
    CARBON_ERROR("=========== %zu failed, %zu passed in %.2fs ===========\n",
                 failed,
                 passed,
                 total_time);
    return 1;
  }
  else CARBON_INFO("=========== %zu passed in %.2fs ===========\n",
                   passed,
                   total_time);
  carbon_test_manager_cleanup();
  return 0;
}
