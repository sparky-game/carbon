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


#ifndef CARBON_TEST_MANAGER_H_
#define CARBON_TEST_MANAGER_H_

#define CARBON_REGISTER_TEST(f) carbon_test_manager_register(f, CARBON_EXPAND_AND_QUOTE(f))
#define CARBON_TEST_FQN(ctx_name, unit_name) ctx_name ## _test_ ## unit_name
#define CARBON_TEST_DECL(ctx_name, unit_name) static unsigned char CARBON_TEST_FQN(ctx_name, unit_name)(void)
#define CARBON_TEST_REG_DECL(ctx_name, unit_name) __attribute__((constructor)) static void CARBON_EXPAND_AND_PASTE(register_, CARBON_TEST_FQN(ctx_name, unit_name))(void)

#define CARBON_TEST(ctx_name, unit_name)                        \
  CARBON_TEST_DECL(ctx_name, unit_name);                        \
  CARBON_TEST_REG_DECL(ctx_name, unit_name) {                   \
    CARBON_REGISTER_TEST(CARBON_TEST_FQN(ctx_name, unit_name)); \
  }                                                             \
  CARBON_TEST_DECL(ctx_name, unit_name)

typedef unsigned char (*TestFunc)(void);

typedef struct {
  TestFunc f;
  char *name;
} Test;

typedef struct {
  Test *tests;
  size_t n;
} Suite;

typedef struct {
  char *output;
} CmdArgs;

void carbon_test_manager_argparse(int argc, char **argv);

Suite carbon_test_manager_spawn(void);

Test *carbon_test_manager_alloc(Suite *s);

void carbon_test_manager_cleanup(Suite *s);

void carbon_test_manager_register_s(Suite *s, TestFunc test_func, char *name);
void carbon_test_manager_register(TestFunc test_func, char *name);

unsigned char carbon_test_manager_run_s(Suite *s);
unsigned char carbon_test_manager_run(void);

#endif  // CARBON_TEST_MANAGER_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_test_manager.c"
#endif  // CARBON_IMPLEMENTATION
