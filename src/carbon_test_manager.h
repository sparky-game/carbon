/*
**  $$==========================$$
**  ||       Test Manager       ||
**  $$==========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_RUN_ALL carbon_test_manager_run
#define CARBON_REGISTER_TEST(f) carbon_test_manager_register(f, CARBON_QUOTE(f), __FILE__)
#define CARBON_TEST_FQN(ctx_name, unit_name) ctx_name ## _test_ ## unit_name
#define CARBON_TEST_DECL(ctx_name, unit_name) static u8 CARBON_TEST_FQN(ctx_name, unit_name)(void)
#define CARBON_TEST_REG_DECL(ctx_name, unit_name)                       \
  __attribute__((constructor)) static void CARBON_PASTE(register_, CARBON_TEST_FQN(ctx_name, unit_name))(void)

#define CARBON_TEST(ctx_name, unit_name)                        \
  CARBON_TEST_DECL(ctx_name, unit_name);                        \
  CARBON_TEST_REG_DECL(ctx_name, unit_name) {                   \
    CARBON_REGISTER_TEST(CARBON_TEST_FQN(ctx_name, unit_name)); \
  }                                                             \
  CARBON_TEST_DECL(ctx_name, unit_name)

typedef u8 (*CBN_TestFunc)(void);

typedef struct {
  CBN_TestFunc f;
  char *name;
  char *filename;
} CBN_Test;

typedef struct {
  CBN_Test *tests;
  usz n;
  CBN_StrList files;
} CBN_Suite;

typedef struct {
  u8 no_output;
  char *output;
} CBN_CmdArgs;

CBNDEF void carbon_test_manager_argparse(int argc, char * const *argv);
CBNDEF void carbon_test_manager_rebuild(const char *src_file, char * const *host_argv);
CBNDEF CBN_Suite carbon_test_manager_spawn(void);
CBNDEF CBN_Test *carbon_test_manager_alloc(CBN_Suite *s);
CBNDEF void carbon_test_manager_cleanup(CBN_Suite *s);
CBNDEF void carbon_test_manager_register_s(CBN_Suite *s, CBN_TestFunc test_func, const char *name, const char *filename);
CBNDEF void carbon_test_manager_register(CBN_TestFunc test_func, const char *name, const char *filename);
CBNDEF u8 carbon_test_manager_run_s(CBN_Suite *s);
CBNDEF u8 carbon_test_manager_run(void);
