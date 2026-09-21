// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

namespace cbn::fs {
  inline bool Glob(const char *pattern, auto &&f) {
    return carbon_fs_glob(pattern, [](const auto e){
      auto cb = static_cast<cbn::meta::RemoveRef_t<typeof(f)> *>(e->arg);
      return (*cb)(e);
    }, &f);
  }
}

template <typename T>
auto CBN_List_tt<T>::FromFile(const char *file) -> cbn::Opt<CBN_List_tt> {
  CBN_List_tt data;
  if (!cbn::fs::ReadFile((CBN_List *) &data, file)) return {};
  return data;
}

template <typename T>
bool CBN_List_tt<T>::ToFile(const char *file) const {
  return cbn::fs::WriteFile((CBN_List *)this, file);
}

#endif
