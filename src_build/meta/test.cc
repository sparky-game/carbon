// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_TESTING_ENTRY
#include <carbon.h>

using namespace cbn::math::literals;

#include <carbon_list_test.hh>
#include <carbon_math_test.hh>
#include <carbon_metaprogramming_test.hh>
#include <carbon_slotmap_test.hh>
#include <carbon_string_test.hh>

int main(void) {
  return CARBON_RUN_ALL();
}
