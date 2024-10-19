// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_OK 1
#define CARBON_KO 0

#define CARBON_QUOTE(x) #x
#define CARBON_EXPAND_AND_QUOTE(x) CARBON_QUOTE(x)

#define CARBON_PASTE(x, y) x ## y
#define CARBON_EXPAND_AND_PASTE(x, y) CARBON_PASTE(x, y)
