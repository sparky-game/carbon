// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_IMPLEMENTATION
#include "../carbon.h"

#define TESTBIN "./test/carbon"
#define WORKDIR "carbon-" CARBON_VERSION_RAW "-" CARBON_TARGET_OS "-" CARBON_CPU_ARCH

#define C_STD   "-std=c11"
#define CXX_STD "-std=c++11"
#define WARNS   "-Wall -Wextra -Wswitch-enum -Werror=format -Werror=incompatible-pointer-types -Wno-return-type-c-linkage"

#if defined(__APPLE__)
#define GUI_LIBS "-framework Cocoa -framework CoreVideo -framework IOKit"
#elif defined(__linux__)
#define GUI_LIBS "-lX11 -lXrandr"
#endif

static const char * const help_msg = "usage: %s [SUBCMD]\n"
  "Subcommands:\n"
  "  help        display this help\n"
  "  clean       remove previously created build artifacts\n"
  "  mrproper    same as `clean` plus remove this binary\n"
  "  test        just run the `TESTING` stage\n"
  "  build       just run the `BUILDING` stage\n"
  "  examples    just run the `EXAMPLES` stage\n"
  "\n"
  "If not provided any subcommand, it runs the full build pipeline:\n"
  "  [TESTING] -> [BUILDING] -> [EXAMPLES] -> [PACKAGING]\n"
  "\n"
  "If compiled with `CARBON_MAKE_USE_SANITIZERS`, tests will run with sanitizers enabled.\n"
  "\n"
  "Report bugs to: <https://github.com/sparky-game/carbon/issues>\n"
  "%s homepage: <https://github.com/sparky-game/carbon>\n";

// TODO: migrate this func to the lib itself
static inline void call_cmd(const char *cmd) {
  if (!system(cmd)) return;
  carbon_log_error("Unable to run `%s`", cmd);
  exit(1);
}

// TODO: migrate this func to the lib itself
static inline void rm_dash_r(const char *path) {
  carbon_println("  RM      %s", path);
  call_cmd(carbon_string_fmt("rm -rf %s", path));
}

