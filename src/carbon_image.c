// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define STBI_MALLOC  carbon_memory_alloc
#define STBI_REALLOC carbon_memory_realloc
#define STBI_FREE    carbon_memory_free
#define STBI_ASSERT  CBN_ASSERT
#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image/stb_image.h"

#define STBIW_MALLOC  carbon_memory_alloc
#define STBIW_REALLOC carbon_memory_realloc
#define STBIW_FREE    carbon_memory_free
#define STBIW_ASSERT  CBN_ASSERT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../thirdparty/stb_image_write/stb_image_write.h"

CBN_Image carbon_image_from_canvas(const CBN_DrawCanvas *dc) {
  const usz w = carbon_drawcanvas_width(dc), h = carbon_drawcanvas_height(dc);
  return (CBN_Image) {
    .data = carbon_image_32bit_to_8bit(carbon_drawcanvas_pixels(dc), w, h),
    .metadata = (CBN_Image_Metadata) {
      .width = w,
      .height = h,
      .channels = 4
    }
  };
}

CBN_Image carbon_image_read_from_file(const char *file) {
  CBN_Image img;
  carbon_memory_set(&img, 0, sizeof(img));
  img.data = carbon_image_read_from_file_linearly(file, &img.metadata.width, &img.metadata.height, &img.metadata.channels);
  return img;
}

CBN_List carbon_image_read_from_file_as_tensor(const char *file) {
  usz width = 0, height = 0, chs = 0;
  u8 *pixels = carbon_image_read_from_file_linearly(file, &width, &height, &chs);
  CBN_List img = carbon_image_tensorize(pixels, width, height, chs);
  carbon_memory_free(pixels);
  return img;
}

u8 *carbon_image_read_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs) {
  return stbi_load(file, (i32 *) out_width, (i32 *) out_height, (i32 *) out_chs, 0);
}

CBN_List carbon_image_tensorize(u8 *pixels, usz width, usz height, usz chs) {
  CBN_List img = carbon_list_create(sizeof(CBN_Mat));
  for (usz c = 0; c < chs; ++c) {
    u8 *ptr = pixels + c;
    CBN_Mat ch = carbon_math_mat_create(height, width);
    for (usz i = 0; i < ch.rows; ++i) {
      for (usz j = 0; j < ch.cols; ++j) {
        CARBON_MAT_AT(ch, i, j) = *ptr;
        ptr += chs;
      }
    }
    carbon_list_push(&img, &ch);
  }
  return img;
}

u8 *carbon_image_linearize(CBN_List *img) {
  CBN_Mat first_ch = carbon_list_at(CBN_Mat, *img, 0);
  usz n_pixels = first_ch.rows * first_ch.cols;
  u8 *pixels = (u8 *) carbon_memory_alloc(img->size * n_pixels * sizeof(u8));
  usz i = 0, j = 0;
  for (usz k = 0; k < n_pixels; ++k) {
    carbon_list_foreach(CBN_Mat, *img) {
      pixels[(k * img->size) + it.i] = (u8) CARBON_MAT_AT(it.var, i, j);
    }
    ++j;
    if (j == first_ch.cols) j = 0, ++i;
  }
  return pixels;
}

u8 *carbon_image_32bit_to_8bit(const u32 *pixels, const usz width, const usz height) {
  u8 *bytes = (u8 *) carbon_memory_alloc(width * height * 4);
  for (usz i = 0; i < width * height; ++i) {
    u32 color = pixels[i];
    bytes[i * 4 + 0] = (color >> 24) & 0xff;
    bytes[i * 4 + 1] = (color >> 16) & 0xff;
    bytes[i * 4 + 2] = (color >> 8)  & 0xff;
    bytes[i * 4 + 3] = (color >> 0)  & 0xff;
  }
  return bytes;
}

u32 *carbon_image_8bit_to_32bit(const u8 *bytes, const usz width, const usz height) {
  u32 *pixels = (u32 *) carbon_memory_alloc(width * height * 4);
  for (usz i = 0; i < width * height; ++i) {
    u32 r = bytes[i * 4 + 0];
    u32 g = bytes[i * 4 + 1];
    u32 b = bytes[i * 4 + 2];
    u32 a = bytes[i * 4 + 3];
    u32 color = (r << 24) | (g << 16) | (b << 8) | a;
    pixels[i] = color;
  }
  return pixels;
}

bool carbon_image_write_to_file(const CBN_Image *img, CBN_Image_Format fmt, const char *file) {
  return carbon_image_write_to_file_linearly(img->data, fmt, img->metadata.width, img->metadata.height, img->metadata.channels, file);
}

bool carbon_image_write_tensor_to_file(CBN_List *img, CBN_Image_Format fmt, const char *file) {
  CBN_Mat first_ch = carbon_list_at(CBN_Mat, *img, 0);
  u8 *pixels = carbon_image_linearize(img);
  bool result = carbon_image_write_to_file_linearly(pixels, fmt, first_ch.cols, first_ch.rows, img->size, file);
  carbon_memory_free(pixels);
  return result;
}

bool carbon_image_write_to_file_linearly(u8 *pixels, CBN_Image_Format fmt, usz width, usz height, usz chs, const char *file) {
  i32 result = 0;
  switch (fmt) {
  case CBN_IMAGE_FORMAT_PNG:
    result = stbi_write_png(file, width, height, chs, pixels, width * chs);
    break;
  case CBN_IMAGE_FORMAT_BMP:
    result = stbi_write_bmp(file, width, height, chs, pixels);
    break;
  case CBN_IMAGE_FORMAT_TGA:
    result = stbi_write_tga(file, width, height, chs, pixels);
    break;
  case CBN_IMAGE_FORMAT_JPG:
    // TODO: customize the quality value [1, 100] with a macro (maybe `CARBON_FS_WRITE_IMG_JPG_QUALITY`)
    result = stbi_write_jpg(file, width, height, chs, pixels, 90);
    break;
  default: CARBON_UNREACHABLE;
  }
  if (!result) CBN_ERROR("unable to write pixels to file (`%s`)", file);
  // NOTE: maybe just `return result;` is fine?
  return result ? true : false;
}

void carbon_image_destroy(CBN_Image *img) {
  carbon_memory_free(img->data);
  carbon_memory_set(img, 0, sizeof(*img));
}

void carbon_image_destroy_tensor(CBN_List *img) {
  carbon_list_foreach(CBN_Mat, *img) {
    carbon_math_mat_destroy(&it.var);
  }
  carbon_list_destroy(img);
}
