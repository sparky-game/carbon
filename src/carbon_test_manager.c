// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

static CBN_Suite carbon_test_manager__test_suite;
static CBN_CmdArgs carbon_test_manager__cmd_args;

CARBON_INLINE void carbon_test_manager__cleanup_and_exit(void) {
  carbon_test_manager_cleanup(&carbon_test_manager__test_suite);
  exit(1);
}

void carbon_test_manager_argparse(i32 argc, char **argv) {
  static const char * const help_msg = "usage: %s [OPTION]\n"
    "Options:\n"
    "  -n, --no-output  disable JUnit XML test results output\n"
    "  -o, --output     output JUnit XML test results to specific file (default: `%s`)\n"
    "  -h, --help       display this help and exit\n"
    "  -v, --version    output version information and exit\n"
    "\n"
    "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
    "%s homepage: <https://github.com/sparky-game/carbon>\n";
  static const char * const version_msg = "%s %s\n"
    "Copyright (C) Wasym A. Alonso. All Rights Reserved.\n"
    "License BSD-3-Clause: <https://opensource.org/license/BSD-3-Clause>.\n"
    "License GPL-3.0-only: <https://www.gnu.org/licenses/gpl-3.0>.\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.\n"
    "\n"
    "Written by Wasym A. Alonso\n";
  if (argc == 1) return;
  if (argc == 2 && (!carbon_string_cmp(argv[1], "-n") || !carbon_string_cmp(argv[1], "--no-output"))) {
    carbon_test_manager__cmd_args.no_output = true;
    return;
  }
  if (argc == 3 && (!carbon_string_cmp(argv[1], "-o") || !carbon_string_cmp(argv[1], "--output"))) {
    carbon_test_manager__cmd_args.output = argv[2];
    return;
  }
  if (argc == 2 && (!carbon_string_cmp(argv[1], "-h") || !carbon_string_cmp(argv[1], "--help"))) {
    CARBON_INFO_RAW(help_msg, argv[0], CARBON_JUNIT_XML_OUT_FILENAME, CARBON_NAME);
    exit(0);
  }
  if (argc == 2 && (!carbon_string_cmp(argv[1], "-v") || !carbon_string_cmp(argv[1], "--version"))) {
    CARBON_INFO_RAW(version_msg, CARBON_NAME, CARBON_VERSION);
    exit(0);
  }
  CARBON_ERROR("unrecognized option\nTry '%s --help' for more information.", argv[0]);
  exit(1);
}

void carbon_test_manager_rebuild(const char *bin_file, const char *src_file) {
  if (strstr(bin_file, ".old")) return;
  carbon_test_manager__test_suite.files = carbon_strlist_create(true);
  carbon_strlist_push(&carbon_test_manager__test_suite.files, src_file);
  for (usz i = 0; i < carbon_test_manager__test_suite.n; ++i) {
    carbon_strlist_push(&carbon_test_manager__test_suite.files, carbon_test_manager__test_suite.tests[i].filename);
  }
  // 0. Check if needs rebuild (compare timestamps of binary vs source)
  u8 needs_rebuild = false;
  i32 bin_timestamp = carbon_fs_mtime(bin_file);
  if (!bin_timestamp) carbon_test_manager__cleanup_and_exit();
  for (usz i = 0; i < carbon_test_manager__test_suite.files.size; ++i) {
    i32 src_timestamp = carbon_fs_mtime(carbon_test_manager__test_suite.files.items[i]);
    if (!src_timestamp) carbon_test_manager__cleanup_and_exit();
    // NOTE: if even a single source file is fresher than binary file, then it needs rebuild
    if (src_timestamp > bin_timestamp) {
      needs_rebuild = true;
      break;
    }
  }
  if (!needs_rebuild) return;
  // 1. Rename `./carbon` -> `./carbon.old`
  char *bin_file_old = carbon_string_fmt("%s.old", bin_file);
  if (!carbon_fs_rename(bin_file, bin_file_old)) carbon_test_manager__cleanup_and_exit();
  // 2. Rebuild binary using `carbon_test_manager__test_suite.files` as args
  i32 rebuild_status_code = 0;
  pid_t rebuild_child_pid = fork();
  if (rebuild_child_pid == -1) {
    CARBON_ERROR("unable to fork child process");
    if (!carbon_fs_rename(bin_file_old, bin_file)) carbon_test_manager__cleanup_and_exit();
  }
  else if (rebuild_child_pid == 0) {
    char *argv[128];
    memset(argv, 0, 128 * sizeof(char *));
    argv[0] = (char *) CARBON_COMPILER;
    argv[1] = (char *) "-I";
    argv[2] = (char *) ".";
    argv[3] = (char *) "-fsanitize=address,undefined";
    argv[4] = (char *) "-o";
    argv[5] = (char *) bin_file;
    for (usz i = 0; i < carbon_test_manager__test_suite.files.size; ++i) {
      argv[i + 6] = carbon_test_manager__test_suite.files.items[i];
    }
    if (-1 == execvp(argv[0], argv)) {
      CARBON_ERROR("unable to execvp from child process");
      carbon_fs_rename(bin_file_old, bin_file);
      carbon_test_manager__cleanup_and_exit();
    }
  }
  // 3. Wait for rebuilding (child process) ends correctly
  else waitpid(rebuild_child_pid, &rebuild_status_code, 0);
  if (rebuild_status_code != 0) {
    CARBON_ERROR("errors detected during rebuild");
    carbon_fs_rename(bin_file_old, bin_file);
    carbon_test_manager__cleanup_and_exit();
  }
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Binary rebuilt successfully (`%s`)", bin_file);
  CARBON_INFO("=======================================");
  // 4. Replace current binary with new one (execvp)
  char *argv[4];
  memset(argv, 0, 4 * sizeof(char *));
  argv[0] = (char *) bin_file;
  if (carbon_test_manager__cmd_args.output) {
    argv[1] = (char *) "-o";
    argv[2] = carbon_test_manager__cmd_args.output;
  }
  if (-1 == execvp(argv[0], argv)) {
    CARBON_ERROR("unable to execvp rebuilt binary");
    carbon_fs_rename(bin_file_old, bin_file);
    carbon_test_manager__cleanup_and_exit();
  }
}

