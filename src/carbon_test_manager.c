#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <time.h>
#include <stdlib.h>

static Tests test_suite = {0};

CARBON_API Test *carbon_test_manager_alloc(void) {
  Test *p = 0;
  size_t size = sizeof(Test);
  if (!test_suite.tests) {
    p = malloc(size);
    if (!p) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_alloc :: failed to allocate memory (%zuB)\n", size);
      exit(1);
    }
  }
  else {
    size *= test_suite.n;
    Test *prev_p = test_suite.tests;
    p = realloc(test_suite.tests, size);
    if (!p) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_alloc :: failed to reallocate memory (%zuB)\n", size);
      free(prev_p);
      exit(1);
    }
  }
  return p;
}

CARBON_API void carbon_test_manager_register(TestFunc test_func, char *name) {
  ++test_suite.n;
  test_suite.tests = carbon_test_manager_alloc();
  test_suite.tests[test_suite.n - 1] = (Test) {
    .f = test_func,
    .name = name
  };
}

CARBON_API void carbon_test_manager_cleanup(void) {
  if (!test_suite.tests || !test_suite.n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_cleanup :: `test_suite` has not been initialized\n");
    return;
  }

  free(test_suite.tests);
  test_suite = (Tests) {0};
}

CARBON_API int carbon_test_manager_run(void) {
  if (!test_suite.tests || !test_suite.n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_run :: `test_suite` has not been initialized\n");
    return 1;
  }

  size_t passed = 0, failed = 0;
  clock_t total_time_start = clock();
  for (size_t i = 0; i < test_suite.n; ++i) {
    bool_t result = test_suite.tests[i].f();
    if (result) {
      CARBON_INFO("(%zu/%zu) %s :: PASSED\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++passed;
    }
    else {
      CARBON_ERROR("(%zu/%zu) %s :: FAILED\n", i + 1, test_suite.n, test_suite.tests[i].name);
      ++failed;
    }
  }
  clock_t total_time_stop = clock();
  double total_time = (double) (total_time_stop - total_time_start) / CLOCKS_PER_SEC;
  if (failed) {
    CARBON_ERROR("=========== %zu failed, %zu passed in %.2fs ===========\n",
                 failed,
                 passed,
                 total_time);
    return 1;
  }
  else CARBON_INFO("=========== %zu passed in %.2fs ===========\n",
                   passed,
                   total_time);
  carbon_test_manager_cleanup();
  return 0;
}
