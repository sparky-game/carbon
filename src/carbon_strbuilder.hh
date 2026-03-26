// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_StrBuilder : CBN_StrBuilder_t {
  static cbn::Opt<CBN_StrBuilder> FromFile(const char *file);

  CBN_StrBuilder(void) : CBN_StrBuilder_t{carbon_list_create(sizeof(char))} {}
  CBN_StrBuilder(CBN_StrBuilder &&sb) : CBN_StrBuilder_t{std::move(sb)} {}

  CBN_StrBuilder &operator=(CBN_StrBuilder &&sb) {
    if (this != &sb) {
      Free();
      *(CBN_StrBuilder_t *)this = std::move(sb);
    }
    return *this;
  }

  ~CBN_StrBuilder(void) { Free(); }

  void Free(void) { carbon_strbuilder_free(this); }

  void Push(void) { carbon_strbuilder_add_null(this); }

  void Push(const char *s) { carbon_strbuilder_add_cstr(this, s); }

  void Push(const CBN_StrView &sv) {
    carbon_strbuilder_add_strview(this, sv);
  }

  CBN_StrBuilder &operator<<(auto &&x) {
    Push(cbn::meta::Forward<typeof(x)>(x));
    return *this;
  }

  CBN_StrView ToString(void) const {
    return CBN_StrView(*this);
  }

  bool ToFile(const char *file) const;
};

#endif
