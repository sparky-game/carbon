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

#define HDRFILE "carbon.h"
#define TESTBIN "./test/testentry"
#define WORKDIR "carbon-" CARBON_VERSION_RAW "-" CARBON_TARGET_OS "-" CARBON_CPU_ARCH

#define C_STD   "-std=c11"
#define CXX_STD "-std=c++20"
#define WARNS   "-Wall -Wextra -Werror=switch-enum -Werror=format -Werror=return-type -Wno-return-type-c-linkage"

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
  CBN_ERROR("Failed to run `%s`", cmd);
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

// TODO: migrate this func to the lib itself
static void exec_cmd(char * const *argv) {
  CBN_ASSERT(-1 != execvp(argv[0], argv) && "Failed to execvp");
}

// TODO: migrate this func to the lib itself
static i32 fork_and_exec_cmd(char * const *argv) {
#ifdef _WIN32
  CBN_StrBuilder cmd = {0};
  carbon_strbuilder_add_cstr(&cmd, argv[0]);
  for (usz i = 1; argv[i]; ++i) {
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" \"%s\"", argv[i]));
  }
  carbon_strbuilder_add_null(&cmd);
  carbon_println("[CMD]: `%.*s`", cmd.size, cmd.items);
  STARTUPINFOA si = { .cb = sizeof(si) };
  PROCESS_INFORMATION pi;
  CBN_ASSERT(CreateProcessA(0, cmd.items, 0, 0, FALSE, 0, 0, 0, &si, &pi) && "Failed to create process");
  carbon_strbuilder_free(&cmd);
  WaitForSingleObject(pi.hProcess, INFINITE);
  DWORD status_code = 0;
  GetExitCodeProcess(pi.hProcess, &status_code);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
#else
  i32 status_code = 0;
  pid_t child_pid = fork();
  CBN_ASSERT(child_pid != -1 && "Failed to fork process");
  if (child_pid == 0) exec_cmd(argv);
  else waitpid(child_pid, &status_code, 0);
#endif
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
#ifdef CARBON_MAKE_USE_SANITIZERS
    "-DCARBON_MAKE_USE_SANITIZERS",
#endif
    "-Wall", "-Wextra", "-Wswitch-enum", "-Werror=format",
    "-fPIE", "-pipe", "-Os",
    __FILE__,
    "-o", host_argv[0], 0
  };
  carbon_println("  CCLD    %s", __FILE__);
  CBN_ASSERT(0 == fork_and_exec_cmd(argv) && "Errors detected during bootstrap");
  carbon_println("  EXEC    %s", host_argv[0]);
  exec_cmd(host_argv);
}

static void clean(void) {
  rm_dash_r(HDRFILE);
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r("examples/*.bin");
  rm_dash_r("examples/*.bin.old");
  rm_dash_r("examples/*.png");
  rm_dash_r(WORKDIR);
  rm_dash_r(WORKDIR ".tgz");
}

static void hdrgen(void) {
  carbon_println("  GEN     " HDRFILE);
  CBN_StrBuilder hdr = {0};
  CBN_ASSERT(carbon_fs_read_entire_file(&hdr, HDRFILE ".in"));
  // TODO: hardcoding this here is rubish; need to produce this ordered list somehow.
  // I supose we should read the `carbon.inc` file, and extract the included file from there.
  const char *hdrs[] = {
    "src/carbon_deps.h",         "src/carbon_defs.h",       "src/carbon_types.h",        "src/carbon_version.h",
    "src/carbon_test_entry.h",   "src/carbon_assert.h",     "src/carbon_memory.h",       "src/carbon_coroutine.h",
    "src/carbon_math.h",         "src/carbon_crypto.h",     "src/carbon_log.h",          "src/carbon_should.h",
    "src/carbon_time.h",         "src/carbon_clock.h",      "src/carbon_list.h",         "src/carbon_circularqueue.h",
    "src/carbon_hashmap.h",      "src/carbon_slotmap.h",    "src/carbon_string.h",       "src/carbon_strview.h",
    "src/carbon_strbuilder.h",   "src/carbon_strlist.h",    "src/carbon_drawcanvas.h",   "src/carbon_fs.h",
    "src/carbon_net.h",          "src/carbon_audio.h",      "src/carbon_nn.h",           "src/carbon_win.h",
    "src/carbon_test_manager.h", "src/carbon_junit.h",      "src/carbon_skap.h"
  };
  for (usz i = 0; i < CARBON_ARRAY_LEN(hdrs); ++i) {
    carbon_strbuilder_add_cstr(&hdr, "\n");
    CBN_ASSERT(carbon_fs_read_entire_file(&hdr, hdrs[i]));
  }
  carbon_strbuilder_add_cstr(&hdr, "\n");
  CBN_ASSERT(carbon_fs_read_entire_file(&hdr, "src/carbon_aliases.h"));
  FILE *hdr_fd = fopen(HDRFILE, "w");
  fwrite(hdr.items, hdr.size, 1, hdr_fd);
  fclose(hdr_fd);
  carbon_strbuilder_free(&hdr);
}

