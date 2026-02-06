/*
**  $$==========================$$
**  ||       Dependencies       ||
**  $$==========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
// Common
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifdef _WIN32  // Windows
#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE
#define NOMINMAX
#include <direct.h>
#include <windows.h>
#include <windns.h>
#else  // POSIX
#include <glob.h>
#include <poll.h>
#include <netdb.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#ifdef __APPLE__  // macOS
#include <mach-o/dyld.h>
#endif
#ifdef __cplusplus  // C++-only
#include <tuple>
#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <functional>
#endif
