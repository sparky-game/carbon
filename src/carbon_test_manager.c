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
static CmdArgs cmd_args = {0};

static const char * const help_msg = "usage: %s [OPTION]\n"
  "Options:\n"
  "  -o, --output     output JUnit XML test results to specific file (default: `carbon_results.xml`)\n"
  "  -h, --help       display this help and exit\n"
  "  -v, --version    output version information and exit\n\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "BSD Carbon home page: <https://github.com/sparky-game/carbon>\n";

static const char * const version_msg = "BSD Carbon %s\n"
  "Copyright (C) 2024 Wasym A. Alonso\n"
  "License MIT: <https://opensource.org/license/MIT>.\n"
  "This is free software: you are free to change and redistribute it.\n"
  "There is NO WARRANTY, to the extent permitted by law.\n\n"
  "Written by Wasym A. Alonso\n";

void carbon_test_manager_argparse(int argc, char **argv) {
  if (argc == 1) return;
  if (argc == 3 && (!strcmp(argv[1], "-o") || !strcmp(argv[1], "--output"))) {
    cmd_args.output = argv[2];
    return;
  }
  if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    CARBON_INFO(help_msg, argv[0]);
    exit(0);
  }
  if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
    CARBON_INFO(version_msg, CARBON_VERSION);
    exit(0);
  }
  else {
    CARBON_ERROR("[ERROR]: unrecognized option\nTry '%s --help' for more information.\n", argv[0]);
    exit(1);
  }
}

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
  carbon_junit_testsuite junit_testsuite_info = { .tests = test_suite.n };
  carbon_junit_testcase junit_testcase_infos[test_suite.n];
  memset(junit_testcase_infos, 0, test_suite.n * sizeof(carbon_junit_testcase));
  clock_t total_time_start = clock();
  for (size_t i = 0; i < test_suite.n; ++i) {
    unsigned char result = test_suite.tests[i].f();
    memset(junit_testcase_infos[i].name, 0, sizeof(junit_testcase_infos[i].name));
    strncpy(junit_testcase_infos[i].name, test_suite.tests[i].name, sizeof(junit_testcase_infos[i].name) - 1);
    if (result) {
      CARBON_INFO(CARBON_COLOR_GREEN "(%zu/%zu) %s :: PASSED" CARBON_COLOR_RESET "\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++passed;
    }
    else {
      CARBON_ERROR(CARBON_COLOR_RED "(%zu/%zu) %s :: FAILED" CARBON_COLOR_RESET "\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++failed;
      ++junit_testcase_infos[i].has_failed;
    }
  }
  clock_t total_time_stop = clock();
  double total_time = (double) (total_time_stop - total_time_start) / CLOCKS_PER_SEC;
  unsigned int total_time_micro = (unsigned int) (total_time * 1e6);
  unsigned char status = 0;
  junit_testsuite_info.time = total_time;
  junit_testsuite_info.failures = failed;
  if (failed) {
    if (!((int) total_time)) CARBON_ERROR(CARBON_COLOR_RED "=========== %zu failed, %zu passed in %uμs ===========" CARBON_COLOR_RESET "\n",
                                          failed,
                                          passed,
                                          total_time_micro);
    else CARBON_ERROR(CARBON_COLOR_RED "=========== %zu failed, %zu passed in %.2fs ===========" CARBON_COLOR_RESET "\n", failed, passed, total_time);
    ++status;
  }
  else {
    if (!((int) total_time)) CARBON_INFO(CARBON_COLOR_GREEN "=========== %zu passed in %uμs ===========" CARBON_COLOR_RESET "\n",
                                         passed,
                                         total_time_micro);
    else CARBON_INFO(CARBON_COLOR_GREEN "=========== %zu passed in %.2fs ===========" CARBON_COLOR_RESET "\n", passed, total_time);
  }
  carbon_junit_output(&junit_testsuite_info, junit_testcase_infos, cmd_args.output);
  carbon_test_manager_cleanup();
  return status;
}
