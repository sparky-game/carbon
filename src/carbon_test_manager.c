// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

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
  "%s home page: <https://github.com/sparky-game/carbon>\n";

static const char * const version_msg = "%s %s\n"
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
    CARBON_INFO(help_msg, argv[0], CARBON_NAME);
    exit(0);
  }
  if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
    CARBON_INFO(version_msg, CARBON_NAME, CARBON_VERSION);
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

void carbon_test_manager_cleanup(Suite *s) {
  if (!s->tests || !s->n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_cleanup_s :: Suite `s` has not been initialized\n");
    return;
  }
  free(s->tests);
  *s = (Suite) {0};
  s = 0;
}

unsigned char carbon_test_manager_run_s(Suite *s) {
  CARBON_INFO("*** BSD Carbon (%s) ***\n", CARBON_VERSION);
  CARBON_INFO("---------------------------------------\n");
  if (!s->tests || !s->n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_run :: `(Suite *) s` has not been initialized\n");
    return 1;
  }
  size_t passed = 0, failed = 0;
  carbon_junit_testsuite junit_testsuite_info = { .tests = s->n };
  carbon_junit_testcase junit_testcase_infos[s->n];
  memset(junit_testcase_infos, 0, s->n * sizeof(carbon_junit_testcase));
  clock_t total_time_start = clock();
  for (size_t i = 0; i < s->n; ++i) {
    unsigned char result = s->tests[i].f();
    memset(junit_testcase_infos[i].name, 0, sizeof(junit_testcase_infos[i].name));
    strncpy(junit_testcase_infos[i].name, s->tests[i].name, sizeof(junit_testcase_infos[i].name) - 1);
    if (result) {
      CARBON_INFO(CARBON_COLOR_GREEN "(%zu/%zu) %s :: PASSED" CARBON_COLOR_RESET "\n", i + 1, s->n, s->tests[i].name);
      ++passed;
    }
    else {
      CARBON_ERROR(CARBON_COLOR_RED "(%zu/%zu) %s :: FAILED" CARBON_COLOR_RESET "\n", i + 1, s->n, s->tests[i].name);
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
  carbon_test_manager_cleanup(s);
  return status;
}

unsigned char carbon_test_manager_run(void) {
  return carbon_test_manager_run_s(&test_suite);
}
