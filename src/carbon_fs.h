// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_FS_H_
#define CARBON_FS_H_

unsigned char carbon_fs_rename(const char *old, const char *new);

#endif  // CARBON_FS_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_fs.c"
#endif  // CARBON_IMPLEMENTATION
