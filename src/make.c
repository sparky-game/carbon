// SPDX-License-Identifier: AGPL-3.0-only
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

#define C_STD   "-std=c99"
#define CXX_STD "-std=c++11"
#define WARNS   "-Wall -Wextra -Wswitch-enum -Werror=format"

static const char * const help_msg = "usage: %s [SUBCMD]\n"
  "Subcommands:\n"
  "  help        display this help\n"
  "  clean       remove previously created build artifacts\n"
  "  mrproper    same as `clean` plus remove this binary\n"
  "  check       only run tests\n"
  "\n"
  "If not provided any subcommand, it runs the full build pipeline.\n"
  "If compiled with `CARBON_MAKE_USE_SANITIZERS`, tests will run with sanitizers enabled.\n"
  "\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "%s homepage: <https://github.com/sparky-game/carbon>\n";

static inline void call_cmd(const char *cmd) {
  if (!system(cmd)) return;
  CARBON_ERROR("Unable to run `%s`", cmd);
  exit(1);
}

static inline void rm_dash_r(const char *path) {
  carbon_println("  RM      %s", path);
  call_cmd(carbon_string_fmt("rm -rf %s", path));
}

static inline void cp_dash_r(const char *origin, const char *dest) {
  carbon_println("  CP      %s -> %s", origin, dest);
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void clean(void) {
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r(WORKDIR);
  rm_dash_r(WORKDIR ".tgz");
}

static void exit_gracefully(void) {
  clean();
  exit(1);
}

static void rebuild_myself(const char **host_argv) {
  const char *bin = host_argv[0];
#ifdef CARBON_MAKE_ALREADY_REBUILT
  if (carbon_fs_mtime(__FILE__) <= carbon_fs_mtime(bin)) return;
#endif
  i32 rebuild_status_code = 0;
  pid_t rebuild_child_pid = fork();
  if (rebuild_child_pid == -1) {
    CARBON_ERROR("unable to fork child process");
    exit(1);
  }
  else if (rebuild_child_pid == 0) {
    char *argv[] = {
      CARBON_C_COMPILER,
      C_STD,
      "-DCARBON_MAKE_ALREADY_REBUILT",
#ifdef CARBON_MAKE_USE_SANITIZERS
      "-DCARBON_MAKE_USE_SANITIZERS",
#endif
      "-Wall", "-Wextra", "-Wswitch-enum", "-Werror=format",
      "-fPIE", "-pipe", "-Os",
      __FILE__,
#ifndef __APPLE__
      "-static", "-Wl,-z,now", "-Wl,-z,relro",
#endif
      "-o", (char *) bin, 0
    };
    carbon_println("  CCLD    %s", __FILE__);
    if (-1 == execvp(argv[0], argv)) {
      CARBON_ERROR("unable to execvp from child process");
      exit(1);
    }
  }
  else waitpid(rebuild_child_pid, &rebuild_status_code, 0);
  if (rebuild_status_code != 0) {
    CARBON_ERROR("errors detected during rebuild");
    exit(1);
  }
  carbon_println("  EXEC    %s", bin);
  if (-1 == execvp(bin, (char **) host_argv)) {
    CARBON_ERROR("unable to execvp rebuilt binary");
    exit(1);
  }
}

static void run_tests(void) {
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Running tests...");
  CBN_PatternMatchedFiles c_files = carbon_fs_pattern_match("test/*.c");
  CBN_PatternMatchedFiles cxx_files = carbon_fs_pattern_match("test/*.cc");
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CC      %s", it.f);
    carbon_string_strip_substr(it.f, ".c");
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c %s.c -o %s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXX     %s", it.f);
    carbon_string_strip_substr(it.f, ".cc");
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c %s.cc -o %s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_println("  LD      " TESTBIN);
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " ");
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
  carbon_strbuilder_add_cstr(&cmd, "test/*.o ");
#if !defined(__APPLE__) && !defined(CARBON_MAKE_USE_SANITIZERS)
  carbon_strbuilder_add_cstr(&cmd, "-static -Wl,-z,now -Wl,-z,relro ");
#endif
  carbon_strbuilder_add_cstr(&cmd, "-o " TESTBIN);
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r("test/*.o");
  carbon_println("+ " TESTBIN " -n");
  call_cmd(TESTBIN " -n");
}

