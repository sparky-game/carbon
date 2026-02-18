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

struct CBN_Image : CBN_Image_t {
  CBN_Image(const CBN_DrawCanvas &dc)
    : CBN_Image_t{
        .data = carbon_fs_img_32bit_to_8bit(dc.Pixels(), dc.Width(), dc.Height()),
        .metadata = (CBN_Image_Metadata) {
          .width = dc.Width(),
          .height = dc.Height(),
          .channels = 4
        }
      }
  {}

  static CBN_Image FromFile(const char *file) {
    return carbon_fs_read_img_from_file(file);
  }

  void Free(void) {
    carbon_fs_destroy_img(this);
  }

  u8 Save(CBN_FileFormat fmt, const char *file) const {
    return carbon_fs_write_img_to_file(this, fmt, file);
  }
};

#endif
