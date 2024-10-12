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

int carbon_main(void);

#define main(...)                               \
  main(int argc, char **argv) {                 \
    carbon_test_manager_argparse(argc, argv);   \
    return carbon_main();                       \
  };                                            \
  int carbon_main(__VA_ARGS__)
