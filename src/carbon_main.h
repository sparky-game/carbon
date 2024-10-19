// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

int carbon_main(void);

#define main(...)                               \
  main(int argc, char **argv) {                 \
    carbon_test_manager_argparse(argc, argv);   \
    return carbon_main();                       \
  };                                            \
  int carbon_main(__VA_ARGS__)
