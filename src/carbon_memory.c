// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_MEMORY__HDR_SZ (sizeof(usz))

static usz carbon_memory__usage;

void *carbon_memory_alloc(usz size) {
  u8 *p = __builtin_malloc(size + CARBON_MEMORY__HDR_SZ);
  CBN_ASSERT(p && "failed to allocate memory");
  *(usz *)p = size;
  carbon_memory__usage += size;
  return p + CARBON_MEMORY__HDR_SZ;
}

void *carbon_memory_zeroed(usz size) {
  void *p = carbon_memory_alloc(size);
  carbon_memory_set(p, 0, size);
  return p;
}

void *carbon_memory_realloc(void *p, usz size) {
  if (!p) return carbon_memory_alloc(size);
  u8 *raw_p = (u8 *)p - CARBON_MEMORY__HDR_SZ;
  usz old_sz = *(usz *)raw_p;
  u8 *new_p = __builtin_realloc(raw_p, size + CARBON_MEMORY__HDR_SZ);
  CBN_ASSERT(new_p && "failed to reallocate memory");
  *(usz *)new_p = size;
  carbon_memory__usage -= old_sz;
  carbon_memory__usage += size;
  return new_p + CARBON_MEMORY__HDR_SZ;
}

void carbon_memory_free(void *p) {
  if (!p) return;
  u8 *raw_p = (u8 *)p - CARBON_MEMORY__HDR_SZ;
  carbon_memory__usage -= *(usz *)raw_p;
  __builtin_free(raw_p);
}

usz carbon_memory_usage(void) {
  return carbon_memory__usage;
}

void *carbon_memory_copy(void *dst, const void *src, usz n) {
  u8 *d = (u8 *) dst;
  const u8 *s = (const u8 *) src;
  if (d == s || !n) return dst;
  if (d < s) while (n--) *d++ = *s++;
  else while (n--) d[n] = s[n];
  return dst;
}

void *carbon_memory_copy_raw(void *dst, const void *src, usz n) {
  u8 *d = (u8 *) dst;
  const u8 *s = (const u8 *) src;
  while (n--) *d++ = *s++;
  return dst;
}

i32 carbon_memory_cmp(const void *v1, const void *v2, usz n) {
  const u8 *l = (const u8 *) v1, *r = (const u8 *) v2;
  for (; n && *l == *r; --n, ++l, ++r);
  return n ? *l - *r : 0;
}

void *carbon_memory_set(void *dst, i32 c, usz n) {
  u8 *d = (u8 *) dst;
  while (n--) *d++ = (u8) c;
  return dst;
}
