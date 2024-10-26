cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(
  testgen
  DESCRIPTION  ""
  HOMEPAGE_URL ""
  LANGUAGES    CXX
)

set(FETCHCONTENT_QUIET FALSE)
set(HWY_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(SPM_ENABLE_SHARED OFF CACHE BOOL "" FORCE)
set(GEMMA_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

FetchContent_Declare(
  gemma.cpp
  GIT_REPOSITORY "https://github.com/google/gemma.cpp"
  GIT_TAG        main
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
)
FetchContent_Declare(
  llm-model
  URL                        "https://huggingface.co/sparky-game/carbon/resolve/main/model.tgz"
  URL_HASH                   SHA256=6c42d35e1707910c5fd57104f85ff7e16022884deea7160dbe7c6c9c93431556
  DOWNLOAD_DIR               ${CMAKE_CURRENT_BINARY_DIR}
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(gemma.cpp)
FetchContent_MakeAvailable(llm-model)

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

add_executable(${CMAKE_PROJECT_NAME} testgen.cc)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "-static")
target_link_libraries(${CMAKE_PROJECT_NAME} libgemma)
