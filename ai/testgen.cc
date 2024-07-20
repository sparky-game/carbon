#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "vendor/gemma.cpp/util/app.h"
#include "vendor/gemma.cpp/util/args.h"
#include "vendor/gemma.cpp/gemma/gemma.h"
#include "vendor/gemma.cpp/evals/benchmark_helper.h"

#define CARBON_AI_ABORT_ERROR(err) HWY_ABORT("%s:%u :: %s", __FILE__, __LINE__, (err))
#define CARBON_AI_ABORT_USAGE HWY_ABORT("usage: %s [-t <N>] --src <FILE> --test <FILE>", argv[0])

static constexpr auto system_prompt {
  "Strictly follow the following instructions and rules:\n"
  "- Just write the function that has been requested, no main, no examples, no nonsense.\n"
  "- When finished writing the function, do not repeat it, just write it once.\n"
  "- Do not explain anything, just write the code requested.\n"
  "- Do not use any markdown formatting at all, just plain text.\n"
  "- Only write the plain text code with no additional formatting.\n"
  "- Always use 2 space indenting, no tabs.\n"
  "- Do not write multiple blocks of code, just one.\n"
  "- Do not use any third-party dependency, just built-in features.\n"
  "Take all of these instructions into consideration while performing as accurate as possible the following requests.\n"
};
static constexpr auto test_code_prompt {
  "Here is a C/C++ unit test translation unit:"
};
static constexpr auto src_code_prompt {
  "And this is the C/C++ translation unit that it tests:"
};
static constexpr auto request_prompt {
  "Write an extended version of the unit test translation unit that includes additional unit tests that will increase the test coverage of the code under test.\n"
};

static inline std::string load_file_contents(const std::string &filepath) {
  std::ifstream ifs { filepath };
  if (not ifs) throw std::runtime_error { "unable to open file for reading (`" + filepath + "`)" };
  std::stringstream ss;
  ss << "```\n" << ifs.rdbuf() << "\n```\n";
  return ss.str();
}

static inline std::vector<int> tokenize(const gcpp::GemmaTokenizer &tokenizer, const std::string &test_code, const std::string &src_code) {
  std::string prompt {
    std::string(system_prompt) + "\n" +
    test_code_prompt + "\n" +
    test_code + "\n" +
    src_code_prompt + "\n" +
    src_code + "\n" +
    request_prompt + "\n"
  };
  std::vector<int> tokens;
  HWY_ASSERT(tokenizer.Encode(prompt, &tokens));
  tokens.insert(tokens.begin(), gcpp::BOS_ID);
  return tokens;
}

static inline void preprocess_output(std::stringstream &ss) {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(ss, line)) lines.emplace_back(line);
  ss.str("");
  ss.clear();
  for (const auto &i : lines) {
    if (i.find("```") == std::string::npos) ss << i << std::endl;
  }
}

static inline bool it_builds(const std::stringstream &ss) {
  std::ofstream ofs { "tmp.cc" };
  if (not ofs) throw std::runtime_error { "unable to open/create file for writing (`./tmp.cc`)" };
  ofs << ss.str();
  ofs.close();
  int result { std::system("clang++ -S tmp.cc -o /dev/null >/dev/null 2>&1") };
  std::remove("tmp.cc");
  if (result != 0) return false;
  return true;
}

