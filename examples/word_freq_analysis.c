// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

typedef struct {
  CBN_StrView token;
  usz freq;
} Point;

static inline i32 compare_point_freqs(const void *a, const void *b) {
  return (i32) ((Point *) b)->freq - (i32) ((Point *) a)->freq;
}

int main(int argc, char **argv) {
  // CLI Arguments
  const char *program_name = CARBON_SHIFT_ARGS(argc, argv);
  if (argc < 2) {
    carbon_log_error("usage: %$ [FILE] [N_TOP_WORDS]", $(program_name));
    return 1;
  }
  const char *file = CARBON_SHIFT_ARGS(argc, argv);
  if (!carbon_fs_is_regular_file(file)) {
    carbon_log_error("FILE (`%$`) needs to be a regular file", $(file));
    return 1;
  }
  const char *n_top_words = CARBON_SHIFT_ARGS(argc, argv);
  if (!carbon_string_is_number(n_top_words)) {
    carbon_log_error("N_TOP_WORDS (`%$`) needs to be a number", $(n_top_words));
    return 1;
  }

  // Read file
  CBN_StrBuilder sb = {0};
  if (!carbon_fs_read_entire_file(&sb, file)) {
    carbon_log_error("file (`%$`) could not be read", $(file));
    return 1;
  }

  // Analyze
  CBN_HashMap hist = carbon_hashmap_create(1024*1024, sizeof(usz));
  CBN_StrList keys = carbon_strlist_create(true);
  CBN_StrView txt = carbon_strview_from_strbuilder(&sb);
  usz n_tokens = 0;
  for (; txt.size; ++n_tokens) {
    txt = carbon_strview_trim_left(txt);
    const char *token = carbon_strview_to_cstr(carbon_strview_chop_by_space(&txt));
    carbon_strlist_push(&keys, token);
    usz freq = 0;
    carbon_hashmap_get(&hist, token, &freq);
    ++freq;
    carbon_hashmap_set(&hist, token, &freq);
  }
  carbon_println("Total amount of tokens: %$ (%$ are unique)", $(n_tokens), $(keys.size));
  Point *points = CARBON_CALLOC(keys.size, sizeof(Point));
  carbon_strlist_foreach(keys) {
    points[it.i].token = it.sv;
    carbon_hashmap_get(&hist, carbon_strview_to_cstr(it.sv), &points[it.i].freq);
  }
  qsort(points, keys.size, sizeof(Point), compare_point_freqs);

  // Print most frequent tokens
  usz n_top_words_num = atoi(n_top_words);
  CARBON_ASSERT(n_top_words_num <= keys.size && "N_TOP_WORDS is too big");
  carbon_println("Top %$ tokens:", $(n_top_words_num));
  for (usz i = 0; i < n_top_words_num; ++i) {
    const char *token = carbon_strview_to_cstr(points[i].token);
    carbon_println("  - `%$` (freq = %$)", $(token), $(points[i].freq));
  }

  // Cleanup
  CARBON_FREE(points);
  carbon_strlist_destroy(&keys);
  carbon_hashmap_destroy(&hist);
  carbon_strbuilder_free(&sb);
  return 0;
}
