// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <stdlib.h>

#include <sys/wait.h>
#include <sys/stat.h>

static Suite test_suite = {0};
static CmdArgs cmd_args = {0};

static const char * const help_msg = "usage: %s [OPTION]\n"
  "Options:\n"
  "  -o, --output     output JUnit XML test results to specific file (default: `%s`)\n"
  "  -h, --help       display this help and exit\n"
  "  -v, --version    output version information and exit\n\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "%s homepage: <https://github.com/sparky-game/carbon>\n";

static const char * const version_msg = "%s %s\n"
  "Copyright (C) 2024 Wasym A. Alonso\n"
  "License BSD-3-Clause: <https://opensource.org/license/BSD-3-Clause>.\n"
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
    CARBON_INFO(help_msg, argv[0], CARBON_JUNIT_XML_OUT_FILENAME, CARBON_NAME);
    exit(0);
  }
  if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
    CARBON_INFO(version_msg, CARBON_NAME, CARBON_VERSION);
    exit(0);
  }
  CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "unrecognized option\nTry '%s --help' for more information." CARBON_COLOR_RESET "\n", argv[0]);
  exit(1);
}

void carbon_test_manager_rebuild(const char *bin_file, const char *src_file) {
#ifndef CARBON_FEATURE_REBUILD
#warning Currently the auto-rebuilding feature is disabled. If you want to enable it, be sure to define the `CARBON_FEATURE_REBUILD` macro.
  (void) bin_file;
  (void) src_file;
  return;
#else
  if (strstr(bin_file, ".old")) return;
  test_suite.files = carbon_uniquelist_create();
  carbon_uniquelist_push(&test_suite.files, src_file);
  for (size_t i = 0; i < test_suite.n; ++i) {
    carbon_uniquelist_push(&test_suite.files, test_suite.tests[i].filename);
  }
  // 0. Check if needs rebuild (compare timestamps of binary vs source)
  struct stat statbuf = {0};
  if (-1 == stat(bin_file, &statbuf)) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: unable to stat file" CARBON_COLOR_RESET "\n");
    carbon_test_manager_cleanup(&test_suite);
    exit(1);
  }
  int bin_timestamp = statbuf.st_mtime;
  unsigned char needs_rebuild = 0;
  for (size_t i = 0; i < test_suite.files.size; ++i) {
    if (-1 == stat(test_suite.files.items[i], &statbuf)) {
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: unable to stat file" CARBON_COLOR_RESET "\n");
      carbon_test_manager_cleanup(&test_suite);
      exit(1);
    }
    int src_timestamp = statbuf.st_mtime;
    // NOTE: if even a single source file is fresher than binary file, then it needs rebuild
    if (src_timestamp > bin_timestamp) {
      ++needs_rebuild;
      break;
    }
  }
  if (!needs_rebuild) return;
  // 1. Rename `./carbon` -> `./carbon.old`
  char bin_file_old[128] = {0};
  strcpy(bin_file_old, bin_file);
  strcat(bin_file_old, ".old");
  if (!carbon_fs_rename(bin_file, bin_file_old)) {
    carbon_test_manager_cleanup(&test_suite);
    exit(1);
  }
  // 2. Rebuild binary using `test_suite.files` as args
  int rebuild_status_code = 0;
  pid_t rebuild_child_pid = fork();
  if (rebuild_child_pid == -1) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: unable to fork child process" CARBON_COLOR_RESET "\n");
    if (!carbon_fs_rename(bin_file_old, bin_file)) {
      carbon_test_manager_cleanup(&test_suite);
      exit(1);
    }
  }
  else if (rebuild_child_pid == 0) {
    char *argv[test_suite.files.size + 9];
    memset(argv, 0, (test_suite.files.size + 9) * sizeof(char *));
#if defined(__GNUC__)
    argv[0] = "gcc";
#elif defined(__clang__)
    argv[0] = "clang";
#else
    argv[0] = "cc";
#endif
    argv[1] = "-D";
    argv[2] = "CARBON_FEATURE_REBUILD";
    argv[3] = "-I";
    argv[4] = ".";
    argv[5] = "-fsanitize=address,undefined";
    argv[6] = "-o";
    argv[7] = (char *) bin_file;
    for (size_t i = 0; i < test_suite.files.size; ++i) {
      argv[i + 8] = test_suite.files.items[i];
    }
    if (-1 == execvp(argv[0], argv)) {
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: unable to execvp from child process" CARBON_COLOR_RESET "\n");
      if (!carbon_fs_rename(bin_file_old, bin_file)) {
        carbon_test_manager_cleanup(&test_suite);
        exit(1);
      }
      carbon_test_manager_cleanup(&test_suite);
      exit(1);
    }
  }
  // 3. Wait for rebuilding (child process) ends correctly
  else waitpid(rebuild_child_pid, &rebuild_status_code, 0);
  if (rebuild_status_code != 0) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: errors detected during rebuild" CARBON_COLOR_RESET "\n");
    if (!carbon_fs_rename(bin_file_old, bin_file)) {
      carbon_test_manager_cleanup(&test_suite);
      exit(1);
    }
    carbon_test_manager_cleanup(&test_suite);
    exit(1);
  }
  CARBON_INFO(CARBON_COLOR_YELLOW "[*] Binary rebuilt successfully (`%s`)" CARBON_COLOR_RESET "\n", bin_file);
  CARBON_INFO("=======================================\n");
  // 4. Replace current binary with new one (execvp)
  char *argv[4] = {0};
  argv[0] = (char *) bin_file;
  if (cmd_args.output) {
    argv[1] = "-o";
    argv[2] = cmd_args.output;
  }
  if (-1 == execvp(argv[0], argv)) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_rebuild :: unable to execvp rebuilt binary" CARBON_COLOR_RESET "\n");
    if (!carbon_fs_rename(bin_file_old, bin_file)) {
      carbon_test_manager_cleanup(&test_suite);
      exit(1);
    }
    carbon_test_manager_cleanup(&test_suite);
    exit(1);
  }
