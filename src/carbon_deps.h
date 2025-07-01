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
// Windows
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <direct.h>
#include <windows.h>
// POSIX
#else
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
// macOS
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
// C++
#ifdef __cplusplus
#include <tuple>
#include <string>
#include <memory>
#include <utility>
#include <optional>
#include <type_traits>
#endif
