#include <carbon.h>
#include <carbon_should_test.h>
#include <carbon_test_manager_test.h>

int main(void) {
  carbon_should_test_register();
  carbon_test_manager_test_register();

  return carbon_test_manager_run();
}
