// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(int argc, char **argv) {
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (!argc) {
    CBN_ERROR("usage: %$ [FILE...]", $(program_name));
    return 1;
  }
  while (argc) {
    const char *file = CARBON_SHIFT_ARGS(argc, argv);
    if (!carbon_fs_is_regular_file(file)) {
      CBN_ERROR("FILE (`%$`) needs to be a regular file", $(file));
      return 1;
    }
    CBN_StrBuilder sb = {0};
    if (!carbon_fs_read_entire_file(&sb, file)) {
      CBN_ERROR("FILE (`%$`) could not be read", $(file));
      return 1;
    }
    carbon_print("%.*s", sb.size, sb.items);
    carbon_strbuilder_free(&sb);
  }
  return 0;
}
