// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  cbn::println("Hello, World!");
  auto v = cbn::Version();
  auto patch = v & 0xff;
  auto minor = (v >> 8) & 0xff;
  auto major = (v >> 16) & 0xff;
  cbn::println("Carbon version %u.%u.%u (%s)", major, minor, patch, cbn::VersionStr());
}
