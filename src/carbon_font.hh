#ifdef __cplusplus

struct CBN_Font : CBN_Font_t {
  static CBN_Font Load(const char *file, usz size) {
    return carbon_font_load(file, size);
  }
};

#endif
