#pragma once

#include "../../carbon.h.in"
#include "../../src/carbon_defs.hh"

#define NAME "carbon"

#define SRC_DIR       "./src"
#define TEST_DIR      "./src_test"
#define SRC_BUILD_DIR "./src_build"
#define TUTORIAL_DIR  "./how_to"

#define BUILD_DIR                               \
  NAME                                          \
  "-" CARBON_VERSION_STR                        \
  "-" CARBON_TARGET_OS                          \
  "-" CARBON_CPU_ARCH

#define HDR_FILE   NAME ".h"
#define SRC_FILE   NAME ".c"
#define OBJ_FILE   NAME ".o"
#define LIB_FILE   "lib" NAME ".a"
#define DLL_FILE   "lib" NAME ".so"
#define PKG_FILE   BUILD_DIR ".tgz"
#define TEST_EXE   SRC_BUILD_DIR "/meta/test"
#define PACKER_EXE SRC_BUILD_DIR "/meta/packer"

#if defined(__APPLE__)
#define LDFLAGS "-framework CoreFoundation -lobjc"
#elif defined (_WIN32)
#define LDFLAGS "-ldnsapi -lws2_32 -lgdi32 -lntdll -lpthread -static"
#else
#define LDFLAGS "-lm"
#endif