// TODO: migrate this func to the lib itself
static inline void cp_dash_r(const char *origin, const char *dest) {
  carbon_println("  CP      %s -> %s", origin, dest);
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void clean(void) {
  rm_dash_r(TESTBIN);
  rm_dash_r("test/*.o");
  rm_dash_r(WORKDIR);
  rm_dash_r("examples/*.bin");
  rm_dash_r("examples/*.bin.old");
  rm_dash_r(WORKDIR ".tgz");
}

static void exit_gracefully(void) {
  clean();
  exit(1);
}

static void rebuild_myself(char * const *host_argv) {
  const char *bin = host_argv[0];
#ifdef CARBON_MAKE_ALREADY_REBUILT
  if (carbon_fs_mtime(__FILE__) <= carbon_fs_mtime(bin)) return;
#endif
  i32 rebuild_status_code = 0;
  pid_t rebuild_child_pid = fork();
  if (rebuild_child_pid == -1) {
    carbon_log_error("unable to fork child process");
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
      "-o", (char *) bin, 0
    };
    carbon_println("  CCLD    %s", __FILE__);
    if (-1 == execvp(argv[0], argv)) {
      carbon_log_error("unable to execvp from child process");
      exit(1);
    }
  }
  else waitpid(rebuild_child_pid, &rebuild_status_code, 0);
  if (rebuild_status_code != 0) {
    carbon_log_error("errors detected during rebuild");
    exit(1);
  }
  carbon_println("  EXEC    %s", bin);
  if (-1 == execvp(bin, host_argv)) {
    carbon_log_error("unable to execvp rebuilt binary");
    exit(1);
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
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
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
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
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
  carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
  carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
  carbon_strbuilder_add_cstr(&cmd, "test/*.o -o " TESTBIN);
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r("test/*.o");
  carbon_println("+ " TESTBIN " -n");
  call_cmd(TESTBIN " -n");
}

static void build(void) {
  carbon_log_info("Building...");
  carbon_println("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) exit_gracefully();
  CBN_PatternMatchedFiles c_files    = carbon_fs_pattern_match("src/carbon_*.c");
  CBN_PatternMatchedFiles cxx_files  = carbon_fs_pattern_match("src/carbon_*.cc");
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CC      %s", it.f);
    carbon_string_strip_substr(it.f, "src/");
    call_cmd(carbon_string_fmt(CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIC -pipe -Os -c src/%s -o %s/%s.o", it.f, WORKDIR, it.f));
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXX     %s", it.f);
    carbon_string_strip_substr(it.f, "src/");
    call_cmd(carbon_string_fmt(CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIC -pipe -Os -c src/%s -o %s/%s.o", it.f, WORKDIR, it.f));
  }
  carbon_println("  AR      libcarbon.a");
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
  carbon_println("  LD      libcarbon.so");
  CBN_StrBuilder cmd = {0};
  carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -pipe -Os " WORKDIR "/*.o -shared " GUI_LIBS " -o " WORKDIR "/libcarbon.so");
  call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
  carbon_strbuilder_free(&cmd);
  rm_dash_r(WORKDIR "/*.o");
}

static void examples(void) {
  carbon_log_info("Building examples...");
  CBN_PatternMatchedFiles c_files = carbon_fs_pattern_match("examples/*.c");
  CBN_PatternMatchedFiles cxx_files = carbon_fs_pattern_match("examples/*.cc");
  const char *gui_examples[] = {
    "examples/flag_jp.c",
    "examples/triangle.c",
    "examples/orbiting.cc",
    "examples/bouncing_square.cc"
  };
  CBN_StrBuilder cmd = {0};
  carbon_fs_pattern_match_foreach(c_files) {
    carbon_println("  CCLD    %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_C_COMPILER " -I . " C_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, it.f);
    for (usz i = 0; i < CARBON_ARRAY_LEN(gui_examples); ++i) {
      if (!carbon_string_cmp(it.f, gui_examples[i])) carbon_strbuilder_add_cstr(&cmd, " " GUI_LIBS);
    }
    carbon_string_strip_substr(it.f, ".c");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -o %s.bin", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
  carbon_fs_pattern_match_foreach(cxx_files) {
    carbon_println("  CXXLD   %s", it.f);
    carbon_strbuilder_add_cstr(&cmd, CARBON_CXX_COMPILER " -I . " CXX_STD " " WARNS " -fPIE ");
#ifdef CARBON_MAKE_USE_SANITIZERS
    carbon_strbuilder_add_cstr(&cmd, "-fsanitize=address,undefined ");
#else
    carbon_strbuilder_add_cstr(&cmd, "-pipe -Os ");
#endif
    carbon_strbuilder_add_cstr(&cmd, it.f);
    for (usz i = 0; i < CARBON_ARRAY_LEN(gui_examples); ++i) {
      if (!carbon_string_cmp(it.f, gui_examples[i])) carbon_strbuilder_add_cstr(&cmd, " " GUI_LIBS);
    }
    carbon_string_strip_substr(it.f, ".cc");
    carbon_strbuilder_add_cstr(&cmd, carbon_string_fmt(" -o %s.bin", it.f));
    call_cmd(carbon_strview_to_cstr(carbon_strview_from_strbuilder(&cmd)));
    carbon_strbuilder_free(&cmd);
  }
}

static void package(void) {
  carbon_log_info("Packaging...");
  cp_dash_r("COPYING carbon.h", WORKDIR);
  carbon_println("  GZIP    " WORKDIR ".tgz");
  call_cmd("tar -zcf " WORKDIR ".tgz " WORKDIR);
  rm_dash_r(WORKDIR);
  carbon_log_info(WORKDIR ".tgz is ready");
}

int main(int argc, char **argv) {
  if (!carbon_fs_change_directory(carbon_fs_get_bin_directory())) {
    carbon_log_error("unable to change CWD to binary's directory");
    return 1;
  }
  rebuild_myself(argv);
#ifdef CARBON_MAKE_ALREADY_REBUILT
  carbon_log_info(CARBON_NAME " " CARBON_VERSION " (" CARBON_COMPILER_VERSION ") " __DATE__ " " __TIME__);
#endif
#ifdef CARBON_MAKE_USE_SANITIZERS
  carbon_log_debug("Compile-time option `CARBON_MAKE_USE_SANITIZERS` is enabled");
#endif
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (argc > 1) {
    carbon_log_error("unrecognized option\nTry '%s help' for more information.", program_name);
    return 1;
  }
  if (argc == 1) {
    const char *subcmd = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_string_cmp(subcmd, "help")) {
      carbon_print(help_msg, program_name, CARBON_NAME);
      return 0;
    }
    else if (!carbon_string_cmp(subcmd, "clean")) {
      clean();
      return 0;
    }
    else if (!carbon_string_cmp(subcmd, "mrproper")) {
      clean();
      rm_dash_r(program_name);
      return 0;
    }
    else if (!carbon_string_cmp(subcmd, "test")) {
      test();
      return 0;
    }
    else if (!carbon_string_cmp(subcmd, "build")) {
      build();
      return 0;
    }
    else if (!carbon_string_cmp(subcmd, "examples")) {
      examples();
      return 0;
    }
    else {
      carbon_log_error("unrecognized option\nTry '%s help' for more information.", program_name);
      return 1;
    }
  }
  test();
  build();
  examples();
  package();
  return 0;
}
