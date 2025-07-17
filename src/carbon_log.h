/*
**  $$=====================$$
**  ||       Logging       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_COLOR_RESET   "\033[0m"
#define CARBON_COLOR_RED     "\033[1;31m"
#define CARBON_COLOR_GREEN   "\033[1;32m"
#define CARBON_COLOR_YELLOW  "\033[1;33m"
#define CARBON_COLOR_MAGENTA "\033[1;35m"
#define CARBON_COLOR_CYAN    "\033[1;36m"

#ifdef __cplusplus
struct CBN_Log_Color {
  static constexpr const char *Red     = CARBON_COLOR_RED;
  static constexpr const char *Green   = CARBON_COLOR_GREEN;
  static constexpr const char *Yellow  = CARBON_COLOR_YELLOW;
  static constexpr const char *Magenta = CARBON_COLOR_MAGENTA;
  static constexpr const char *Cyan    = CARBON_COLOR_CYAN;
};
#endif

#define CBN_DEBUG(msg, ...) carbon_cprintln(CARBON_COLOR_CYAN, carbon_string_fmt("[^] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_INFO(msg, ...)  carbon_cprintln(CARBON_COLOR_YELLOW, carbon_string_fmt("[*] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_WARN(msg, ...)  carbon_cprintln(CARBON_COLOR_MAGENTA, carbon_string_fmt("[?] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_ERROR(msg, ...) carbon_ceprintln(CARBON_COLOR_RED, carbon_string_fmt("[!] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)

#ifndef __cplusplus
#ifdef _WIN32
#define carbon_log__var_to_spec__platform_t
#else
#define carbon_log__var_to_spec__platform_t usz: "%zu", isz: "%zd",
#endif
#define carbon_log__var_to_spec(x)              \
  _Generic((x),                                 \
           u8:           "%hhu",                \
           i8:           "%hhd",                \
           u16:          "%hu",                 \
           i16:          "%hd",                 \
           u32:          "%u",                  \
           i32:          "%d",                  \
           u64:          "%llu",                \
           i64:          "%lld",                \
           carbon_log__var_to_spec__platform_t  \
           f32:          "%f",                  \
           f64:          "%lf",                 \
           char:         "%c",                  \
           char *:       "%s",                  \
           const char *: "%s",                  \
           default:      "<'Stuff' At %p>")
#else
template <typename T>
constexpr auto carbon_log__var_to_spec(T) {
  if constexpr      (CARBON_TYPE_IS_SAME(T, u8))           return "%hhu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i8))           return "%hhd";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u16))          return "%hu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i16))          return "%hd";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u32))          return "%u";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i32))          return "%d";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u64))          return "%llu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i64))          return "%lld";
#ifndef _WIN32
  else if constexpr (CARBON_TYPE_IS_SAME(T, usz))          return "%zu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, isz))          return "%zd";
#endif
  else if constexpr (CARBON_TYPE_IS_SAME(T, f32))          return "%f";
  else if constexpr (CARBON_TYPE_IS_SAME(T, f64))          return "%lf";
  else if constexpr (CARBON_TYPE_IS_SAME(T, char))         return "%c";
  else if constexpr (CARBON_TYPE_IS_SAME(T, char *))       return "%s";
  else if constexpr (CARBON_TYPE_IS_SAME(T, const char *)) return "%s";
  else return "<'Stuff' At %p>";
}
#endif

#define $(x) (CBN_Log_BoxedVar){carbon_log__var_to_spec(x), (void *) &x}

#define carbon_print(msg, ...)            carbon_log_print(stdout, msg, ##__VA_ARGS__)
#define carbon_eprint(msg, ...)           carbon_log_print(stderr, msg, ##__VA_ARGS__)
#define carbon_println(msg, ...)          carbon_print(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_eprintln(msg, ...)         carbon_eprint(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_cprint(color, msg, ...)    carbon_print(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)
#define carbon_ceprint(color, msg, ...)   carbon_eprint(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)
#define carbon_cprintln(color, msg, ...)  carbon_println(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)
#define carbon_ceprintln(color, msg, ...) carbon_eprintln(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)

typedef struct {
  const char *spec;
  void *var;
} CBN_Log_BoxedVar;

typedef enum {
  CBN_LOG_TIME_FORMAT_SECS_SINCE_INIT,
  CBN_LOG_TIME_FORMAT_CURR_TIMESTAMP
} CBN_Log_TimeFormat;

CARBON_API void carbon_log_init_timer(CBN_Log_TimeFormat tf);

CARBON_API void carbon_log_print(FILE *stream, const char *fmt, ...);

// Local Variables:
// mode: c++
// End:
