// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <stddef.h>

#define CARBON_OK 1
#define CARBON_KO 0

#define CARBON_QUOTE(x) #x
#define CARBON_EXPAND_AND_QUOTE(x) CARBON_QUOTE(x)

#define CARBON_PASTE(x, y) x ## y
#define CARBON_EXPAND_AND_PASTE(x, y) CARBON_PASTE(x, y)

#if defined(__GNUC__) || defined(__clang__)
#define CARBON_STATIC_ASSERT _Static_assert
#else
#define CARBON_STATIC_ASSERT static_assert
#endif

#define CARBON_NOTUSED(v) (void)(v)

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef size_t usz;
typedef float f32;
typedef double f64;

CARBON_STATIC_ASSERT(sizeof(u8)  == 1, "Expected u8 to be 1 byte");
CARBON_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes");
CARBON_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes");
CARBON_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes");
CARBON_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes");
