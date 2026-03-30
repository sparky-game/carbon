// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
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

template <typename T>
inline auto CBN_List_tt<T>::FromFile(const char *file) -> cbn::Opt<CBN_List_tt> {
  CBN_List_tt data;
  if (!cbn::fs::ReadFile((CBN_List *) &data, file)) return {};
  return data;
}

template <typename T>
inline bool CBN_List_tt<T>::ToFile(const char *file) const {
  return cbn::fs::WriteFile((CBN_List *)this, file);
}

#endif
