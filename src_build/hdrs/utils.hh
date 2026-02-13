#pragma once

constexpr bool StartsWith(const std::string_view &s, const std::string_view &ss) {
  return 0 == s.compare(0, ss.length(), ss);
}

inline void RunCmd(const char *cmd) {
  assert(0 == std::system(cmd));
}

inline void RunMetaprogram(const char *exe) {
  printf("  EXEC    %s\n", exe);
  RunCmd(std::format(CXX_CMD " {0}.cc " LIB_FILE " " LDFLAGS " -o {0}", exe).c_str());
  RunCmd(exe);
  assert(fs::remove(exe));
}
