// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

u8 CBN_SKAP::Create(const char *decl, const char *skap) {
  return carbon_skap_create(decl, skap);
}

cbn::Opt<CBN_SKAP> CBN_SKAP::make(const char *skap) {
  CBN_SKAP handle;
  if (!carbon_skap_open(skap, &handle)) return {};
  return handle;
}

void CBN_SKAP::Free(void) {
  carbon_skap_close(this);
}

void CBN_SKAP::Print(void) const {
  carbon_skap_print(this);
}

usz CBN_SKAP::Count(void) const {
  return carbon_skap_count(this);
}
