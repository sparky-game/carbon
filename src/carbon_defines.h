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

#define CARBON_OK 1
#define CARBON_KO 0

#define CARBON_QUOTE(x) #x
#define CARBON_EXPAND_AND_QUOTE(x) CARBON_QUOTE(x)

#define CARBON_PASTE(x, y) x ## y
#define CARBON_EXPAND_AND_PASTE(x, y) CARBON_PASTE(x, y)
