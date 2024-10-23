// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_TEST_MANAGER_H_
#define CARBON_TEST_MANAGER_H_

#define CARBON_REGISTER_TEST(f) carbon_test_manager_register(f, CARBON_EXPAND_AND_QUOTE(f), __FILE__)
#define CARBON_TEST_FQN(ctx_name, unit_name) ctx_name ## _test_ ## unit_name
#define CARBON_TEST_DECL(ctx_name, unit_name) static u8 CARBON_TEST_FQN(ctx_name, unit_name)(void)
#define CARBON_TEST_REG_DECL(ctx_name, unit_name) __attribute__((constructor)) static void CARBON_EXPAND_AND_PASTE(register_, CARBON_TEST_FQN(ctx_name, unit_name))(void)

#define CARBON_TEST(ctx_name, unit_name)                        \
  CARBON_TEST_DECL(ctx_name, unit_name);                        \
  CARBON_TEST_REG_DECL(ctx_name, unit_name) {                   \
    CARBON_REGISTER_TEST(CARBON_TEST_FQN(ctx_name, unit_name)); \
  }                                                             \
  CARBON_TEST_DECL(ctx_name, unit_name)

#define CARBON_RUN_ALL carbon_test_manager_run

typedef u8 (*TestFunc)(void);

typedef struct {
  TestFunc f;
  char *name;
  char *filename;
} Test;

typedef struct {
  Test *tests;
  size_t n;
  UniqueList files;
} Suite;

typedef struct {
  char *output;
} CmdArgs;

void carbon_test_manager_argparse(int argc, char **argv);

void carbon_test_manager_rebuild(const char *bin_file, const char *src_file);

Suite carbon_test_manager_spawn(void);

Test *carbon_test_manager_alloc(Suite *s);

void carbon_test_manager_cleanup(Suite *s);

void carbon_test_manager_register_s(Suite *s, TestFunc test_func, char *name, char *filename);
void carbon_test_manager_register(TestFunc test_func, char *name, char *filename);

u8 carbon_test_manager_run_s(Suite *s);
u8 carbon_test_manager_run(void);

#endif  // CARBON_TEST_MANAGER_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_test_manager.c"
#endif  // CARBON_IMPLEMENTATION
