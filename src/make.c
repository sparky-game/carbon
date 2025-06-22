// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"
// Needed sources for bootstrapping purposes
#include "carbon_assert.c"
#include "carbon_memory.c"
#include "carbon_math.c"
#include "carbon_log.c"
#include "carbon_time.c"
#include "carbon_list.c"
#include "carbon_string.c"
#include "carbon_strbuilder.c"
#include "carbon_strview.c"
#include "carbon_strlist.c"
#include "carbon_fs.c"

#define TESTBIN "./test/testentry"
#define WORKDIR "carbon-" CARBON_VERSION_RAW "-" CARBON_TARGET_OS "-" CARBON_CPU_ARCH

#define C_STD   "-std=c11"
#define CXX_STD "-std=c++17"
#define WARNS   "-Wall -Wextra -Wswitch-enum -Werror=format -Wno-return-type-c-linkage"

static const char * const help_msg = "usage: %s [FLAG...] [SUBCMD]\n"
  "\n"
  "Flags:\n"
  "  -B, --always-bootstrap\n"
  "       Unconditionally re-bootstrap the build system.\n"
  "\n"
  "Subcommands:\n"
  "  help        display this help\n"
  "  clean       remove previously created build artifacts\n"
  "  mrproper    same as `clean` plus remove this binary\n"
  "  build       just run the `BUILDING` stage\n"
  "  test        just run the `TESTING` stage\n"
  "  examples    just run the `EXAMPLES` stage\n"
  "\n"
  "If not provided any subcommand, it runs the full build pipeline:\n"
  "  [BUILDING] -> [TESTING] -> [EXAMPLES] -> [PACKAGING]\n"
  "\n"
  "If compiled with `CARBON_MAKE_USE_SANITIZERS`, tests will run with sanitizers enabled.\n"
  "\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "%s homepage: <https://github.com/sparky-game/carbon>\n";

// TODO: migrate this func to the lib itself
static void call_cmd(const char *cmd) {
  if (!system(cmd)) return;
  carbon_log_error("Failed to run `%s`", cmd);
  CARBON_UNREACHABLE;
}

// TODO: migrate this func to the lib itself
static void rm_dash_r(const char *path) {
  carbon_println("  RM      %s", path);
  call_cmd(carbon_string_fmt("rm -rf %s", path));
}

// TODO: migrate this func to the lib itself
static void cp_dash_r(const char *origin, const char *dest) {
  carbon_println("  CP      %s -> %s", origin, dest);
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void bootstrap_myself(char * const *host_argv, u8 force) {
  const char *bin = host_argv[0];
#ifdef CARBON_MAKE_ALREADY_REBUILT
  if (!force && carbon_fs_mtime(__FILE__) <= carbon_fs_mtime(bin)) return;
#endif
  i32 bootstrap_status_code = 0;
  pid_t bootstrap_child_pid = fork();
  if (bootstrap_child_pid == -1) {
    carbon_log_error("unable to fork child process");
    CARBON_UNREACHABLE;
  }
  else if (bootstrap_child_pid == 0) {
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
      "-o", (char *) bin, 0
    };
    carbon_println("  CCLD    %s", __FILE__);
    if (-1 == execvp(argv[0], argv)) {
      carbon_log_error("unable to execvp from child process");
      CARBON_UNREACHABLE;
    }
  }
  else waitpid(bootstrap_child_pid, &bootstrap_status_code, 0);
  if (bootstrap_status_code != 0) {
    carbon_log_error("errors detected during bootstrap");
    CARBON_UNREACHABLE;
  }
  carbon_println("  EXEC    %s", bin);
  if (-1 == execvp(bin, host_argv)) {
    carbon_log_error("unable to execvp rebuilt binary");
    CARBON_UNREACHABLE;
  }
}

static void clean(void) {
  rm_dash_r("carbon.h");
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r("examples/*.bin");
  rm_dash_r("examples/*.bin.old");
  rm_dash_r("examples/*.png");
  rm_dash_r("carbon-*/");
  rm_dash_r("carbon-*.tgz");
}

static void hdrgen(void) {
  carbon_println("  GEN     carbon.h");
  call_cmd("cat carbon.h.in > carbon.h");
  // TODO: hardcoding this here is rubish; need to produce this ordered list somehow.
  const char *hdrs[] = {
    "src/carbon_deps.h",       "src/carbon_defs.h",       "src/carbon_types.h",        "src/carbon_version.h",
    "src/carbon_test_entry.h", "src/carbon_assert.h",     "src/carbon_memory.h",       "src/carbon_coroutine.h",
    "src/carbon_math.h",       "src/carbon_crypto.h",     "src/carbon_log.h",          "src/carbon_should.h",
    "src/carbon_time.h",       "src/carbon_clock.h",      "src/carbon_list.h",         "src/carbon_circularqueue.h",
    "src/carbon_hashmap.h",    "src/carbon_string.h",     "src/carbon_strview.h",      "src/carbon_strbuilder.h",
    "src/carbon_strlist.h",    "src/carbon_drawcanvas.h", "src/carbon_fs.h",           "src/carbon_net.h",
    "src/carbon_nn.h",         "src/carbon_win.h",        "src/carbon_test_manager.h", "src/carbon_junit.h",
    "src/carbon_skap.h",       "src/carbon_aliases.h"
  };
  for (usz i = 0; i < CARBON_ARRAY_LEN(hdrs); ++i) {
    call_cmd("echo >> carbon.h");
    call_cmd(carbon_string_fmt("cat %s >> carbon.h", hdrs[i]));
  }
}

