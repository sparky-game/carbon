// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  const char *ver = carbon_version(0, 0, 0);
  carbon_println("Hello, World! This is Carbon %s", ver);
  return 0;
}
