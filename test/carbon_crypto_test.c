// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_crypto, base64) {
  const char *msg = "Hello";
  const usz msg_size = strlen(msg);
  usz b64_msg_size = 0;
  char *b64_msg = carbon_crypto_base64_encode((const u8 *) msg, msg_size, &b64_msg_size);
  carbon_should_be_s("SGVsbG8=", b64_msg);
  carbon_should_be(8, b64_msg_size);
  usz recv_msg_size = 0;
  u8 *recv_msg = carbon_crypto_base64_decode((const u8 *) b64_msg, &recv_msg_size);
  carbon_should_be_s(msg, recv_msg);
  carbon_should_be(msg_size, recv_msg_size);
  CARBON_FREE(b64_msg);
  CARBON_FREE(recv_msg);
  return CARBON_OK;
}

CARBON_TEST(carbon_crypto, crc32) {
  const char *msg = "Hello";
  const usz msg_size = strlen(msg);
  u32 crc32_msg = carbon_crypto_crc32((const u8 *) msg, msg_size);
  carbon_should_be(0xf7d18982, crc32_msg);
  return CARBON_OK;
}
