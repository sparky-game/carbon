// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(int argc, char **argv) {
  auto self = CARBON_SHIFT_ARGS(argc, argv);
  if (argc < 1) {
    CBN_ERROR("usage: %s [DOMAIN]", self);
    return 1;
  }
  auto domain = CARBON_SHIFT_ARGS(argc, argv);
  cbn::println("Domain: %s", domain);
  auto ip = cbn::net::ResolveDNS(domain);
  cbn::println("IP: %s", ip);
}
