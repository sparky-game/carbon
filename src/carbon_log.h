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
           usz: "%zu",                          \
           isz: "%zd",                          \
           u8: "%hhu",                          \
           i8: "%hhd",                          \
           u16: "%hu",                          \
           i16: "%hd",                          \
           u32: "%u",                           \
           i32: "%d",                           \
           u64: "%llu",                         \
           i64: "%lld",                         \
           f32: "%f",                           \
           f64: "%lf",                          \
           char: "%c",                          \
           char *: "%s",                        \
           const char *: "%s",                  \
           default: "<'Stuff' At %p>")
#else
template <typename T> constexpr const char *carbon_log__var_to_spec(T)  { return "<'Stuff' At %p>"; }
template <> constexpr const char *carbon_log__var_to_spec(usz)          { return "%zu"; }
template <> constexpr const char *carbon_log__var_to_spec(isz)          { return "%zd"; }
template <> constexpr const char *carbon_log__var_to_spec(u8)           { return "%hhu"; }
template <> constexpr const char *carbon_log__var_to_spec(i8)           { return "%hhd"; }
template <> constexpr const char *carbon_log__var_to_spec(u16)          { return "%hu"; }
template <> constexpr const char *carbon_log__var_to_spec(i16)          { return "%hd"; }
template <> constexpr const char *carbon_log__var_to_spec(u32)          { return "%u"; }
template <> constexpr const char *carbon_log__var_to_spec(i32)          { return "%d"; }
template <> constexpr const char *carbon_log__var_to_spec(u64)          { return "%llu"; }
template <> constexpr const char *carbon_log__var_to_spec(i64)          { return "%lld"; }
template <> constexpr const char *carbon_log__var_to_spec(f32)          { return "%f"; }
template <> constexpr const char *carbon_log__var_to_spec(f64)          { return "%lf"; }
template <> constexpr const char *carbon_log__var_to_spec(char)         { return "%c"; }
template <> constexpr const char *carbon_log__var_to_spec(char *)       { return "%s"; }
template <> constexpr const char *carbon_log__var_to_spec(const char *) { return "%s"; }
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
