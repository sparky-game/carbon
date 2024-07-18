#include <iostream>
#include "vendor/gemma.cpp/util/app.h"
#include "vendor/gemma.cpp/util/args.h"
#include "vendor/gemma.cpp/gemma/gemma.h"
#include "vendor/gemma.cpp/evals/benchmark_helper.h"

static constexpr auto system_prompt {
  "Just write the function that has been requested, no main, no examples, no nonsense."
  "When finished writing the function, do not repeat it, just write it once."
  "Do not explain anything, just write the code requested."
  "Do not use any markdown formatting at all, just plain text."
  "Only write the plain text code with no additional formatting."
  "Always use 2 space indenting, no tabs."
  "Taking into consideration all of the instructions above, do what is requested next:"
};

std::vector<int> tokenize(const std::string &prompt, const gcpp::GemmaTokenizer &tokenizer) {
  std::string ctx { std::string(system_prompt) + "\n" + prompt + "\n" };
  std::vector<int> tokens;
  HWY_ASSERT(tokenizer.Encode(ctx, &tokens));
  tokens.insert(tokens.begin(), gcpp::BOS_ID);
  return tokens;
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

  gcpp::AppArgs app(argc, argv);
  app.num_threads = 2;

  if (const char* err { loader.Validate() }) HWY_ABORT("%s", err);
  if (const char* err { inference.Validate() }) HWY_ABORT("%s", err);

  hwy::ThreadPool pool(app.num_threads);
  if (app.num_threads > 10) gcpp::PinWorkersToCores(pool);
  gcpp::Gemma model { gcpp::CreateGemma(loader, pool) };
  gcpp::KVCache kv_cache { gcpp::KVCache::Create(model.Info().model) };
  gcpp::TimingInfo timings;
  std::random_device rand_dev;
  std::mt19937 rand_gen { rand_dev() };

  constexpr auto user_prompt {
    "Write a function in C++ which checks whether a number is prime."
  };
  std::vector<int> prompt { tokenize(user_prompt, model.Tokenizer()) };
  size_t prompt_size { prompt.size() };

  size_t pos {0};
  auto stream_token = [prompt_size, &pos, &model](int token, float) {
    ++pos;
    if (pos > prompt_size and token != gcpp::EOS_ID) {
      std::string tok;
      HWY_ASSERT(model.Tokenizer().Decode(std::vector<int>{token}, &tok));
      std::cout << tok << std::flush;
    }
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
}
