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
  if (argc < 2) {
    carbon_log_error("usage: %$ [FILE]", $(argv[0]));
    return 1;
  }
  if (!carbon_fs_is_regular_file(argv[1])) {
    carbon_log_error("argument (`%$`) needs to be a regular file", $(argv[1]));
    return 1;
  }

  // Read file
  CBN_StrBuilder sb = {0};
  if (!carbon_fs_read_entire_file(&sb, argv[1])) {
    carbon_log_error("file (`%$`) could not be read", $(argv[1]));
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
  usz top_max = 10;
  carbon_println("Top %$ tokens:", $(top_max));
  for (usz i = 0; i < top_max; ++i) {
    const char *token = carbon_strview_to_cstr(points[i].token);
    carbon_println("  - [%$] `%$` (freq = %$)", $(i), $(token), $(points[i].freq));
  }

  // Cleanup
  CARBON_FREE(points);
  carbon_strlist_destroy(&keys);
  carbon_hashmap_destroy(&hist);
  carbon_strbuilder_free(&sb);
  return 0;
}