int main(int argc, char **argv) {
  gcpp::LoaderArgs loader(argc, argv);
  loader.tokenizer = "model/tokenizer.spm";
  loader.weights = "build/weights.sbs";
  loader.model_type_str = "7b-it";
  loader.weight_type_str = "sfp";

  gcpp::InferenceArgs inference(argc, argv);
  inference.max_tokens = gcpp::kSeqLen;
  inference.max_generated_tokens = 1024;
  inference.temperature = 0.2f;
  inference.deterministic = false;
  inference.multiturn = false;

  gcpp::AppArgs app { argc, argv };
  app.num_threads = 1;
  if (argc >= 3 and argv[1] == std::string("-t")) app.num_threads = std::atoi(argv[2]);
  else std::cout << "WARNING: using 1 thread for inference by default.\n" << std::endl;
  hwy::ThreadPool thread_pool(app.num_threads);
  if (app.num_threads > 10) gcpp::PinWorkersToCores(thread_pool);

  std::string src_file, test_file;
  switch (argc) {
  case 5:
    if (argv[1] != std::string("--src")) CARBON_AI_ABORT_USAGE;
    if (not std::filesystem::exists(argv[2])) CARBON_AI_ABORT_USAGE;
    if (argv[3] != std::string("--test")) CARBON_AI_ABORT_USAGE;
    if (not std::filesystem::exists(argv[4])) CARBON_AI_ABORT_USAGE;
    src_file = argv[2];
    test_file = argv[4];
    break;
  case 7:
    if (argv[3] != std::string("--src")) CARBON_AI_ABORT_USAGE;
    if (not std::filesystem::exists(argv[4])) CARBON_AI_ABORT_USAGE;
    if (argv[5] != std::string("--test")) CARBON_AI_ABORT_USAGE;
    if (not std::filesystem::exists(argv[6])) CARBON_AI_ABORT_USAGE;
    src_file = argv[4];
    test_file = argv[6];
    break;
  default:
    CARBON_AI_ABORT_USAGE;
  }

  if (const char* err { loader.Validate() }) CARBON_AI_ABORT_ERROR(err);
  if (const char* err { inference.Validate() }) CARBON_AI_ABORT_ERROR(err);

  std::string src_file_contents { load_file_contents(src_file) };
  std::string test_file_contents { load_file_contents(test_file) };

  gcpp::Gemma model { gcpp::CreateGemma(loader, thread_pool) };
  gcpp::KVCache kv_cache { gcpp::KVCache::Create(model.Info().model) };
  gcpp::TimingInfo timings;
  std::random_device rand_dev;
  std::mt19937 rand_gen { rand_dev() };

  std::vector<int> prompt { tokenize(model.Tokenizer(), test_file_contents, src_file_contents) };
  size_t prompt_size { prompt.size() };
  size_t pos {0};
  std::stringstream buf;
  auto stream_token = [prompt_size, &pos, &buf, &model](int token, float) {
    ++pos;
    if (pos > prompt_size and token != gcpp::EOS_ID) {
      if (pos == prompt_size + 1) std::cout << std::endl << std::flush;
      std::string tok;
      HWY_ASSERT(model.Tokenizer().Decode(std::vector<int>{token}, &tok));
      buf << tok;
      std::cout << tok << std::flush;
    }
    else std::cout << "." << std::flush;
    return true;
  };

  gcpp::RuntimeConfig runtime_conf {
    .max_tokens = inference.max_tokens,
    .max_generated_tokens = inference.max_generated_tokens,
    .temperature = inference.temperature,
    .gen = &rand_gen,
    .stream_token = stream_token
  };

  model.Generate(runtime_conf, prompt, 0, kv_cache, timings);

  std::cout << "\n\nStats:" << std::endl;
  std::cout << "  prefill_tok_sec:     " << timings.prefill_tok_sec << std::endl;
  std::cout << "  gen_tok_sec:         " << timings.gen_tok_sec << std::endl;
  std::cout << "  time_to_first_token: " << timings.time_to_first_token << std::endl;

  std::cout << "\n\n1. Pre-Process:" << std::endl;
  preprocess_output(buf);
  std::cout << buf.str();

  std::cout << "\n\n2. It builds?:" << std::endl;
  if (it_builds(buf)) std::cout << "Yes. Hurray!" << std::endl;
  else std::cout << "No. Maybe next time..." << std::endl;

  std::ofstream ofs { "tmp.cc" };
  if (not ofs) throw std::runtime_error { "unable to open/create file for writing (`./tmp.cc`)" };
  ofs << buf.str();
  ofs.close();
  [[maybe_unused]] int result { std::system(("diff -u " + test_file + " tmp.cc > testgen.diff").c_str()) };
  std::remove("tmp.cc");
  std::cout << "\nOutput from BSD Carbon AI written to: `testgen.diff`" << std::endl;
}
