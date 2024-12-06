// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_time, snowflake) {
  CBN_SnowflakeComponents cmps = carbon_time_snowflake_parse(1160192133117220889);
  carbon_should_be_u64(1733452918, cmps.timestamp);
  carbon_should_be_u64(2594841, cmps.random);
  return CARBON_OK;
}
