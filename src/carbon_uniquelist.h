// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_UNIQUELIST_H_
#define CARBON_UNIQUELIST_H_

typedef struct {
  char **items;
  size_t size;
  size_t capacity;
} UniqueList;

UniqueList carbon_uniquelist_create(void);

void carbon_uniquelist_destroy(UniqueList *ul);

void carbon_uniquelist_push(UniqueList *ul, const char *s);

void carbon_uniquelist_pop(UniqueList *ul, const char *s);

unsigned char carbon_uniquelist_contains(UniqueList *ul, const char *s);

#endif  // CARBON_UNIQUELIST_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_uniquelist.c"
#endif  // CARBON_IMPLEMENTATION
