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

static void rm_first_substr_from_str(char *s, const char *subs) {
  char *pos = strstr(s, subs);
  if (pos) {
    usz len = strlen(subs);
    memmove(pos, pos + len, strlen(pos + len) + 1);
  }
}

static inline void rm_substr_from_str(char *s, const char *subs) {
  while (strstr(s, subs)) rm_first_substr_from_str(s, subs);
}

static void call_cmd(const char *cmd) {
  if (!system(cmd)) {
    CARBON_INFO("+ %s", cmd);
    return;
  }
  CARBON_ERROR("Unable to run `%s`", cmd);
  exit(1);
}

static inline void rm_dash_r(const char *path) {
  call_cmd(carbon_string_fmt("rm -r %s", path));
}

static inline void cp_dash_r(const char *origin, const char *dest) {
  call_cmd(carbon_string_fmt("cp -r %s %s", origin, dest));
}

static inline void make_dir(const char *path) {
  call_cmd(carbon_string_fmt("mkdir %s", path));
}

static void build_src_files(void) {
  glob_t glob_result;
  glob("src/carbon_*.c", GLOB_TILDE, 0, &glob_result);
  for (usz i = 0; i < glob_result.gl_pathc; ++i) {
    CARBON_INFO("  CC      %s", glob_result.gl_pathv[i]);
    rm_substr_from_str(glob_result.gl_pathv[i], "src/");
    rm_substr_from_str(glob_result.gl_pathv[i], ".c");
    const char *cmd = carbon_string_fmt("clang -I . -std=gnu99 -Wall -Wextra -pipe -O3 -c src/%s.c -o %s/%s.o",
                                        glob_result.gl_pathv[i], WORKDIR, glob_result.gl_pathv[i]);
    if (!system(cmd)) continue;
    CARBON_ERROR("Errors when compiling the code");
    rm_dash_r("build");
    exit(1);
  }
}

static inline void strip_objs(const char *path) {
  call_cmd(carbon_string_fmt("strip %s", path));
}

static void create_static_lib(const char *name) {
  strip_objs(WORKDIR "/*.o");
  if (!system(carbon_string_fmt("ar -rcs %s/%s %s/*.o", WORKDIR, name, WORKDIR))) {
    CARBON_INFO("  AR      %s", name);
    rm_dash_r(WORKDIR "/*.o");
    return;
  }
  CARBON_ERROR("Unable to create static lib `%s`", name);
  exit(1);
}

static inline void compress_dir(const char *path) {
  call_cmd(carbon_string_fmt("tar -zcf %s.tgz %s", path, path));
}

int main(void) {
  make_dir(WORKDIR);
  build_src_files();
  create_static_lib("libcarbon.a");
  make_dir(WORKDIR "/src");
  cp_dash_r("COPYING carbon.h", WORKDIR);
  cp_dash_r("src/carbon_*.c", WORKDIR "/src");
  compress_dir(WORKDIR);
  rm_dash_r(WORKDIR);
  return 0;
}
