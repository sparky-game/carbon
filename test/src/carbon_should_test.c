#include <carbon.h>
#include <carbon_should_test.h>

unsigned char carbon_should_test_should_be(void) {
  carbon_should_be(1, 1);
  return 1;
}

unsigned char carbon_should_test_should_not_be(void) {
  carbon_should_not_be(2, 1);
  return 1;
}

unsigned char carbon_should_test_should_be_true(void) {
  carbon_should_be_true(1 == 1);
  return 1;
}

unsigned char carbon_should_test_should_be_false(void) {
  carbon_should_be_false(2 == 1);
  return 1;
}

void carbon_should_test_register(void) {
  CARBON_REGISTER_TEST(carbon_should_test_should_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_not_be);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_true);
  CARBON_REGISTER_TEST(carbon_should_test_should_be_false);
}
