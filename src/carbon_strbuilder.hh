#ifdef __cplusplus

struct CBN_StrBuilder : CBN_StrBuilder_t {
  constexpr CBN_StrBuilder(void) : CBN_StrBuilder_t{} {}

  static cbn::Opt<CBN_StrBuilder> FromFile(const char *file);

  void Free(void) {
    carbon_strbuilder_free(this);
  }

  void Push(void) {
    carbon_strbuilder_add_null(this);
  }

  void Push(const char *s) {
    carbon_strbuilder_add_cstr(this, s);
  }

  CBN_StrBuilder &operator<<(const char *s) {
    Push(s);
    return *this;
  }

  void Push(const CBN_StrView &sv) {
    carbon_strbuilder_add_strview(this, sv);
  }

  CBN_StrBuilder &operator<<(const CBN_StrView &sv) {
    Push(sv);
    return *this;
  }

  CBN_StrView ToString(void) const {
    return CBN_StrView::make(*this);
  }

  bool ToFile(const char *file) const;
};

#endif
