// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <stdio.h>

#define CARBON_COLOR_RESET  "\033[0m"
#define CARBON_COLOR_RED    "\033[1;31m"
#define CARBON_COLOR_GREEN  "\033[1;32m"
#define CARBON_COLOR_YELLOW "\033[1;33m"
#define CARBON_COLOR_CYAN   "\033[1;36m"

#define CARBON_INFO(msg, ...) printf(msg, ##__VA_ARGS__)
#define CARBON_ERROR(msg, ...) fprintf(stderr, msg, ##__VA_ARGS__)
