#ifdef __cplusplus

struct CBN_StrBuilder : CBN_StrBuilder_t {
  static CBN_StrBuilder New(void) {
    auto l = carbon_list_create(sizeof(char));
    return *(CBN_StrBuilder *) &l;
  }

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
