#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

#include <glob.h>

#define WORKDIR                                         \
  "carbon"                                              \
  "-" CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MAJOR)     \
  "." CARBON_EXPAND_AND_QUOTE(CARBON_VERSION_MINOR)     \
  "-" CARBON_TARGET_OS                                  \
  "-" CARBON_CPU_ARCH

static void call_cmd(const char *cmd) {
  CARBON_INFO("+ %s", cmd);
  if (!system(cmd)) return;
  CARBON_ERROR("Unable to run `%s`", cmd);
  exit(EXIT_FAILURE);
}

static inline void rm_dash_r(const char *path) {
  call_cmd(carbon_string_fmt("rm -rf %s", path));
}

static inline void cp_dash_r(const char *origin, const char *dest) {
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static void build_src_files(void) {
  glob_t glob_result;
  glob("src/carbon_*.c", GLOB_TILDE, 0, &glob_result);
  for (usz i = 0; i < glob_result.gl_pathc; ++i) {
    CARBON_INFO("  CC      %s", glob_result.gl_pathv[i]);
    carbon_string_strip_substr(glob_result.gl_pathv[i], "src/");
    carbon_string_strip_substr(glob_result.gl_pathv[i], ".c");
    const char *cmd = carbon_string_fmt(CARBON_COMPILER " -I . -std=gnu99 -Wall -Wextra -pipe -O3 -c src/%s.c -o %s/%s.o",
                                        glob_result.gl_pathv[i], WORKDIR, glob_result.gl_pathv[i]);
    if (!system(cmd)) continue;
    CARBON_ERROR("Errors when compiling the code");
    rm_dash_r("build");
    exit(EXIT_FAILURE);
  }
}

static inline void strip_objs(const char *path) {
  call_cmd(carbon_string_fmt("strip %s", path));
}

static void create_static_lib(const char *name) {
  strip_objs(WORKDIR "/*.o");
  CARBON_INFO("  AR      %s", name);
  if (!system(carbon_string_fmt("ar -rcs %s/%s %s/*.o", WORKDIR, name, WORKDIR))) {
    rm_dash_r(WORKDIR "/*.o");
    return;
  }
  CARBON_ERROR("Unable to create static lib `%s`", name);
  exit(EXIT_FAILURE);
}

static inline void compress_dir(const char *path) {
  call_cmd(carbon_string_fmt("tar -zcf %s.tgz %s", path, path));
}

static void run_tests(void) {
  const char *cmd = CARBON_COMPILER " -I . -std=c99 -Wall -Wextra -fsanitize=address,undefined test/*.c -o carbon";
  CARBON_INFO("  CCLD    carbon");
  if (!system(cmd)) {
    call_cmd("./carbon");
    return;
  }
  CARBON_ERROR("Errors when compiling the code");
  exit(EXIT_FAILURE);
}

static inline void clean(void) {
  rm_dash_r("carbon " WORKDIR " " WORKDIR ".tgz");
}

static inline void mrproper(void) {
  clean();
  rm_dash_r("make");
}

static void handle_args(int argc, char **argv) {
  if (argc != 2) return;
  if (!carbon_string_cmp(argv[1], "clean")) {
    clean();
    exit(EXIT_SUCCESS);
  }
  if (!carbon_string_cmp(argv[1], "mrproper")) {
    mrproper();
    exit(EXIT_SUCCESS);
  }
}

static void build(void) {
  CARBON_INFO("+ mkdir " WORKDIR);
  if (!carbon_fs_create_directory(WORKDIR)) {
    clean();
    exit(EXIT_FAILURE);
  }
  build_src_files();
  create_static_lib("libcarbon.a");
}

static void package(void) {
  CARBON_INFO("+ mkdir " WORKDIR "/src");
  if (!carbon_fs_create_directory(WORKDIR "/src")) {
    clean();
    exit(EXIT_FAILURE);
  }
  cp_dash_r("COPYING carbon.h", WORKDIR);
  cp_dash_r("src/carbon_*.c", WORKDIR "/src");
  compress_dir(WORKDIR);
  rm_dash_r(WORKDIR);
}

int main(int argc, char **argv) {
  handle_args(argc, argv);
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Running tests...");
  run_tests();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Building and packaging...");  
  build();
  package();
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Output: " WORKDIR ".tgz");
  return EXIT_SUCCESS;
}
