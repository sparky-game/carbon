#ifdef __cplusplus

struct CBN_StrBuilder : CBN_StrBuilder_t {
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

  void Push(const CBN_StrView &sv) {
    carbon_strbuilder_add_strview(this, sv);
  }

  CBN_StrView ToString(void) const {
    return CBN_StrView::make(*this);
  }

  CBN_StrBuilder &operator<<(const char *s) {
    Push(s);
    return *this;
  }
  
  CBN_StrBuilder &operator<<(const CBN_StrView &sv) {
    Push(sv);
    return *this;
  }
};

#endif
