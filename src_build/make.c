// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "../src/carbon.inc"
// Needed sources for bootstrapping purposes
#include "../src/carbon_assert.c"
#include "../src/carbon_memory.c"
#include "../src/carbon_math.c"
#include "../src/carbon_log.c"
#include "../src/carbon_time.c"
#include "../src/carbon_list.c"
#include "../src/carbon_string.c"
#include "../src/carbon_strbuilder.c"
#include "../src/carbon_strview.c"
#include "../src/carbon_strlist.c"
#include "../src/carbon_fs.c"
#include "../src/carbon_skap.c"
#include "../src/carbon_crypto.c"
#include "../src/carbon_hashmap.c"

#define HDRFILE "carbon.h"
#define SRCDIR "src"
#define TESTDIR "test"
#define TESTBIN "./" TESTDIR "/testentry"
#define WORKDIR "carbon-" CARBON_VERSION_RAW "-" CARBON_TARGET_OS "-" CARBON_CPU_ARCH

#define C_STD   "-std=c11"
#define CXX_STD "-std=c++20"
#define WARNS   "-Wall -Wextra -Werror=switch-enum -Werror=format -Werror=return-type -Wno-return-type-c-linkage -Wno-strict-aliasing"

#ifndef CARBON_MAKE_DEBUG
#define OPTIMIZATIONS "-ffp-contract=off -pipe -O3 "
#else
#define OPTIMIZATIONS "-ffp-contract=off -fsanitize=address,undefined -g "
#endif

#if defined(__APPLE__)
#define LIBS "-framework CoreFoundation -lobjc "
#elif defined (_WIN32)
#define LIBS "-ldnsapi -lgdi32 -lntdll -static "
#else
#define LIBS ""
#endif

#ifdef _WIN32
#define SHARED_LIB_FILE "libcarbon.dll"
#else
#define SHARED_LIB_FILE "libcarbon.so"
#endif

static const char * const help_msg = "usage: %s [FLAG...] [SUBCMD]\n"
  "\n"
  "Flags:\n"
  "  -B, --always-bootstrap\n"
  "       Unconditionally re-bootstrap the build system.\n"
  "\n"
  "Subcommands:\n"
  "  help        display this help\n"
  "  version     display version information\n"
  "  clean       remove previously created build artifacts\n"
  "  mrproper    same as `clean` plus remove this binary\n"
  "  build       just run the `BUILDING` stage\n"
  "  test        just run the `TESTING` stage\n"
  "  examples    just run the `EXAMPLES` stage\n"
  "\n"
  "If not provided any subcommand, it runs the full build pipeline:\n"
  "  [BUILDING] -> [TESTING] -> [EXAMPLES] -> [PACKAGING]\n"
  "\n"
  "If compiled with `CARBON_MAKE_DEBUG`, it will build and run in debug mode.\n"
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
#ifdef _WIN32
  CBN_WARN("Bootstrapping is not supported on Windows");
  return;
#endif
#ifdef CARBON_MAKE_ALREADY_REBUILT
  if (!force && carbon_fs_mtime(__FILE__) <= carbon_fs_mtime(host_argv[0])) return;
#endif
  char *argv[] = {
    CARBON_C_COMPILER,
    C_STD,
    "-DCARBON_MAKE_ALREADY_REBUILT",
#ifdef CARBON_MAKE_DEBUG
    "-DCARBON_MAKE_DEBUG",
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
  rm_dash_r(TESTBIN ".exe");
  rm_dash_r(TESTDIR "/*.o");
  rm_dash_r("examples/*.exe");
  rm_dash_r("examples/*.bin");
  rm_dash_r("examples/*.bin.old");
  rm_dash_r("examples/*.png");
  rm_dash_r("examples/*.skap*");
  rm_dash_r("examples/*.dSYM");
  rm_dash_r(WORKDIR);
  rm_dash_r(WORKDIR ".tgz");
}

static void hdrgen(void) {
  carbon_println("  GEN     " HDRFILE);
  CBN_StrBuilder hdr = {0};
  CBN_ASSERT(carbon_fs_read_entire_file(&hdr, HDRFILE ".in"));
  CBN_StrList hdrs = carbon_strlist_create(true);
  CBN_StrBuilder inc = {0};
  CBN_ASSERT(carbon_fs_read_entire_file(&inc, SRCDIR "/carbon.inc"));
  CBN_StrView inc_sv = carbon_strview_from_strbuilder(&inc);
  while (inc_sv.size) {
    CBN_StrView line = carbon_strview_chop(&inc_sv, '\n');
    if (!carbon_strview_starts_with(line, carbon_strview_from_cstr("#include")) ||
        carbon_strview_are_equal(line, carbon_strview_from_cstr("#include \"../" HDRFILE ".in\""))) continue;
    char *line_str = carbon_strview_to_cstr(line);
    carbon_string_strip_substr(line_str, "#include \"");
    carbon_string_strip_substr(line_str, "\"");
    carbon_strlist_push(&hdrs, carbon_string_fmt(SRCDIR "/%s", line_str));
  }
  carbon_strlist_foreach(hdrs) {
    carbon_strbuilder_add_cstr(&hdr, "\n");
    CBN_ASSERT(carbon_fs_read_entire_file(&hdr, carbon_strview_to_cstr(it.sv)));
  }
  carbon_strbuilder_add_cstr(&hdr, "\n");
  CBN_ASSERT(carbon_fs_read_entire_file(&hdr, SRCDIR "/carbon_aliases.h"));
  FILE *hdr_fd = fopen(HDRFILE, "w");
  fwrite(hdr.items, hdr.size, 1, hdr_fd);
  fclose(hdr_fd);
  carbon_strlist_destroy(&hdrs);
  carbon_strbuilder_free(&hdr);
}

