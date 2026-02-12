// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(int argc, char **argv) {
  const char *self = CARBON_SHIFT_ARGS(argc, argv);
  if (!argc) {
    CBN_ERROR("usage: %s [FILE...]", self);
    return 1;
  }
  while (argc) {
    const char *file = CARBON_SHIFT_ARGS(argc, argv);
    auto sb = cbn::str::Builder::FromFile(file);
    if (!sb) return 1;
    cbn::print("%.*s", sb->size, sb->items);
    sb->Free();
  }
}