static void test(void) {
  carbon_log_info("Running tests...");
  CBN_PatternMatchedFiles c_files = carbon_fs_pattern_match("test/*.c");
  CBN_PatternMatchedFiles cxx_files = carbon_fs_pattern_match("test/*.cc");
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CC      %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c %s -o %s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXX     %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c %s -o %s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_println("  LD      " TESTBIN);
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " ");
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
  carbon_strbuilder_add_cstr(&cmd, "test/*.o " WORKDIR "/libcarbon.a -o " TESTBIN);
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r("test/*.o");
  carbon_println("+ " TESTBIN " -n");
  call_cmd(TESTBIN " -n");
}

static void build(void) {
  carbon_log_info("Building...");
  carbon_println("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) CARBON_UNREACHABLE;
  CBN_PatternMatchedFiles c_files    = carbon_fs_pattern_match("src/carbon_*.c");
  CBN_PatternMatchedFiles cxx_files  = carbon_fs_pattern_match("src/carbon_*.cc");
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CC      %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIC ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_string_strip_substr(it.f, "src/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -c src/%s -o " WORKDIR "/%s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXX     %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIC ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_string_strip_substr(it.f, "src/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -c src/%s -o " WORKDIR "/%s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_println("  AR      libcarbon.a");
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
  carbon_println("  LD      libcarbon.so");
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " ");
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
  carbon_strbuilder_add_cstr(&cmd, WORKDIR "/*.o -shared ");
#ifdef __APPLE__
  carbon_strbuilder_add_cstr(&cmd, "-framework CoreFoundation -lobjc ");
#endif
  carbon_strbuilder_add_cstr(&cmd, " -o " WORKDIR "/libcarbon.so");
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r(WORKDIR "/*.o");
}

static void examples(void) {
  carbon_log_info("Building examples...");
  CBN_PatternMatchedFiles c_files = carbon_fs_pattern_match("examples/*.c");
  CBN_PatternMatchedFiles cxx_files = carbon_fs_pattern_match("examples/*.cc");
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CCLD    %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, it.f);
    carbon_string_strip_substr(it.f, ".c");
    carbon_strbuilder_add_cstr(&cmd, " " WORKDIR "/libcarbon.a ");
#ifdef __APPLE__
    carbon_strbuilder_add_cstr(&cmd, "-framework CoreFoundation -lobjc ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o %s.bin", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXXLD   %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, it.f);
    carbon_string_strip_substr(it.f, ".cc");
    carbon_strbuilder_add_cstr(&cmd, " " WORKDIR "/libcarbon.a ");
#ifdef __APPLE__
    carbon_strbuilder_add_cstr(&cmd, "-framework CoreFoundation -lobjc ");
#endif
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o %s.bin", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void package(void) {
  carbon_log_info("Packaging...");
  cp_dash_r("COPYING carbon.h", WORKDIR);
  carbon_println("  GZIP    " WORKDIR ".tgz");
  call_cmd("tar -zcf " WORKDIR ".tgz " WORKDIR);
  carbon_log_info(WORKDIR ".tgz is ready");
}

static void full_pipeline(void) {
  hdrgen();
  build();
  test();
  examples();
  package();
}

int main(int argc, char **argv) {
  if (!carbon_fs_change_directory(carbon_fs_get_bin_directory())) CARBON_UNREACHABLE;
  bootstrap_myself(argv, false);
#ifdef CARBON_MAKE_ALREADY_REBUILT
  carbon_log_info(CARBON_NAME " " CARBON_VERSION " (" CARBON_COMPILER_VERSION ") " __DATE__ " " __TIME__);
#endif
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_log_debug("Compile-time option `CARBON_MAKE_USE_SANITIZERS` is enabled");
#endif
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (argc > 2) {
    carbon_log_error("ill-formed command\nTry '%s help' for more information.", program_name);
    return 1;
  }
  if (argc == 2) {
    const char *flag = CARBON_SHIFT_ARGS(argc, argv);
    const char *subcmd = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_string_cmp(flag, "-B") || !carbon_string_cmp(flag, "--always-bootstrap")) {
      const char *new_argv[] = { program_name, subcmd, 0 };
      bootstrap_myself((char **) new_argv, true);
    }
    carbon_log_error("unrecognized option\nTry '%s help' for more information.", program_name);
    return 1;
  }
  if (argc == 1) {
    const char *flag = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_string_cmp(flag, "-B") || !carbon_string_cmp(flag, "--always-bootstrap")) {
      const char *new_argv[] = { program_name, 0 };
      bootstrap_myself((char **) new_argv, true);
    }
    const char *subcmd = flag;
    if (!carbon_string_cmp(subcmd, "help")) {
      carbon_print(help_msg, program_name, CARBON_NAME);
      return 0;
    }
    if (!carbon_string_cmp(subcmd, "clean")) {
      clean();
      return 0;
    }
    if (!carbon_string_cmp(subcmd, "mrproper")) {
      clean();
      rm_dash_r(program_name);
      return 0;
    }
    if (!carbon_string_cmp(subcmd, "build")) {
      hdrgen();
      build();
      return 0;
    }
    if (!carbon_string_cmp(subcmd, "test")) {
      hdrgen();
      test();
      return 0;
    }
    if (!carbon_string_cmp(subcmd, "examples")) {
      hdrgen();
      examples();
      return 0;
    }
    carbon_log_error("unrecognized option\nTry '%s help' for more information.", program_name);
    return 1;
  }
  full_pipeline();
  return 0;
}
