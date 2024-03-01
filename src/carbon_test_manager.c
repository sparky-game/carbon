#ifndef CARBON_IMPLEMENTATION
#include <carbon_test_manager.h>
#endif  // CARBON_IMPLEMENTATION

#include <time.h>
#include <stdlib.h>

static Tests test_suite = {0};

CARBON_API void carbon_test_manager_register(TestFunc test_func, char *name) {
  ++test_suite.n;
  if (!test_suite.tests) {
    test_suite.tests = malloc(sizeof(Test));
    if (!test_suite.tests) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_register :: failed to allocate memory\n");
      exit(1);
    }
  }
  else {
    Test *p = test_suite.tests;
    test_suite.tests = realloc(test_suite.tests, test_suite.n * sizeof(Test));
    if (!test_suite.tests) {
      CARBON_ERROR("[ERROR]: carbon_test_manager_register :: failed to allocate memory\n");
      free(p);
      exit(1);
    }
  }
  test_suite.tests[test_suite.n - 1] = (Test) {
    .f = test_func,
    .name = name
  };
}

CARBON_API int carbon_test_manager_run(void) {
  if (!test_suite.tests || !test_suite.n) {
    CARBON_ERROR("[ERROR]: carbon_test_manager_run :: `test_suite` has not been initialized\n");
    return;
  }

  int exit_code = 0;
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
    ++exit_code;
  }
  else {
    CARBON_INFO("=========== %zu passed in %.2fs ===========\n",
                passed,
                total_time);
  }
  free(test_suite.tests);
  return exit_code;
}
