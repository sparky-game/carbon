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

#ifndef CC
#define CC "cc"
#endif
#ifndef CXX
#define CXX "c++"
#endif
#define CC_STD       "-std=c11"
#define CXX_STD      "-std=c++20"
#define WARNS        "-Wall -Wextra -Werror=switch-enum -Wno-return-type-c-linkage -Wno-strict-aliasing"
#define INCLUDE_DIRS "-I" BUILD_DIR " -I" TEST_DIR
#define CC_CMD       CC  " " CC_STD  " " INCLUDE_DIRS " " WARNS " -O3"
#define CXX_CMD      CXX " " CXX_STD " " INCLUDE_DIRS " " WARNS " -O3"

#if defined(__APPLE__)
#define LDFLAGS "-framework Cocoa -framework CoreVideo -framework IOKit"
#elif defined (_WIN32)
#define LDFLAGS "-ldnsapi -lws2_32 -lgdi32 -lntdll -lpthread -static"
#else
#define LDFLAGS "-lX11 -lXrandr -lm"
#endif