#endif
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
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_alloc :: failed to allocate memory (%zuB)" CARBON_COLOR_RESET "\n", size);
      exit(1);
    }
  }
  else {
    size *= s->n;
    Test *prev_p = s->tests;
    p = realloc(s->tests, size);
    if (!p) {
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_alloc :: failed to reallocate memory (%zuB)" CARBON_COLOR_RESET "\n", size);
      free(prev_p);
      exit(1);
    }
  }
  return p;
}

void carbon_test_manager_register_s(Suite *s, TestFunc test_func, char *name, char *filename) {
  ++s->n;
  s->tests = carbon_test_manager_alloc(s);
  s->tests[s->n - 1] = (Test) {
    .f = test_func,
    .name = name,
    .filename = filename
  };
}

void carbon_test_manager_register(TestFunc test_func, char *name, char *filename) {
  carbon_test_manager_register_s(&test_suite, test_func, name, filename);
}

void carbon_test_manager_cleanup(Suite *s) {
  if (!s->tests || !s->n) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_cleanup_s :: Suite `s` has not been initialized" CARBON_COLOR_RESET "\n");
    return;
  }
  free(s->tests);
  carbon_uniquelist_destroy(&s->files);
  *s = (Suite) {0};
  s = 0;
}

unsigned char carbon_test_manager_run_s(Suite *s) {
  CARBON_INFO(CARBON_COLOR_CYAN "*** %s (%s) ***" CARBON_COLOR_RESET "\n", CARBON_NAME, CARBON_VERSION);
  CARBON_INFO("=======================================\n");
  if (!s->tests || !s->n) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_test_manager_run :: `(Suite *) s` has not been initialized" CARBON_COLOR_RESET "\n");
    return 1;
  }
  CARBON_INFO(CARBON_COLOR_YELLOW "[*] Collected %zu tests" CARBON_COLOR_RESET "\n", s->n);
  CARBON_INFO(CARBON_COLOR_YELLOW "[*] Output to ./%s" CARBON_COLOR_RESET "\n", cmd_args.output ?: CARBON_JUNIT_XML_OUT_FILENAME);
  CARBON_INFO("=======================================\n");
  size_t passed = 0, failed = 0;
  JUnitTestsuite junit_testsuite_info = { .tests = s->n };
  JUnitTestcase junit_testcase_infos[s->n];
  memset(junit_testcase_infos, 0, s->n * sizeof(JUnitTestcase));
  Clock clk = carbon_clock_start();
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
  carbon_clock_update(&clk);
  carbon_clock_stop(&clk);
  unsigned int total_time_micro = (unsigned int) (clk.elapsed * 1e6);
  unsigned char status = 0;
  junit_testsuite_info.time = clk.elapsed;
  junit_testsuite_info.failures = failed;
  if (failed) {
    if (!((int) clk.elapsed)) CARBON_ERROR("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %uμs" CARBON_COLOR_RESET " ===========\n",
                                           failed,
                                           passed,
                                           total_time_micro);
    else CARBON_ERROR("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %.2fs" CARBON_COLOR_RESET " ===========\n",
                      failed,
                      passed,
                      clk.elapsed);
    ++status;
  }
  else {
    if (!((int) clk.elapsed)) CARBON_INFO("=========== " CARBON_COLOR_GREEN "%zu passed in %uμs" CARBON_COLOR_RESET " ===========\n",
                                          passed,
                                          total_time_micro);
    else CARBON_INFO("=========== " CARBON_COLOR_GREEN "%zu passed in %.2fs" CARBON_COLOR_RESET " ===========\n",
                     passed,
                     clk.elapsed);
  }
  carbon_junit_output(&junit_testsuite_info, junit_testcase_infos, cmd_args.output);
  carbon_test_manager_cleanup(s);
  return status;
}

unsigned char carbon_test_manager_run(void) {
  return carbon_test_manager_run_s(&test_suite);
}
