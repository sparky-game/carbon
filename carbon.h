/*
 * BSD Carbon --- A simple C/C++ unit testing framework
 * Copyright (C) 2024 Wasym A. Alonso
 *
 * This file is part of Carbon.
 *
 * Carbon is free software: you can redistribute it and/or modify
 * it under the terms of the BSD 3-Clause "New" or "Revised" License
 * as published by The Regents of the University of California.
 *
 * Carbon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * BSD 3-Clause "New" or "Revised" License for more details.
 *
 * You should have received a copy of the BSD 3-Clause "New" or
 * "Revised" License along with Carbon.
 * If not, see <https://opensource.org/license/BSD-3-Clause>.
 */


#pragma once

#define CARBON_NAME "BSD Carbon"
#define CARBON_VERSION "v0.6-alpha"

#ifdef __cplusplus
extern "C" {
#endif

#include "src/carbon_log.h"
#include "src/carbon_main.h"
#include "src/carbon_junit.h"
#include "src/carbon_should.h"
#include "src/carbon_defines.h"
#include "src/carbon_test_manager.h"

#ifdef __cplusplus
}
#endif
