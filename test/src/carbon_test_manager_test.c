#include <carbon.h>
#include <carbon_test_manager_test.h>

unsigned char test_dummy(void) {
  carbon_should_be_true(1 == 1);
  return 1;
}

unsigned char carbon_test_manager_test_suite_creation(void) {
  Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return 1;
}

unsigned char carbon_test_manager_test_registration(void) {
  Suite s = carbon_test_manager_spawn();
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);

  carbon_test_manager_register_s(&s, test_dummy, "test_dummy");
  carbon_should_not_be_p(0, s.tests);
  carbon_should_be(1, s.n);
  carbon_should_be_s("test_dummy", s.tests[0].name);

  carbon_test_manager_cleanup_s(&s);
  carbon_should_be_p(0, s.tests);
  carbon_should_be(0, s.n);
  return 1;
}

void carbon_test_manager_test_register(void) {
  CARBON_REGISTER_TEST(carbon_test_manager_test_suite_creation);
  CARBON_REGISTER_TEST(carbon_test_manager_test_registration);
}
