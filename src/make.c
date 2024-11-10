// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

#define TESTBIN "./test/carbon"
#define WORKDIR                                         \
  "carbon"                                              \
  "-" CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)     \
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)     \
  "-" CARBON_TARGET_OS                                  \
  "-" CARBON_CPU_ARCH

static const char * const help_msg = "usage: %s [SUBCMD]\n"
  "Subcommands:\n"
  "  help        display this help\n"
  "  clean       remove previously created build artifacts\n"
  "  mrproper    same as `clean` plus remove this binary\n"
  "  check       only run tests\n"
  "\n"
  "If not provided any subcommand, it runs the full build pipeline.\n"
  "\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "%s homepage: <https://github.com/sparky-game/carbon>\n";

static void call_cmd(const char *cmd) {
  if (!system(cmd)) return;
  CARBON_ERROR("Unable to run `%s`", cmd);
  exit(1);
}

static inline void rm_dash_r(const char *path) {
  CARBON_INFO("  RM      %s", path);
  call_cmd(carbon_string_fmt("rm -rf %s", path));
}

static inline void cp_dash_r(const char *origin, const char *dest) {
  CARBON_INFO("  CP      %s -> %s", origin, dest);
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void build_src_files(void) {
  usz c_files_count = 0, cxx_files_count = 0;
  char **c_files = carbon_fs_pattern_match("src/carbon_*.c", &c_files_count);
  char **cxx_files = carbon_fs_pattern_match("src/carbon_*.cc", &cxx_files_count);
  for (usz i = 0; i < c_files_count; ++i) {
    CARBON_INFO("  CC      %s", c_files[i]);
    carbon_string_strip_substr(c_files[i], "src/");
    carbon_string_strip_substr(c_files[i], ".c");
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -I . -std=gnu99 -Wall -Wextra -pipe -Os -c src/%s.c -o %s/%s.o", c_files[i], WORKDIR, c_files[i]));
  }
  for (usz i = 0; i < cxx_files_count; ++i) {
    CARBON_INFO("  CXX     %s", cxx_files[i]);
    carbon_string_strip_substr(cxx_files[i], "src/");
    carbon_string_strip_substr(cxx_files[i], ".cc");
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -I . -std=c++11 -Wall -Wextra -pipe -Os -c src/%s.cc -o %s/%s.o", cxx_files[i], WORKDIR, cxx_files[i]));
  }
}

static void run_tests(void) {
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Running tests...");
  static const char *test_cmd = TESTBIN " -n";
  usz c_files_count = 0, cxx_files_count = 0;
  char **c_files = carbon_fs_pattern_match("test/*.c", &c_files_count);
  char **cxx_files = carbon_fs_pattern_match("test/*.cc", &cxx_files_count);
  for (usz i = 0; i < c_files_count; ++i) {
    CARBON_INFO("  CC      %s", c_files[i]);
    carbon_string_strip_substr(c_files[i], ".c");
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -I . -std=gnu99 -Wall -Wextra -fsanitize=address,undefined -c %s.c -o %s.o", c_files[i], c_files[i]));
  }
  for (usz i = 0; i < cxx_files_count; ++i) {
    CARBON_INFO("  CXX     %s", cxx_files[i]);
    carbon_string_strip_substr(cxx_files[i], ".cc");
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -I . -std=c++11 -Wall -Wextra -fsanitize=address,undefined -c %s.cc -o %s.o", cxx_files[i], cxx_files[i]));
  }
  CARBON_INFO("  LD      " TESTBIN);
  call_cmd("clang++ -fsanitize=address,undefined test/*.o -o " TESTBIN);
  rm_dash_r("test/*.o");
  CARBON_INFO("+ %s", test_cmd);
  call_cmd(test_cmd);
}

static inline void clean(void) {
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r(WORKDIR);
  rm_dash_r(WORKDIR ".tgz");
}

static void build(void) {
  CARBON_INFO("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) {
    clean();
    exit(1);
  }
  build_src_files();
  CARBON_INFO("  AR      libcarbon.a");
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
  rm_dash_r(WORKDIR "/*.o");
}

static void package(void) {
  CARBON_INFO("  MKDIR   " WORKDIR "/src");
  if (!carbon_fs_create_directory(WORKDIR "/src")) {
    clean();
    exit(1);
  }
  cp_dash_r("COPYING.BSD COPYING.GPL carbon.h", WORKDIR);
  cp_dash_r("src/carbon_*.c src/carbon_*.cc", WORKDIR "/src");
  CARBON_INFO("  GZIP    " WORKDIR ".tgz");
  call_cmd("tar -zcf " WORKDIR ".tgz " WORKDIR);
  rm_dash_r(WORKDIR);
}

int main(int argc, char **argv) {
  if (!carbon_fs_change_directory(carbon_fs_get_bin_directory())) {
    CARBON_ERROR("Unable to change CWD to binary's directory");
    return 1;
  }
  if (argc == 2) {
    if (!carbon_string_cmp(argv[1], "help")) {
      CARBON_INFO_RAW(help_msg, argv[0], CARBON_NAME);
      return 0;
    }
    else if (!carbon_string_cmp(argv[1], "clean")) {
      clean();
      return 0;
    }
    else if (!carbon_string_cmp(argv[1], "mrproper")) {
      clean();
      rm_dash_r(argv[0]);
      return 0;
    }
    else if (!carbon_string_cmp(argv[1], "check")) {
      run_tests();
      return 0;
    }
    else {
      CARBON_ERROR("unrecognized option\nTry '%s help' for more information.", argv[0]);
      return 1;
    }
  }
  else if (argc > 2) {
    CARBON_ERROR("unrecognized option\nTry '%s help' for more information.", argv[0]);
    return 1;
  }
  run_tests();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Building and packaging...");
  build();
  package();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output: " WORKDIR ".tgz is ready");
  return 0;
}
