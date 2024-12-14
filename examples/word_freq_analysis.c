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

int main(void) {
  // Read file
  CBN_StrBuilder sb = {0};
  if (!carbon_fs_read_entire_file(&sb, "assets/shakespeare.txt")) return 1;

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
