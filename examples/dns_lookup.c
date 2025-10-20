// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(int argc, char **argv) {
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (argc < 1) {
    CBN_ERROR("usage: %$ [DOMAIN]", $(program_name));
    return 1;
  }
  const char *domain = CARBON_SHIFT_ARGS(argc, argv);
  carbon_println("Domain: %$", $(domain));
  const char *ip = carbon_net_resolve_dns_to_ipv4(domain);
  carbon_println("IP: %$", $(ip));
  return 0;
}
