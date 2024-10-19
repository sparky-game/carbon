cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(
  carbon/ai
  DESCRIPTION  ""
  HOMEPAGE_URL "https://github.com/sparky-game/carbon/tree/master/ai"
  LANGUAGES    CXX
)
set(CMAKE_PROJECT_VERSION 0.6-alpha)

set(FETCHCONTENT_QUIET FALSE)
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
FetchContent_Declare(
  llm-model-weights
  URL                 "https://huggingface.co/sparky-game/carbon/resolve/main/weights.sbs"
  URL_HASH            SHA256=4703b49c4e7177a949a5e60d91a5078f81e0d9ce80f0afa4ab4cb9af44fd334c
  DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_Declare(
  llm-model-tokenizer
  URL                 "https://huggingface.co/sparky-game/carbon/resolve/main/tokenizer.spm"
  URL_HASH            SHA256=61a7b147390c64585d6c3543dd6fc636906c9af3865a5548f27f31aee1d4c8e2
  DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(gemma.cpp)
FetchContent_MakeAvailable(llm-model-weights)
FetchContent_MakeAvailable(llm-model-tokenizer)

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