static void build(void) {
  carbon_println("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) exit_gracefully();
  CBN_PatternMatchedFiles c_files    = carbon_fs_pattern_match("src/carbon_*.c");
  CBN_PatternMatchedFiles cxx_files  = carbon_fs_pattern_match("src/carbon_*.cc");
  CBN_PatternMatchedFiles objc_files = carbon_fs_pattern_match("src/carbon_*.m");
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CC      %s", it.f);
    carbon_string_strip_substr(it.f, "src/");
    carbon_string_strip_substr(it.f, ".c");
    call_cmd(carbon_string_fmt(CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIC -pipe -Os -c src/%s.c -o %s/%s.o", it.f, WORKDIR, it.f));
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXX     %s", it.f);
    carbon_string_strip_substr(it.f, "src/");
    carbon_string_strip_substr(it.f, ".cc");
    call_cmd(carbon_string_fmt(CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIC -pipe -Os -c src/%s.cc -o %s/%s.o", it.f, WORKDIR, it.f));
  }
  carbon_fs_pattern_match_foreach(objc_files) {
    carbon_println("  OBJC    %s", it.f);
    carbon_string_strip_substr(it.f, "src/");
    carbon_string_strip_substr(it.f, ".m");
    call_cmd(carbon_string_fmt(CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIC -pipe -Os -c src/%s.m -o %s/%s.o", it.f, WORKDIR, it.f));
  }
  carbon_println("  AR      libcarbon.a");
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
  carbon_println("  LD      libcarbon.so");
  CBN_StrBuilder cmd = {0};
  carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -pipe -Os " WORKDIR "/*.o -shared ");
#ifdef __APPLE__
  carbon_strbuilder_add_cstr(&cmd, "-framework Cocoa ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-Wl,-z,now -Wl,-z,relro ");
#endif
  carbon_strbuilder_add_cstr(&cmd, "-o " WORKDIR "/libcarbon.so");
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r(WORKDIR "/*.o");
}

static void package(void) {
  cp_dash_r("COPYING carbon.h", WORKDIR);
#undef dir
#define dir "src"
  carbon_println("  MKDIR   " WORKDIR "/" dir);
  if (!carbon_fs_create_directory(WORKDIR "/" dir)) exit_gracefully();
  cp_dash_r(dir "/carbon_*", WORKDIR "/" dir);
#undef dir
#define dir "vendor/stb_image"
  carbon_println("  MKDIR   " WORKDIR "/" dir);
  if (!carbon_fs_create_directories(WORKDIR "/" dir)) exit_gracefully();
  cp_dash_r(dir "/*", WORKDIR "/" dir);
#undef dir
#define dir "vendor/stb_image_write"
  carbon_println("  MKDIR   " WORKDIR "/" dir);
  if (!carbon_fs_create_directories(WORKDIR "/" dir)) exit_gracefully();
  cp_dash_r(dir "/*", WORKDIR "/" dir);
#undef dir
  carbon_println("  GZIP    " WORKDIR ".tgz");
  call_cmd("tar -zcf " WORKDIR ".tgz " WORKDIR);
  rm_dash_r(WORKDIR);
}

int main(int argc, char **argv) {
  if (!carbon_fs_change_directory(carbon_fs_get_bin_directory())) {
    CARBON_ERROR("Unable to change CWD to binary's directory");
    return 1;
  }
  rebuild_myself((const char **) argv);
#ifdef CARBON_MAKE_ALREADY_REBUILT
  CARBON_WARNING(CARBON_NAME " " CARBON_VERSION " (" CARBON_COMPILER_VERSION ") " __DATE__ " " __TIME__);
#endif
#ifdef CARBON_MAKE_USE_SANITIZERS
  CARBON_WARNING("Compile-time option `CARBON_MAKE_USE_SANITIZERS` is enabled");
#endif
  if (argc > 2) {
    CARBON_ERROR("unrecognized option\nTry '%s help' for more information.", argv[0]);
    return 1;
  }
  if (argc == 2) {
    if (!carbon_string_cmp(argv[1], "help")) {
      carbon_print(help_msg, argv[0], CARBON_NAME);
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
  run_tests();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Building and packaging...");
  build();
  package();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output: " WORKDIR ".tgz is ready");
  return 0;
}
