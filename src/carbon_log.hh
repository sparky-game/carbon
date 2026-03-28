/*
  $$=====================$$
  ||       Logging       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_COLOR_RESET   "\033[0m"
#define CARBON_COLOR_RED     "\033[1;31m"
#define CARBON_COLOR_GREEN   "\033[1;32m"
#define CARBON_COLOR_YELLOW  "\033[1;33m"
#define CARBON_COLOR_MAGENTA "\033[1;35m"
#define CARBON_COLOR_CYAN    "\033[1;36m"

#ifdef __cplusplus
namespace cbn::log {
  struct Color {
    static constexpr const char *Red     = CARBON_COLOR_RED;
    static constexpr const char *Green   = CARBON_COLOR_GREEN;
    static constexpr const char *Yellow  = CARBON_COLOR_YELLOW;
    static constexpr const char *Magenta = CARBON_COLOR_MAGENTA;
    static constexpr const char *Cyan    = CARBON_COLOR_CYAN;
  };
}
#endif

#define carbon_print(msg, ...)            carbon_log_print(stdout, msg, ##__VA_ARGS__)
#define carbon_eprint(msg, ...)           carbon_log_print(stderr, msg, ##__VA_ARGS__)
#define carbon_cprint(color, msg, ...)    carbon_print(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)
#define carbon_ceprint(color, msg, ...)   carbon_eprint(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)

#define carbon_println(msg, ...)          carbon_print(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_eprintln(msg, ...)         carbon_eprint(carbon_string_fmt("%s\n", msg), ##__VA_ARGS__)
#define carbon_cprintln(color, msg, ...)  carbon_println(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)
#define carbon_ceprintln(color, msg, ...) carbon_eprintln(carbon_string_fmt("%s%s%s", color, msg, CARBON_COLOR_RESET), ##__VA_ARGS__)

#define CBN_DEBUG(msg, ...) carbon_cprintln(CARBON_COLOR_CYAN, carbon_string_fmt("[^] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_INFO(msg, ...)  carbon_cprintln(CARBON_COLOR_YELLOW, carbon_string_fmt("[*] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_WARN(msg, ...)  carbon_cprintln(CARBON_COLOR_MAGENTA, carbon_string_fmt("[?] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)
#define CBN_ERROR(msg, ...) carbon_ceprintln(CARBON_COLOR_RED, carbon_string_fmt("[!] %s:%u :: %s", __FILE__, __LINE__, msg), ##__VA_ARGS__)

// typedef enum {
//   CBN_LOG_TIME_FORMAT_SECS_SINCE_INIT,
//   CBN_LOG_TIME_FORMAT_CURR_TIMESTAMP
// } CBN_Log_TimeFormat;

// CBNDEF void carbon_log_init_timer(CBN_Log_TimeFormat tf);

CBNDEF void carbon_log_print(FILE *stream, const char *fmt, ...);
