cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(
  carbon/ai
  DESCRIPTION  ""
  HOMEPAGE_URL "https://github.com/sparky-game/carbon/tree/master/ai"
  LANGUAGES    CXX
)
set(CMAKE_PROJECT_VERSION 0.6-alpha)

set(HWY_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(SPM_ENABLE_SHARED OFF CACHE BOOL "" FORCE)
set(GEMMA_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  gemma.cpp
  GIT_REPOSITORY "https://github.com/google/gemma.cpp"
  GIT_TAG        main
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(gemma.cpp)

set_target_properties(
  gemma
  benchmark
  benchmarks
  debug_prompt
  single_benchmark
  compress_weights
  PROPERTIES
  EXCLUDE_FROM_ALL TRUE
)
target_compile_options(sentencepiece-static PRIVATE "-w")

add_executable(testgen testgen.cc)
target_link_libraries(testgen libgemma)