static void test(void) {
  CBN_INFO("Running tests...");
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

static void build_compile_c_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match("src/carbon_*.c");
  carbon_fs_pattern_match_foreach(files) {
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
}

static void build_compile_cxx_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match("src/carbon_*.cc");
  carbon_fs_pattern_match_foreach(files) {
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
}

static void build_static_lib(void) {
  carbon_println("  AR      libcarbon.a");
#ifndef _WIN32
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
#else
  CBN_StrBuilder cmd = {0};
  carbon_strbuilder_add_cstr(&cmd, "ar -rcs " WORKDIR "/libcarbon.a ");
  CBN_PatternMatchedFiles o_files = carbon_fs_pattern_match(WORKDIR "/*.o");
  carbon_fs_pattern_match_foreach(o_files) {
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(WORKDIR "/%s ", it.f));
  }
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
#endif
}

static void build_shared_lib(void) {
  CBN_StrBuilder cmd = {0};
#ifdef _WIN32
  carbon_println("  LD      libcarbon.dll");
#else
  carbon_println("  LD      libcarbon.so");
#endif
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " ");
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined -g ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
#ifndef _WIN32
  carbon_strbuilder_add_cstr(&cmd, WORKDIR "/*.o -shared ");
#else
  CBN_PatternMatchedFiles o_files = carbon_fs_pattern_match(WORKDIR "/*.o");
  carbon_fs_pattern_match_foreach(o_files) {
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(WORKDIR "/%s ", it.f));
  }
  carbon_strbuilder_add_cstr(&cmd, "-shared -static ");
#endif
#if defined(__APPLE__)
  carbon_strbuilder_add_cstr(&cmd, "-framework CoreFoundation -lobjc ");
#elif defined (_WIN32)
  carbon_strbuilder_add_cstr(&cmd, "-ldnsapi -lgdi32 -lntdll ");
#endif
#ifdef _WIN32
  carbon_strbuilder_add_cstr(&cmd, "-o " WORKDIR "/libcarbon.dll");
#else
  carbon_strbuilder_add_cstr(&cmd, "-o " WORKDIR "/libcarbon.so");
#endif
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
}

static void build(void) {
  CBN_INFO("Building...");
  carbon_println("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) CARBON_UNREACHABLE;
  build_compile_c_files();
  build_compile_cxx_files();
  build_static_lib();
  build_shared_lib();
  rm_dash_r(WORKDIR "/*.o");
}

static void examples(void) {
  CBN_INFO("Building examples...");
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
  CBN_INFO("Packaging...");
  cp_dash_r("COPYING " HDRFILE, WORKDIR);
  carbon_println("  GZIP    " WORKDIR ".tgz");
  call_cmd("tar -zcf " WORKDIR ".tgz " WORKDIR);
  CBN_INFO(WORKDIR ".tgz is ready");
}

int main(int argc, char **argv) {
  CBN_ASSERT(!carbon_string_cmp(carbon_fs_get_curr_directory(), carbon_fs_get_bin_directory()) && "Need to be in root dir");
#ifndef _WIN32
  bootstrap(argv, false);
#endif
#ifdef CARBON_MAKE_ALREADY_REBUILT
  CBN_INFO(CARBON_NAME " " CARBON_VERSION " (" CARBON_COMPILER_VERSION ") " __DATE__ " " __TIME__);
#endif
#ifdef CARBON_MAKE_USE_SANITIZERS
  CBN_DEBUG("Compile-time option `CARBON_MAKE_USE_SANITIZERS` is enabled");
#endif
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (argc > 2) {
    CBN_ERROR("ill-formed command\nTry '%s help' for more information.", program_name);
    return 1;
  }
  if (argc == 2) {
    const char *flag = CARBON_SHIFT_ARGS(argc, argv);
    const char *subcmd = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_string_cmp(flag, "-B") || !carbon_string_cmp(flag, "--always-bootstrap")) {
      const char *new_argv[] = { program_name, subcmd, 0 };
      bootstrap((char **) new_argv, true);
    }
    CBN_ERROR("unrecognized option\nTry '%s help' for more information.", program_name);
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
#ifndef _WIN32
      rm_dash_r(program_name);
#endif
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
    CBN_ERROR("unrecognized option\nTry '%s help' for more information.", program_name);
    return 1;
  }
  hdrgen();
  build();
  test();
  examples();
  package();
  return 0;
}
