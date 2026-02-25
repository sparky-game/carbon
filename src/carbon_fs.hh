#ifdef __cplusplus

struct CBN_PatternMatchedFiles : CBN_PatternMatchedFiles_t {
  using iterator = char **;

  iterator begin(void) const {
    return files;
  }

  iterator end(void) const {
    return files + count;
  }
};

cbn::Opt<cbn::str::Builder> cbn::str::Builder::FromFile(const char *file) {
  cbn::str::Builder sb;
  if (!cbn::fs::ReadFile(&sb, file)) return {};
  return sb;
}

bool cbn::str::Builder::ToFile(const char *file) const {
  return cbn::fs::WriteFile(this, file);
}

#endif
