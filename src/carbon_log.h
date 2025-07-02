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

#define carbon_log_debug(msg, ...) carbon_println(CARBON_COLOR_CYAN "[^] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_info(msg, ...) carbon_println(CARBON_COLOR_YELLOW "[*] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_warn(msg, ...) carbon_println(CARBON_COLOR_MAGENTA "[?] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_log_error(msg, ...) carbon_eprintln(CARBON_COLOR_RED "[!] " __FILE__ ":" CARBON_EXPAND_AND_QUOTE(__LINE__) " :: " msg CARBON_COLOR_RESET, ##__VA_ARGS__)

#ifndef __cplusplus
#define carbon_log__var_to_spec(x)              \
  _Generic((x),                                 \
           usz:          "%zu",                 \
           isz:          "%zd",                 \
           u8:           "%hhu",                \
           i8:           "%hhd",                \
           u16:          "%hu",                 \
           i16:          "%hd",                 \
           u32:          "%u",                  \
           i32:          "%d",                  \
           u64:          "%llu",                \
           i64:          "%lld",                \
           f32:          "%f",                  \
           f64:          "%lf",                 \
           char:         "%c",                  \
           char *:       "%s",                  \
           const char *: "%s",                  \
           default:      "<'Stuff' At %p>")
#else
template <typename T>
constexpr auto carbon_log__var_to_spec(T) {
  if constexpr      (CARBON_TYPE_IS_SAME(T, usz))          return "%zu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, isz))          return "%zd";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u8))           return "%hhu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i8))           return "%hhd";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u16))          return "%hu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i16))          return "%hd";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u32))          return "%u";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i32))          return "%d";
  else if constexpr (CARBON_TYPE_IS_SAME(T, u64))          return "%llu";
  else if constexpr (CARBON_TYPE_IS_SAME(T, i64))          return "%lld";
  else if constexpr (CARBON_TYPE_IS_SAME(T, f32))          return "%f";
  else if constexpr (CARBON_TYPE_IS_SAME(T, f64))          return "%lf";
  else if constexpr (CARBON_TYPE_IS_SAME(T, char))         return "%c";
  else if constexpr (CARBON_TYPE_IS_SAME(T, char *))       return "%s";
  else if constexpr (CARBON_TYPE_IS_SAME(T, const char *)) return "%s";
  else return "<'Stuff' At %p>";
}
#endif

#define $(x) (CBN_Log_BoxedVar){carbon_log__var_to_spec(x), (void *) &x}

#define carbon_print(msg, ...) carbon_log_print(stdout, msg, ##__VA_ARGS__)
#define carbon_eprint(msg, ...) carbon_log_print(stderr, msg, ##__VA_ARGS__)
#define carbon_println(msg, ...) carbon_print(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_eprintln(msg, ...) carbon_eprint(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)

#define carbon_cprint(color, msg, ...) carbon_print(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_ceprint(color, msg, ...) carbon_eprint(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_cprintln(color, msg, ...) carbon_println(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)
#define carbon_ceprintln(color, msg, ...) carbon_eprintln(color msg CARBON_COLOR_RESET, ##__VA_ARGS__)

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
