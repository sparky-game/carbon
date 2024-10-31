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
  usz b64_msg_size = 0;
  char *b64_msg = carbon_crypto_base64_encode((const u8 *) msg, msg_size, &b64_msg_size);
  CARBON_INFO("b64_msg -> `%s`", b64_msg);
  CARBON_INFO("b64_msg_size -> %zu", b64_msg_size);
  CARBON_INFO("---------------------------------------");
  usz recv_msg_size = 0;
  u8 *recv_msg = carbon_crypto_base64_decode((const u8 *) b64_msg, &recv_msg_size);
  CARBON_INFO("recv_msg -> `%s`", recv_msg);
  CARBON_INFO("recv_msg_size -> %zu", recv_msg_size);
  CARBON_ASSERT(msg_size == recv_msg_size);
  CARBON_FREE(b64_msg);
  CARBON_FREE(recv_msg);
  return 0;
}
