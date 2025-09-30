// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::Opt<CBN_StrBuilder> CBN_StrBuilder::make(const char *file) {
  CBN_StrBuilder sb = {};
  if (!carbon_fs_read_entire_file(&sb, file)) return {};
  return sb;
}

void CBN_StrBuilder::Free(void) {
  carbon_strbuilder_free(this);
}

void CBN_StrBuilder::Push(void) {
  carbon_strbuilder_add_null(this);
}

void CBN_StrBuilder::Push(const char *s) {
  carbon_strbuilder_add_cstr(this, s);
}

void CBN_StrBuilder::Push(const CBN_StrView &sv) {
  carbon_strbuilder_add_strview(this, sv);
}

CBN_StrView CBN_StrBuilder::ToString(void) const {
  return CBN_StrView::make(*this);
}

CBN_StrBuilder &CBN_StrBuilder::operator<<(const char *s) {
  Push(s);
  return *this;
}

CBN_StrBuilder &CBN_StrBuilder::operator<<(const CBN_StrView &sv) {
  Push(sv);
  return *this;
}
