#include <carbon.h>
#include <carbon_should_test.h>

int main(void) {
  carbon_should_test_register();

  return carbon_test_manager_run();
}
