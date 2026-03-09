#pragma once

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