static void build_compile_c_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match(SRCDIR "/carbon_*.c");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CC      %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIC " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, SRCDIR "/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -c " SRCDIR "/%s -o " WORKDIR "/%s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void build_compile_cxx_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match(SRCDIR "/carbon_*.cc");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CXX     %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIC " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, SRCDIR "/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -c " SRCDIR "/%s -o " WORKDIR "/%s.o", it.f, it.f));
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
  carbon_println("  LD      " SHARED_LIB_FILE);
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " " OPTIMIZATIONS);
#ifndef _WIN32
  carbon_strbuilder_add_cstr(&cmd, WORKDIR "/*.o -shared ");
#else
  CBN_PatternMatchedFiles o_files = carbon_fs_pattern_match(WORKDIR "/*.o");
  carbon_fs_pattern_match_foreach(o_files) {
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(WORKDIR "/%s ", it.f));
  }
  carbon_strbuilder_add_cstr(&cmd, "-shared ");
#endif
  carbon_strbuilder_add_cstr(&cmd, LIBS);
  carbon_strbuilder_add_cstr(&cmd, "-o " WORKDIR "/" SHARED_LIB_FILE);
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
#ifndef _WIN32
  // TODO: make this work on Windows (MinGW)...
  rm_dash_r(WORKDIR "/*.o");
#endif
}

static void test_compile_c_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match(TESTDIR "/*.c");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CC      %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, TESTDIR "/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c " TESTDIR "/%s -o " TESTDIR "/%s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void test_compile_cxx_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match(TESTDIR "/*.cc");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CXX     %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, TESTDIR "/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-c " TESTDIR "/%s -o " TESTDIR "/%s.o", it.f, it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void test_link(void) {
  CBN_StrBuilder cmd = {0};
  carbon_println("  LD      " TESTBIN);
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " " OPTIMIZATIONS);
#ifdef _WIN32
  CBN_PatternMatchedFiles o_files = carbon_fs_pattern_match(TESTDIR "/*.o");
  carbon_fs_pattern_match_foreach(o_files) {
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(TESTDIR "/%s ", it.f));
  }
  carbon_strbuilder_add_cstr(&cmd, WORKDIR "/libcarbon.a -static -o " TESTBIN);
#else
  carbon_strbuilder_add_cstr(&cmd, TESTDIR "/*.o " WORKDIR "/libcarbon.a -o " TESTBIN);
#endif
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
}

static void test(void) {
  CBN_INFO("Running tests...");
  test_compile_c_files();
  test_compile_cxx_files();
  test_link();
#ifndef _WIN32
  // TODO: make this work on Windows (MinGW)...
  rm_dash_r(TESTDIR "/*.o");
#endif
  carbon_println("+ " TESTBIN " -n");
  call_cmd(TESTBIN " -n");
}

static void examples_c_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match("examples/*.c");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CCLD    %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, "examples/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("examples/%s ", it.f));
    carbon_string_strip_substr(it.f, ".c");
    carbon_strbuilder_add_cstr(&cmd, WORKDIR "/libcarbon.a ");
    carbon_strbuilder_add_cstr(&cmd, LIBS);
#ifdef _WIN32
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o examples/%s", it.f));
#else
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o examples/%s.bin", it.f));
#endif
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void examples_cxx_files(void) {
  CBN_StrBuilder cmd = {0};
  CBN_PatternMatchedFiles files = carbon_fs_pattern_match("examples/*.cc");
  carbon_fs_pattern_match_foreach(files) {
    carbon_println("  CXXLD   %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE " OPTIMIZATIONS);
    carbon_string_strip_substr(it.f, "examples/");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("examples/%s ", it.f));
    carbon_string_strip_substr(it.f, ".cc");
    carbon_strbuilder_add_cstr(&cmd, WORKDIR "/libcarbon.a ");
    carbon_strbuilder_add_cstr(&cmd, LIBS);
#ifdef _WIN32
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o examples/%s", it.f));
#else
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt("-o examples/%s.bin", it.f));
#endif
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void examples(void) {
  CBN_INFO("Building examples...");
  examples_c_files();
  examples_cxx_files();
  const char *pong_skap = "examples/pong.skap";
  CBN_INFO("Creating `%$`...", $(pong_skap));
  CBN_ASSERT(carbon_skap_create("examples/pong.d/assets.txt", pong_skap));
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
  bootstrap(argv, false);
#ifdef CARBON_MAKE_DEBUG
  CBN_DEBUG("Compile-time option `CARBON_MAKE_DEBUG` is enabled");
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
    if (!carbon_string_cmp(subcmd, "version")) {
      carbon_print(version_msg, CARBON_NAME, CARBON_VERSION);
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
