#if 0  // https://stackoverflow.com/a/29709521
EXE="/tmp/a.out";
c++ -std=c++20 -O3 "$0" -o $EXE && $EXE "$@";
exit;
#endif
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <cstdio>
#include <cassert>
#include <cstring>
#include <format>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "hdrs/constants.h"
#include "hdrs/utils.hh"
#include "hdrs/header_files.hh"

void hdr_amalgamation(void) {
  printf("  GEN     " BUILD_DIR "/" HDR_FILE "\n");
  std::ofstream ofs {BUILD_DIR "/" HDR_FILE};
  assert(ofs);
  {// Header of the header
    std::ifstream ifs {HDR_FILE ".in"};
    assert(ifs);
    ofs << ifs.rdbuf() << "\n";
  }
  for (const auto &f : c_HeaderFiles) {
    std::ifstream ifs {std::format(SRC_DIR "/" NAME "_{}", f)};
    assert(ifs);
    std::string line;
    while (std::getline(ifs, line)) {
      if (StartsWith(line, "// SPDX-License-Identifier:") ||
          StartsWith(line, "// Copyright (C)")) continue;
      ofs << line << "\n";
    }
    ofs << "\n";
  }
}

void prepare_unity_build(void) {
  printf("  GEN     " SRC_FILE "\n");
  std::ofstream ofs {SRC_FILE};
  assert(ofs);
  ofs << "#include " << "\"" BUILD_DIR "/" HDR_FILE "\"" << "\n";
  for (const auto &e : fs::directory_iterator(SRC_DIR)) {
    if (!e.is_regular_file()) continue;
    const auto p = e.path();
    if (p.extension() == ".c") ofs << "#include " << p << "\n";
  }
}

void compile_and_link_lib(void) {
  printf("  CC      " OBJ_FILE "\n");
  RunCmd(CC_CMD " -fPIC -c " SRC_FILE " -o " OBJ_FILE);
  printf("  AR      " LIB_FILE "\n");
  RunCmd("ar -rcs " LIB_FILE " " OBJ_FILE);
  printf("  LD      " DLL_FILE "\n");
  RunCmd(CC_CMD " " OBJ_FILE " -shared " LDFLAGS " -o " DLL_FILE);
  printf("  RM      " SRC_FILE "\n");
  assert(fs::remove(SRC_FILE));
  printf("  RM      " OBJ_FILE "\n");
  assert(fs::remove(OBJ_FILE));
}

void test(void) {
  RunMetaprogram(TEST_EXE);
}

void build_tutorials(void) {
  for (const auto &e : fs::directory_iterator(TUTORIAL_DIR)) {
    if (!e.is_regular_file()) continue;
    auto p = e.path();
    const auto ext = p.extension();
    const auto f = p.replace_extension().c_str();
    if (ext == ".cc") {
      printf("  CXXLD   %s\n", f);
      RunCmd(std::format(CXX_CMD " {0}.cc " LIB_FILE " " LDFLAGS " -o {0}.exe", f).c_str());
    }
  }
}

void create_skap_for_tutorials(void) {
  RunMetaprogram(PACKER_EXE);
}

void tutorials(void) {
  build_tutorials();
  create_skap_for_tutorials();
}

void clean(void) {
  if (fs::remove_all(BUILD_DIR)) printf("  RM      " BUILD_DIR  "\n");
  if (fs::remove(SRC_FILE))      printf("  RM      " SRC_FILE   "\n");
  if (fs::remove(OBJ_FILE))      printf("  RM      " OBJ_FILE   "\n");
  if (fs::remove(PKG_FILE))      printf("  RM      " PKG_FILE   "\n");
  if (fs::remove(TEST_EXE))      printf("  RM      " TEST_EXE   "\n");
  if (fs::remove(PACKER_EXE))    printf("  RM      " PACKER_EXE "\n");
  for (const auto &e : fs::directory_iterator(TUTORIAL_DIR)) {
    if (!e.is_regular_file()) continue;
    const auto p = e.path();
    if (p.extension() == ".exe") {
      printf("  RM      %s\n", p.c_str());
      fs::remove(p);
    }
  }
}

void build(void) {
  printf("  MKDIR   " BUILD_DIR "\n");
  fs::create_directory(BUILD_DIR);
  hdr_amalgamation();
  prepare_unity_build();
  compile_and_link_lib();
  test();
  tutorials();
}

void pkg(void) {
  printf("  CP      ./LICENSE -> " BUILD_DIR "/LICENSE\n");
  fs::copy_file("./LICENSE", BUILD_DIR "/LICENSE", fs::copy_options::overwrite_existing);
  printf("  GZIP    " PKG_FILE "\n");
  RunCmd("tar -zcf " PKG_FILE " " BUILD_DIR);
}

void usage(void) {
  printf("usage: make [clean|build]\n");
}

int main(int argc, char **argv) {
  assert(fs::path(__FILE__).parent_path() == SRC_BUILD_DIR);
  if (argc == 1) {
    build();
    pkg();
  }
  else if (argc == 2) {
    const auto cmd = argv[1];
    if (!std::strcmp(cmd, "clean")) {
      clean();
    }
    else if (!std::strcmp(cmd, "build")) {
      build();
    }
    else if (!std::strcmp(cmd, "-h")    ||
             !std::strcmp(cmd, "help")  ||
             !std::strcmp(cmd, "-help") ||
             !std::strcmp(cmd, "--help")) {
      usage();
    }
    else {
      printf("unrecognized command\n");
      usage();
      return 1;
    }
  }
  else {
    usage();
    return 1;
  }
}
