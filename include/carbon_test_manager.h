#ifndef CARBON_TEST_MANAGER_H_
#define CARBON_TEST_MANAGER_H_

#define CARBON_REGISTER_TEST(f) carbon_test_manager_register(f, #f)

typedef bool_t (*TestFunc)(void);

typedef struct {
  TestFunc f;
  char *name;
} Test;

typedef struct {
  Test *tests;
  size_t n;
} Suite;

Suite carbon_test_manager_spawn(void);
Test *carbon_test_manager_alloc(void);
void carbon_test_manager_register(TestFunc test_func, char *name);
void carbon_test_manager_cleanup(void);
int carbon_test_manager_run(void);

#endif  // CARBON_TEST_MANAGER_H_

#ifdef CARBON_IMPLEMENTATION
#include "../src/carbon_test_manager.c"
#endif  // CARBON_IMPLEMENTATION
