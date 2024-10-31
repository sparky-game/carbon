// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

int main(void) {
  const char *msg = "Hello";
  usz msg_size = strlen(msg);
  CARBON_INFO("msg -> `%s`", msg);
  CARBON_INFO("msg_size -> %zu", msg_size);
  CARBON_INFO("---------------------------------------");
  u32 crc32_msg = carbon_crypto_crc32((const u8 *) msg, msg_size);
  CARBON_INFO("crc32_msg (decimal) -> `%u`", crc32_msg);
  CARBON_INFO("crc32_msg (hex)     -> `%#02x`", crc32_msg);
  return 0;
}
