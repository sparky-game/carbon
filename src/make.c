// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

#define WORKDIR                                         \
  "carbon"                                              \
  "-" CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)     \
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)     \
  "-" CARBON_TARGET_OS                                  \
  "-" CARBON_CPU_ARCH

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
  CARBON_INFO("  CP      %s", dest);
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void build_src_files(void) {
  usz files_count = 0;
  char **files = carbon_fs_pattern_match("src/carbon_*.c", &files_count);
  for (usz i = 0; i < files_count; ++i) {
    CARBON_INFO("  CC      %s", files[i]);
    carbon_string_strip_substr(files[i], "src/");
    carbon_string_strip_substr(files[i], ".c");
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -I . -std=gnu99 -Wall -Wextra -pipe -O3 -c src/%s.c -o %s/%s.o", files[i], WORKDIR, files[i]));
  }
}

static void create_static_lib(void) {
  CARBON_INFO("  STRIP   " WORKDIR "/*.o");
  call_cmd(carbon_string_fmt("strip " WORKDIR "/*.o"));
  CARBON_INFO("  AR      libcarbon.a");
  call_cmd("ar -rcs " WORKDIR "/libcarbon.a " WORKDIR "/*.o");
  rm_dash_r(WORKDIR "/*.o");
}

static inline void compress_dir(const char *path) {
  CARBON_INFO("  GZIP   %s.tgz", path);
  call_cmd(carbon_string_fmt("tar -zcf %s.tgz %s", path, path));
}

static void run_tests(void) {
  const char *cc_cmd = CARBON_COMPILER " -I . -std=gnu99 -Wall -Wextra -fsanitize=address,undefined test/*.c -o test/carbon";
  const char *test_cmd = "./test/carbon -n";
  CARBON_INFO("  CCLD    test/carbon");
  call_cmd(cc_cmd);
  CARBON_INFO("+ %s", test_cmd);
  call_cmd(test_cmd);
  return;
}

static void build_examples(void) {
  usz files_count = 0;
  char **files = carbon_fs_pattern_match("examples/*.c", &files_count);
  for (usz i = 0; i < files_count; ++i) {
    carbon_string_strip_substr(files[i], ".c");
    CARBON_INFO("  CCLD    %s", files[i]);
    call_cmd(carbon_string_fmt(CARBON_COMPILER " -std=gnu99 -Wall -Wextra -pipe -O3 %s.c -static -o %s", files[i], files[i]));
  }
}

static inline void clean(void) {
  rm_dash_r("test/carbon");
  rm_dash_r(WORKDIR);
  rm_dash_r(WORKDIR ".tgz");
  const char *delete_cmd = "find examples -type f -executable -delete";
  CARBON_INFO("+ %s", delete_cmd);
  call_cmd(carbon_string_fmt(delete_cmd));
}

static inline void mrproper(void) {
  clean();
  rm_dash_r("make");
}

static void handle_args(int argc, char **argv) {
  if (argc != 2) return;
  if (!carbon_string_cmp(argv[1], "clean")) {
    clean();
    exit(0);
  }
  if (!carbon_string_cmp(argv[1], "mrproper")) {
    mrproper();
    exit(0);
  }
}

static void build(void) {
  CARBON_INFO("  MKDIR   " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) {
    clean();
    exit(1);
  }
  build_src_files();
  create_static_lib();
}

static void package(void) {
  CARBON_INFO("  MKDIR   " WORKDIR "/src");
  if (!carbon_fs_create_directory(WORKDIR "/src")) {
    clean();
    exit(1);
  }
  cp_dash_r("COPYING carbon.h", WORKDIR);
  cp_dash_r("src/carbon_*.c", WORKDIR "/src");
  compress_dir(WORKDIR);
  rm_dash_r(WORKDIR);
}

int main(int argc, char **argv) {
  if (!carbon_fs_change_directory(carbon_fs_get_bin_directory())) {
    CARBON_ERROR("Unable to change CWD to binary's directory");
    return 1;
  }
  handle_args(argc, argv);
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Running tests...");
  run_tests();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Building examples...");
  build_examples();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Building and packaging...");
  build();
  package();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output: " WORKDIR ".tgz is ready");
  return 0;
}