CBN_Suite carbon_test_manager_spawn(void) {
  CBN_Suite s;
  memset(&s, 0, sizeof(CBN_Suite));
  return s;
}

CBN_Test *carbon_test_manager_alloc(CBN_Suite *s) {
  CBN_Test *p = 0;
  usz size = sizeof(CBN_Test);
  if (!s->tests) {
    p = (CBN_Test *) CARBON_MALLOC(size);
    if (!p) {
      CARBON_ERROR("failed to allocate memory (%zuB)", size);
      exit(1);
    }
  }
  else {
    size *= s->n;
    CBN_Test *prev_p = s->tests;
    p = (CBN_Test *) CARBON_REALLOC(s->tests, size);
    if (!p) {
      CARBON_ERROR("failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      exit(1);
    }
  }
  return p;
}

void carbon_test_manager_register_s(CBN_Suite *s, CBN_TestFunc test_func, const char *name, const char *filename) {
  ++s->n;
  s->tests = carbon_test_manager_alloc(s);
  s->tests[s->n - 1] = (CBN_Test) {
    .f        = test_func,
    .name     = (char *) name,
    .filename = (char *) filename
  };
}

void carbon_test_manager_register(CBN_TestFunc test_func, const char *name, const char *filename) {
  carbon_test_manager_register_s(&carbon_test_manager__test_suite, test_func, name, filename);
}

void carbon_test_manager_cleanup(CBN_Suite *s) {
  if (!s->tests || !s->n) {
    CARBON_ERROR("Suite `s` has not been initialized");
    return;
  }
  CARBON_FREE(s->tests);
  carbon_strlist_destroy(&s->files);
  memset(s, 0, sizeof(CBN_Suite));
  s = 0;
}

u8 carbon_test_manager_run_s(CBN_Suite *s) {
  CARBON_INFO_COLOR(CARBON_COLOR_CYAN, "*** %s (%s) ***", CARBON_NAME, CARBON_VERSION);
  CARBON_INFO("=======================================");
  if (!s->tests || !s->n) {
    CARBON_ERROR("`(Suite *) s` has not been initialized");
    return EXIT_FAILURE;
  }
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Collected %zu tests", s->n);
  if (carbon_test_manager__cmd_args.no_output) CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output disabled");
  else CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output to ./%s", carbon_test_manager__cmd_args.output ?: CARBON_JUNIT_XML_OUT_FILENAME);
  CARBON_INFO("=======================================");
  usz passed = 0, failed = 0;
  CBN_List junit_testcase_infos = carbon_list_create(sizeof(CBN_JUnitTestcase));
  CBN_Clock clk = carbon_clock_start();
  for (usz i = 0; i < s->n; ++i) {
    u8 has_passed = s->tests[i].f();
    if (has_passed) {
      CARBON_INFO_COLOR(CARBON_COLOR_GREEN, "(%zu/%zu) %s :: PASSED", i + 1, s->n, s->tests[i].name);
      ++passed;
    }
    else {
      CARBON_ERROR_ASS("(%zu/%zu) %s :: FAILED", i + 1, s->n, s->tests[i].name);
      ++failed;
    }
    if (!carbon_test_manager__cmd_args.no_output) {
      CBN_JUnitTestcase tjc = {
        .name = s->tests[i].name,
        .has_failed = !has_passed
      };
      carbon_list_push(&junit_testcase_infos, &tjc);
    }
  }
  carbon_clock_update(&clk), carbon_clock_stop(&clk);
  u32 total_time_micro = (u32) (clk.elapsed * 1e6);
  u8 status = EXIT_SUCCESS;
  if (failed) {
    if (!((i32) clk.elapsed)) CARBON_ERROR_RAW("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %uμs" CARBON_COLOR_RESET " ===========\n",
                                               failed,
                                               passed,
                                               total_time_micro);
    else CARBON_ERROR_RAW("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %.2fs" CARBON_COLOR_RESET " ===========\n",
                          failed,
                          passed,
                          clk.elapsed);
    status = EXIT_FAILURE;
  }
  else {
    if (!((i32) clk.elapsed)) CARBON_INFO_RAW("=========== " CARBON_COLOR_GREEN "%zu passed in %uμs" CARBON_COLOR_RESET " ===========\n",
                                              passed,
                                              total_time_micro);
    else CARBON_INFO_RAW("=========== " CARBON_COLOR_GREEN "%zu passed in %.2fs" CARBON_COLOR_RESET " ===========\n",
                         passed,
                         clk.elapsed);
  }
  if (!carbon_test_manager__cmd_args.no_output) carbon_junit_output(junit_testcase_infos, carbon_test_manager__cmd_args.output, failed, clk.elapsed);
  carbon_list_destroy(&junit_testcase_infos);
  carbon_test_manager_cleanup(s);
  return status;
}

u8 carbon_test_manager_run(void) {
  return carbon_test_manager_run_s(&carbon_test_manager__test_suite);
}
