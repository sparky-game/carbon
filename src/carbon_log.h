/*
  $$=====================$$
  ||       Logging       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_LOG_RESET   "\033[0m"
#define CARBON_LOG_RED     "\033[1;31m"
#define CARBON_LOG_GREEN   "\033[1;32m"
#define CARBON_LOG_YELLOW  "\033[1;33m"
#define CARBON_LOG_MAGENTA "\033[1;35m"
#define CARBON_LOG_CYAN    "\033[1;36m"

#define print(msg, ...)            carbon_log_print(1, msg, ##__VA_ARGS__)
#define eprint(msg, ...)           carbon_log_print(2, msg, ##__VA_ARGS__)
#define println(msg, ...)          print(msg "\n", ##__VA_ARGS__)
#define eprintln(msg, ...)         eprint(msg "\n", ##__VA_ARGS__)
#define cprint(color, msg, ...)    print(color msg CARBON_LOG_RESET, ##__VA_ARGS__)
#define ceprint(color, msg, ...)   eprint(color msg CARBON_LOG_RESET, ##__VA_ARGS__)
#define cprintln(color, msg, ...)  println(color msg CARBON_LOG_RESET, ##__VA_ARGS__)
#define ceprintln(color, msg, ...) eprintln(color msg CARBON_LOG_RESET, ##__VA_ARGS__)

#define CBN_DEBUG(msg, ...) cprintln(CARBON_LOG_CYAN, "[^] %s:%u :: " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define CBN_INFO(msg, ...)  cprintln(CARBON_LOG_YELLOW, "[*] %s:%u :: " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define CBN_WARN(msg, ...)  cprintln(CARBON_LOG_MAGENTA, "[?] %s:%u :: " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define CBN_ERROR(msg, ...) ceprintln(CARBON_LOG_RED, "[!] %s:%u :: " msg, __FILE__, __LINE__, ##__VA_ARGS__)

// typedef enum {
//   CBN_LOG_TIME_FORMAT_SECS_SINCE_INIT,
//   CBN_LOG_TIME_FORMAT_CURR_TIMESTAMP
// } CBN_Log_TimeFormat;

// CBNDEF void carbon_log_init_timer(CBN_Log_TimeFormat tf);

/**
 */
CBNDEF void carbon_log_write(i32 fd, CBN_Span msg);

/**
 */
CBNDEF void carbon_log_print(i32 fd, const char *s, ...) CBNFMT(2, 3);
