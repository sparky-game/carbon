#pragma once

constexpr bool StartsWith(const std::string_view &s, const std::string_view &ss) {
  return 0 == s.compare(0, ss.length(), ss);
}

inline void RunCmd(const char *cmd) {
  assert(0 == std::system(cmd));
}
