// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <cstdio>
#include <cassert>
#include <cstring>
#include <format>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "hdrs/dirs_n_files.h"
#include "hdrs/compiler_flags.h"
#include "hdrs/utils.hh"
#include "hdrs/header_files.hh"

void license_amalgamation(void) {
  printf("  GEN     " THIRDPARTY_LICENSE_FILE "\n");
  std::ofstream ofs {THIRDPARTY_LICENSE_FILE};
  assert(ofs);
  for (const auto &e : fs::directory_iterator(THIRDPARTY_DIR)) {
    if (!e.is_directory()) continue;
    const auto p = e.path();
    const auto l = p / "LICENSE";
    if (!fs::is_regular_file(l)) continue;
    const auto name = p.filename();
    ofs << "** " << name.string() << " **\n";
    {// Dump the license text
      std::ifstream ifs {l};
      assert(ifs);
      ofs << ifs.rdbuf() << "\n------\n\n";
    }
  }
}

void hdr_amalgamation(void) {
  printf("  GEN     " HDR_FILE "\n");
  std::ofstream ofs {HDR_FILE};
  assert(ofs);
  {// Header of the header
    std::ifstream ifs {HDR_IN_FILE};
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

void src_amalgamation(void) {
  printf("  GEN     " SRC_FILE "\n");
  std::ofstream ofs {SRC_FILE};
  assert(ofs);
  ofs << "#include " << "\"" HDR_FILE "\"" << "\n";
  for (const auto &e : fs::directory_iterator(SRC_DIR)) {
    if (!e.is_regular_file()) continue;
    const auto p = e.path();
    if (p.extension() == ".c") ofs << "#include " << p << "\n";
  }
}

void build_and_embed_shader(void) {
#if defined(__APPLE__)
  printf("  METAL   " SHADER_OUT_FILE "\n");
  RunCmd("xcrun --sdk macosx metal " SHADER_IN_FILE " -o " SHADER_OUT_FILE " -target air64-apple-macos11.0");
  printf("  GEN     " SHADER_INL_FILE "\n");
  RunCmd("xxd -i " SHADER_OUT_FILE " > " SHADER_INL_FILE);
#elif defined(__linux__) || defined(__FreeBSD__)
  printf("  SPIR-V  " SHADER_OUT_FILE("vert") "\n");
  RunCmd("glslangValidator --quiet -G " SHADER_IN_FILE("vert") " -o " SHADER_OUT_FILE("vert"));
  printf("  SPIR-V  " SHADER_OUT_FILE("frag") "\n");
  RunCmd("glslangValidator --quiet -G " SHADER_IN_FILE("frag") " -o " SHADER_OUT_FILE("frag"));
  printf("  GEN     " SHADER_INL_FILE("vert") "\n");
  RunCmd("xxd -i " SHADER_OUT_FILE("vert") " > " SHADER_INL_FILE("vert"));
  printf("  GEN     " SHADER_INL_FILE("frag") "\n");
  RunCmd("xxd -i " SHADER_OUT_FILE("frag") " > " SHADER_INL_FILE("frag"));
#else
#error Not implemented yet
#endif
}

void compile_and_link_lib(void) {
  printf("  CC      " OBJ_FILE "\n");
#if defined(__APPLE__)
  RunCmd(CC_CMD " -x objective-c -include " SHADER_INL_FILE " -fPIC -c " SRC_FILE " -o " OBJ_FILE);
#elif defined(__linux__) || defined(__FreeBSD__)
  RunCmd(CC_CMD " -include " SHADER_INL_FILE("vert") " -include " SHADER_INL_FILE("frag") " -fPIC -c " SRC_FILE " -o " OBJ_FILE);
#else
#error Not implemented yet
#endif
  printf("  AR      " LIB_FILE "\n");
  RunCmd("ar -rcs " LIB_FILE " " OBJ_FILE);
  printf("  LD      " DLL_FILE "\n");
  RunCmd(CC_CMD " " OBJ_FILE " -shared " LDFLAGS " -o " DLL_FILE);
  printf("  RM      " SRC_FILE "\n");
  assert(fs::remove(SRC_FILE));
  printf("  RM      " OBJ_FILE "\n");
  assert(fs::remove(OBJ_FILE));
#if defined(__APPLE__)
  printf("  RM      " SHADER_OUT_FILE "\n");
  assert(fs::remove(SHADER_OUT_FILE));
  printf("  RM      " SHADER_INL_FILE "\n");
  assert(fs::remove(SHADER_INL_FILE));
#elif defined(__linux__) || defined(__FreeBSD__)
  printf("  RM      " SHADER_OUT_FILE("vert") "\n");
  assert(fs::remove(SHADER_OUT_FILE("vert")));
  printf("  RM      " SHADER_OUT_FILE("frag") "\n");
  assert(fs::remove(SHADER_OUT_FILE("frag")));
  printf("  RM      " SHADER_INL_FILE("vert") "\n");
  assert(fs::remove(SHADER_INL_FILE("vert")));
  printf("  RM      " SHADER_INL_FILE("frag") "\n");
  assert(fs::remove(SHADER_INL_FILE("frag")));
#else
#error Not implemented yet
#endif
}

void build_tutorials(void) {
  for (const auto &e : fs::directory_iterator(TUTORIAL_DIR)) {
    if (!e.is_regular_file()) continue;
    auto p = e.path();
    const auto ext = p.extension();
    const auto f = p.replace_extension().string();
    if (ext == ".cc") {
      printf("  CXXLD   %s\n", f.c_str());
      RunCmd(std::format(CXX_CMD " {0}.cc " LIB_FILE " " LDFLAGS " -o {0}.exe", f).c_str());
    }
  }
}

void build(void) {
  printf("  MKDIR   " BUILD_DIR "\n");
  fs::create_directory(BUILD_DIR);
  license_amalgamation();
  hdr_amalgamation();
  src_amalgamation();
  build_and_embed_shader();
  compile_and_link_lib();
  RunMetaprogram(TEST_EXE);
  build_tutorials();
  RunMetaprogram(PACKER_EXE);
}

void pkg(void) {
  printf("  CP      ./LICENSE -> " BUILD_DIR "/LICENSE\n");
  fs::copy_file("./LICENSE", BUILD_DIR "/LICENSE", fs::copy_options::overwrite_existing);
  printf("  GZIP    " PKG_FILE "\n");
  RunCmd("tar -zcf " PKG_FILE " " BUILD_DIR);
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
      printf("  RM      %s\n", p.string().c_str());
      fs::remove(p);
    }
  }
}

void usage(void) {
  printf("usage: make [clean|build]\n");
}

int main(int argc, char **argv) {
  {// Check running from expected dir
    auto act = fs::path(__FILE__).parent_path().lexically_normal();
    auto exp = fs::path(SRC_BUILD_DIR).lexically_normal();
    assert(act == exp);
  }
  if (argc == 1) {
    build();
    pkg();
  }
  else if (argc == 2) {
    const auto cmd = argv[1];
    if      (!std::strcmp(cmd, "clean")) clean();
    else if (!std::strcmp(cmd, "build")) build();
    else if (!std::strcmp(cmd, "-h")    ||
             !std::strcmp(cmd, "help")  ||
             !std::strcmp(cmd, "-help") ||
             !std::strcmp(cmd, "--help")) usage();
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
