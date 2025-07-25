// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

static CBN_Suite carbon_test_manager__test_suite;
static CBN_CmdArgs carbon_test_manager__cmd_args;

CARBON_INLINE void carbon_test_manager__cleanup_and_exit(void) {
  carbon_test_manager_cleanup(&carbon_test_manager__test_suite);
  exit(1);
}

void carbon_test_manager_argparse(i32 argc, char * const *argv) {
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
    "License AGPL-3.0-only: <https://www.gnu.org/licenses/agpl-3.0>.\n"
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
    carbon_print(help_msg, argv[0], CARBON_JUNIT_XML_OUT_FILENAME, CARBON_NAME);
    exit(0);
  }
  if (argc == 2 && (!carbon_string_cmp(argv[1], "-v") || !carbon_string_cmp(argv[1], "--version"))) {
    carbon_print(version_msg, CARBON_NAME, CARBON_VERSION);
    exit(0);
  }
  CBN_ERROR("unrecognized option\n    Try '%s --help' for more information.", argv[0]);
  exit(1);
}

void carbon_test_manager_rebuild(const char *src_file, char * const *host_argv) {
#ifdef _WIN32
  // TODO: consider what to do regarding rebuilding this whole thing...
  CARBON_UNUSED(src_file), CARBON_UNUSED(host_argv);
#else
  const char *bin_file = host_argv[0];
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
  carbon_strlist_foreach(carbon_test_manager__test_suite.files) {
    i32 src_timestamp = carbon_fs_mtime(carbon_strview_to_cstr(it.sv));
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
    CBN_ERROR("unable to fork child process");
    carbon_fs_rename(bin_file_old, bin_file), carbon_test_manager__cleanup_and_exit();
  }
  else if (rebuild_child_pid == 0) {
    char *argv[128];
    carbon_memory_set(argv, 0, 128 * sizeof(char));
    argv[0] = (char *) CARBON_COMPILER;
    argv[1] = (char *) "-I";
    argv[2] = (char *) ".";
    argv[3] = (char *) "-fsanitize=address,undefined";
    argv[4] = (char *) "-o";
    argv[5] = (char *) bin_file;
    carbon_strlist_foreach(carbon_test_manager__test_suite.files) {
      argv[it.i + 6] = carbon_strview_to_cstr(it.sv);
    }
    if (-1 == execvp(argv[0], argv)) {
      CBN_ERROR("unable to execvp from child process");
      carbon_fs_rename(bin_file_old, bin_file), carbon_test_manager__cleanup_and_exit();
    }
  }
  // 3. Wait for rebuilding (child process) ends correctly
  else waitpid(rebuild_child_pid, &rebuild_status_code, 0);
  if (rebuild_status_code != 0) {
    CBN_ERROR("errors detected during rebuild");
    carbon_fs_rename(bin_file_old, bin_file), carbon_test_manager__cleanup_and_exit();
  }
  carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Binary rebuilt successfully (`%s`)", bin_file);
  carbon_println("=======================================");
  // 4. Replace current binary with new one (execvp)
  if (-1 == execvp(bin_file, host_argv)) {
    CBN_ERROR("unable to execvp rebuilt binary");
    carbon_fs_rename(bin_file_old, bin_file), carbon_test_manager__cleanup_and_exit();
  }
#endif
}

CBN_Suite carbon_test_manager_spawn(void) {
  CBN_Suite s;
  carbon_memory_set(&s, 0, sizeof(s));
  return s;
}

CBN_Test *carbon_test_manager_alloc(CBN_Suite *s) {
  CBN_Test *p = 0;
  usz size = sizeof(CBN_Test);
  if (!s->tests) {
    p = (CBN_Test *) CBN_MALLOC(size);
    CBN_ASSERT(p && "failed to allocate memory");
  }
  else {
    size *= s->n;
    p = (CBN_Test *) CBN_REALLOC(s->tests, size);
    CBN_ASSERT(p && "failed to reallocate memory");
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
    CBN_ERROR("Suite `s` has not been initialized");
    return;
  }
  CBN_FREE(s->tests);
  carbon_strlist_destroy(&s->files);
  carbon_memory_set(s, 0, sizeof(*s));
}

u8 carbon_test_manager_run_s(CBN_Suite *s) {
  carbon_cprintln(CARBON_COLOR_CYAN, "*** %s (%s) ***", CARBON_NAME, CARBON_VERSION);
  carbon_println("=======================================");
  if (!s->tests || !s->n) {
    CBN_ERROR("`(Suite *) s` has not been initialized");
    return EXIT_FAILURE;
  }
  carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Collected %zu tests", s->n);
  if (carbon_test_manager__cmd_args.no_output) carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Output disabled");
  else carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Output to ./%s", carbon_test_manager__cmd_args.output ?: CARBON_JUNIT_XML_OUT_FILENAME);
  carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Compiler is " CARBON_COMPILER_VERSION);
  carbon_cprintln(CARBON_COLOR_YELLOW, "[*] Compiled on %s at %s", __DATE__, __TIME__);
  carbon_println("=======================================");
  usz passed = 0, failed = 0;
  CBN_List junit_testcase_infos = carbon_list_create(sizeof(CBN_JUnitTestcase));
  CBN_Chrono chr = carbon_chrono_start();
  for (usz i = 0; i < s->n; ++i) {
    u8 has_passed = s->tests[i].f();
    if (has_passed) {
      carbon_cprintln(CARBON_COLOR_GREEN, "(%.2zu/%.2zu) PASSED :: %s", i + 1, s->n, s->tests[i].name);
      ++passed;
    }
    else {
      carbon_ceprintln(CARBON_COLOR_RED, "(%.2zu/%.2zu) FAILED :: %s", i + 1, s->n, s->tests[i].name);
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
  carbon_chrono_update(&chr), carbon_chrono_stop(&chr);
  u32 total_time_micro = (u32) (chr.elapsed * 1e6);
  u8 status = EXIT_SUCCESS;
  if (failed) {
    if (!((i32) chr.elapsed)) carbon_eprintln("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %uμs" CARBON_COLOR_RESET " ===========",
                                              failed,
                                              passed,
                                              total_time_micro);
    else carbon_eprintln("=========== " CARBON_COLOR_RED "%zu failed, %zu passed in %.2fs" CARBON_COLOR_RESET " ===========",
                         failed,
                         passed,
                         chr.elapsed);
    status = EXIT_FAILURE;
  }
  else {
    if (!((i32) chr.elapsed)) carbon_println("=========== " CARBON_COLOR_GREEN "%zu passed in %uμs" CARBON_COLOR_RESET " ===========",
                                             passed,
                                             total_time_micro);
    else carbon_println("=========== " CARBON_COLOR_GREEN "%zu passed in %.2fs" CARBON_COLOR_RESET " ===========",
                        passed,
                        chr.elapsed);
  }
  if (!carbon_test_manager__cmd_args.no_output) carbon_junit_output(junit_testcase_infos, carbon_test_manager__cmd_args.output, failed, chr.elapsed);
  carbon_list_destroy(&junit_testcase_infos);
  carbon_test_manager_cleanup(s);
  return status;
}

u8 carbon_test_manager_run(void) {
  return carbon_test_manager_run_s(&carbon_test_manager__test_suite);
}
