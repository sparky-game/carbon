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
static i32 call_cmd(const char *cmd) {
  i32 status_code = system(cmd);
  CARBON_ASSERT(status_code != -1 && "system(3) error during fork/waitpid operations");
  CARBON_ASSERT(status_code != 127 && "system(3) error during shell execution");
  return status_code;
}

// TODO: migrate this func to the lib itself
static void call_cmd_or_panic(const char *cmd) {
  if (!system(cmd)) return;
  carbon_log_error("Failed to run `%s`", cmd);
  CARBON_UNREACHABLE;
}

// TODO: migrate this func to the lib itself
static void rm_dash_r(const char *path) {
  carbon_println("  RM      %s", path);
  call_cmd_or_panic(carbon_string_fmt("rm -rf %s", path));
}

// TODO: migrate this func to the lib itself
static void cp_dash_r(const char *origin, const char *dest) {
  carbon_println("  CP      %s -> %s", origin, dest);
  call_cmd_or_panic(carbon_string_fmt("cp -r %s %s", origin, dest));
}

// TODO: migrate this func to the lib itself
static void exec_cmd(char * const *argv) {
  CARBON_ASSERT(-1 != execvp(argv[0], argv) && "Failed to execvp");
}

// TODO: migrate this func to the lib itself
static i32 fork_and_exec_cmd(char * const *argv) {
  i32 status_code = 0;
  pid_t child_pid = fork();
  CARBON_ASSERT(child_pid != -1 && "Failed to fork process");
  if (child_pid == 0) exec_cmd(argv);
  else waitpid(child_pid, &status_code, 0);
  return status_code;
}

static void bootstrap(char * const *host_argv, u8 force) {
#ifdef CARBON_MAKE_ALREADY_REBUILT
  if (!force && carbon_fs_mtime(__FILE__) <= carbon_fs_mtime(host_argv[0])) return;
#endif
  char *argv[] = {
    CARBON_C_COMPILER,
    C_STD,
    "-DCARBON_MAKE_ALREADY_REBUILT",
    "-DCARBON_C_COMPILER=\"" CARBON_C_COMPILER "\"",
    "-DCARBON_CXX_COMPILER=\"" CARBON_CXX_COMPILER "\"",
    "-DCARBON_AR_TOOL=\"" CARBON_AR_TOOL "\"",
#ifdef CARBON_MAKE_USE_SANITIZERS
    "-DCARBON_MAKE_USE_SANITIZERS",
#endif
    "-Wall", "-Wextra", "-Wswitch-enum", "-Werror=format",
    "-fPIE", "-pipe", "-Os",
    __FILE__,
    "-o", host_argv[0], 0
  };
  carbon_println("  CCLD    %s", __FILE__);
  CARBON_ASSERT(0 == fork_and_exec_cmd(argv) && "Errors detected during bootstrap");
  carbon_println("  EXEC    %s", host_argv[0]);
  exec_cmd(host_argv);
}

static void clean(void) {
  rm_dash_r("carbon.h");
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r("examples/*.bin");
  rm_dash_r("examples/*.bin.old");
  rm_dash_r("examples/*.png");
  rm_dash_r("examples/*.wasm");
  rm_dash_r("carbon-*/");
  rm_dash_r("carbon-*.tgz");
}

static void hdrgen(void) {
  carbon_println("  GEN     carbon.h");
  call_cmd_or_panic("cat carbon.h.in > carbon.h");
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
    call_cmd_or_panic("echo >> carbon.h");
    call_cmd_or_panic(carbon_string_fmt("cat %s >> carbon.h", hdrs[i]));
  }
}

static void try_build_wasm_lib(void) {
  carbon_log_info("Trying to build for WASM...");
  const char *files[] = {
    "carbon_memory",
    "carbon_math",
    "carbon_drawcanvas"
  };
  i32 status = 0;
  CBN_StrBuilder cmd = {0};
  for (usz i = 0; i < CARBON_ARRAY_LEN(files); ++i) {
    const char *f = files[i];
    const char *full_f = carbon_string_fmt("src/%s.c", f);
    carbon_println("  CC      %s", full_f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " --target=wasm32 -I . " C_STD " " WARNS " -fPIC -pipe -Os ");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -c %s -o " WORKDIR "/%s.wasm.o", full_f, f));
    status |= call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  if (status != 0) {
    carbon_log_error("Unable to build for WASM. Skipping it...");
    return;
  }
  carbon_println("  AR      libcarbon.wasm.a");
  call_cmd_or_panic(CARBON_AR_TOOL " -rcs " WORKDIR "/libcarbon.wasm.a " WORKDIR "/*.wasm.o");
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_println("  AR      libcarbon.a");
  call_cmd_or_panic(CARBON_AR_TOOL " -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
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
  call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  // WASM build
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_log_warn("Building for WASM has been disabled due to `CARBON_MAKE_USE_SANITIZERS` being defined");
#else
  try_build_wasm_lib();
#endif
  rm_dash_r(WORKDIR "/*.o");
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
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
  call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r("test/*.o");
  carbon_println("+ " TESTBIN " -n");
  call_cmd_or_panic(TESTBIN " -n");
}

static void try_build_wasm_examples(CBN_PatternMatchedFiles c_files, CBN_PatternMatchedFiles cxx_files) {
  carbon_log_info("Trying to build examples for WASM...");
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CCLD    %s.wasm", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " --target=wasm32 -I . " C_STD " " WARNS " -fPIE -pipe -Os ");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("%s.c ", it.f));
    carbon_strbuilder_add_cstr(&cmd, WORKDIR "/libcarbon.wasm.a -nostdlib -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=main ");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o %s.wasm", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXXLD   %s.wasm", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " --target=wasm32 -I . " CXX_STD " " WARNS " -fPIE -pipe -Os ");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("%s.cc ", it.f));
    carbon_strbuilder_add_cstr(&cmd, WORKDIR "/libcarbon.wasm.a -nostdlib -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=main ");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o %s.wasm", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
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
    call_cmd_or_panic(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
#ifndef CARBON_MAKE_USE_SANITIZERS
  try_build_wasm_examples(c_files, cxx_files);
#else
  carbon_log_warn("Building examples for WASM has been disabled due to `CARBON_MAKE_USE_SANITIZERS` being enabled");
#endif
}

static void package(void) {
  carbon_log_info("Packaging...");
  cp_dash_r("COPYING carbon.h", WORKDIR);
  carbon_println("  GZIP    " WORKDIR ".tgz");
  call_cmd_or_panic("tar -zcf " WORKDIR ".tgz " WORKDIR);
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
  bootstrap(argv, false);
#ifdef CARBON_MAKE_ALREADY_REBUILT
  carbon_log_info(CARBON_NAME " " CARBON_VERSION " (" CARBON_COMPILER_VERSION ") " __DATE__ " " __TIME__);
#endif
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_log_debug("CARBON_MAKE_USE_SANITIZERS := enabled");
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
      bootstrap((char **) new_argv, true);
    }
    carbon_log_error("unrecognized option\nTry '%s help' for more information.", program_name);
    return 1;
  }
  if (argc == 1) {
    const char *flag = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_string_cmp(flag, "-B") || !carbon_string_cmp(flag, "--always-bootstrap")) {
      const char *new_argv[] = { program_name, 0 };
      bootstrap((char **) new_argv, true);
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
