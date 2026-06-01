#pragma once

#include "../../carbon.h.in"
#include "../../src/carbon_defs.hh"

#define NAME "carbon"

#define SRC_DIR        "./src"
#define TEST_DIR       "./src_test"
#define SRC_BUILD_DIR  "./src_build"
#define TUTORIAL_DIR   "./how_to"
#define THIRDPARTY_DIR "./thirdparty"

#define BUILD_DIR                               \
  NAME                                          \
  "-" CARBON_VERSION_STR                        \
  "-" CARBON_TARGET_OS                          \
  "-" CARBON_CPU_ARCH

#define HDR_NAME    NAME ".h"
#define HDR_IN_FILE HDR_NAME ".in"
#define HDR_FILE    BUILD_DIR "/" HDR_NAME
#define SRC_FILE    NAME ".c"
#define OBJ_FILE    NAME ".o"
#define LIB_FILE    BUILD_DIR "/lib" NAME ".a"

#ifdef _WIN32
#define DLL_FILE BUILD_DIR "/lib" NAME ".dll"
#else
#define DLL_FILE BUILD_DIR "/lib" NAME ".so"
#endif

#define PKG_FILE BUILD_DIR ".tgz"
#define THIRDPARTY_LICENSE_FILE BUILD_DIR "/LICENSE.THIRDPARTY"
#define TEST_EXE   SRC_BUILD_DIR "/meta/test"
#define PACKER_EXE SRC_BUILD_DIR "/meta/packer"

#define SHADER_NAME NAME "_win_shader"
#if defined(__APPLE__)
#define SHADER_IN_FILE SRC_DIR "/" SHADER_NAME ".metal"
#define SHADER_OUT_FILE SHADER_NAME ".metallib"
#define SHADER_INL_FILE SHADER_OUT_FILE ".inl"
#elif defined(_WIN32) || defined(__linux__) || defined(__FreeBSD__)
#define SHADER_FILE(stage) SRC_DIR "/" SHADER_NAME "." stage ".glsl"
#define SHADER_INL_FILE(stage) SHADER_NAME "." stage ".inl"
#else
#error Target platform is not supported
#endif
