#pragma once

constexpr bool StartsWith(const std::string_view &s, const std::string_view &ss) {
  return 0 == s.compare(0, ss.length(), ss);
}

inline void RunCmd(const char *cmd) {
  assert(0 == std::system(cmd));
}

inline void RunMetaprogram(fs::path &&exe) {
  const auto s_exe = exe.make_preferred().string();
  printf("  EXEC    %s\n", s_exe.c_str());
  RunCmd(std::format(CXX_CMD " {0}.cc " LIB_FILE " " LDFLAGS " -o {0}", s_exe).c_str());
  RunCmd(s_exe.c_str());
#ifdef _WIN32
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(500ms);
  assert(fs::remove(std::format("{}.exe", s_exe).c_str()));
#else
  assert(fs::remove(s_exe));
#endif
}
