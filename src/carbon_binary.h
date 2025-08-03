/*
**  $$====================$$
**  ||       Binary       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct {
  usz size;
} CBN_Binary_Metadata;

typedef struct {
  u8 *data;
  CBN_Binary_Metadata metadata;
} CBN_Binary;
