/*
**  $$================================$$
**  ||       Testing Entrypoint       ||
**  $$================================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef CARBON_TESTING_ENTRY
#define main(...)                                       \
  main(int argc, char **argv) {                         \
    carbon_test_manager_argparse(argc, argv);           \
    carbon_test_manager_rebuild(__FILE__, argv);        \
    return carbon_main();                               \
  };                                                    \
  int carbon_main(__VA_ARGS__)
#endif  // CARBON_TESTING_ENTRY

CARBON_API int carbon_main(void);
