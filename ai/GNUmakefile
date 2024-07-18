PPO_MKDIR = MKDIR
PPO_CLEAN = CLEAN
PPO_MERGE = MERGE
PPO_CXX   = CXX
PPO_LD    = LD

CXX           = clang++
OPTIMIZATIONS = -pipe -O3

HIGHWAY_HDR_DIR   = vendor/highway
HIGHWAY_SRC_DIR   = $(HIGHWAY_HDR_DIR)/hwy
HIGHWAY_BUILD_DIR = build/highway
HIGHWAY_SRCS     := $(wildcard $(HIGHWAY_SRC_DIR)/contrib/sort/vqsort*.cc) $(addsuffix .cc, $(addprefix $(HIGHWAY_SRC_DIR)/, abort aligned_allocator nanobenchmark per_target print targets timer)) $(HIGHWAY_SRC_DIR)/contrib/thread_pool/topology.cc
HIGHWAY_OBJS     := $(patsubst $(HIGHWAY_SRC_DIR)/%.cc, $(HIGHWAY_BUILD_DIR)/%.o, $(HIGHWAY_SRCS))
HIGHWAY_OBJS     := $(patsubst $(HIGHWAY_BUILD_DIR)/contrib/sort/%.o, $(HIGHWAY_BUILD_DIR)/%.o, $(HIGHWAY_OBJS))
HIGHWAY_OBJS     := $(patsubst $(HIGHWAY_BUILD_DIR)/contrib/thread_pool/%.o, $(HIGHWAY_BUILD_DIR)/%.o, $(HIGHWAY_OBJS))
HIGHWAY_CPPFLAGS  = -isystem $(HIGHWAY_HDR_DIR)
HIGHWAY_CXXFLAGS  = -std=c++11 $(OPTIMIZATIONS)

SENTENCEPIECE_HDR_DIR   = vendor/sentencepiece
SENTENCEPIECE_SRC_DIR   = $(SENTENCEPIECE_HDR_DIR)/src
SENTENCEPIECE_BUILD_DIR = build/sentencepiece
SENTENCEPIECE_SRCS     := $(wildcard $(SENTENCEPIECE_HDR_DIR)/third_party/protobuf-lite/*.cc) $(SENTENCEPIECE_HDR_DIR)/third_party/absl/flags/flag.cc $(addsuffix .pb.cc, $(addprefix $(SENTENCEPIECE_SRC_DIR)/builtin_pb/, sentencepiece sentencepiece_model)) $(addsuffix .cc, $(addprefix $(SENTENCEPIECE_SRC_DIR)/, bpe_model char_model error filesystem model_factory model_interface normalizer sentencepiece_processor unigram_model util word_model))
SENTENCEPIECE_OBJS     := $(patsubst $(SENTENCEPIECE_SRC_DIR)/%.cc, $(SENTENCEPIECE_BUILD_DIR)/%.o, $(SENTENCEPIECE_SRCS))
SENTENCEPIECE_OBJS     := $(patsubst $(SENTENCEPIECE_BUILD_DIR)/builtin_pb/%.o, $(SENTENCEPIECE_BUILD_DIR)/%.o, $(SENTENCEPIECE_OBJS))
SENTENCEPIECE_OBJS     := $(patsubst $(SENTENCEPIECE_HDR_DIR)/third_party/protobuf-lite/%.cc, $(SENTENCEPIECE_BUILD_DIR)/%.o, $(SENTENCEPIECE_OBJS))
SENTENCEPIECE_OBJS     := $(patsubst $(SENTENCEPIECE_HDR_DIR)/third_party/absl/flags/%.cc, $(SENTENCEPIECE_BUILD_DIR)/%.o, $(SENTENCEPIECE_OBJS))
SENTENCEPIECE_CPPFLAGS  = -D HAVE_PTHREAD -isystem $(SENTENCEPIECE_HDR_DIR) -isystem $(SENTENCEPIECE_SRC_DIR)/builtin_pb -isystem $(SENTENCEPIECE_HDR_DIR)/third_party/protobuf-lite
SENTENCEPIECE_CXXFLAGS  = -std=c++17 $(OPTIMIZATIONS)
SENTENCEPIECE_VERSION  := $(shell cat $(SENTENCEPIECE_HDR_DIR)/VERSION.txt)

GEMMACPP_HDR_DIR   = vendor/gemma.cpp
GEMMACPP_SRC_DIR   = $(GEMMACPP_HDR_DIR)
GEMMACPP_BUILD_DIR = build/gemma.cpp
GEMMACPP_SRCS     := $(addsuffix .cc, $(addprefix $(GEMMACPP_SRC_DIR)/backprop/, backward forward optimizer)) $(addsuffix .cc, $(addprefix $(GEMMACPP_SRC_DIR)/compression/, blob_store io_win io)) $(addsuffix .cc, $(addprefix $(GEMMACPP_SRC_DIR)/evals/, benchmark_helper cross_entropy)) $(wildcard $(GEMMACPP_SRC_DIR)/gemma/instantiations/*.cc) $(addsuffix .cc, $(addprefix $(GEMMACPP_SRC_DIR)/gemma/, common gemma kv_cache tokenizer weights))
GEMMACPP_OBJS     := $(patsubst $(GEMMACPP_SRC_DIR)/gemma/%.cc, $(GEMMACPP_BUILD_DIR)/%.o, $(GEMMACPP_SRCS))
GEMMACPP_OBJS     := $(patsubst $(GEMMACPP_BUILD_DIR)/instantiations/%.o, $(GEMMACPP_BUILD_DIR)/%.o, $(GEMMACPP_OBJS))
GEMMACPP_OBJS     := $(patsubst $(GEMMACPP_SRC_DIR)/evals/%.cc, $(GEMMACPP_BUILD_DIR)/%.o, $(GEMMACPP_OBJS))
GEMMACPP_OBJS     := $(patsubst $(GEMMACPP_SRC_DIR)/backprop/%.cc, $(GEMMACPP_BUILD_DIR)/%.o, $(GEMMACPP_OBJS))
GEMMACPP_OBJS     := $(patsubst $(GEMMACPP_SRC_DIR)/compression/%.cc, $(GEMMACPP_BUILD_DIR)/%.o, $(GEMMACPP_OBJS))
GEMMACPP_CPPFLAGS  = -isystem $(GEMMACPP_HDR_DIR) -isystem $(HIGHWAY_HDR_DIR) -isystem $(SENTENCEPIECE_HDR_DIR)
GEMMACPP_CXXFLAGS  = -std=c++17 $(OPTIMIZATIONS)

TESTGEN_BUILD_DIR   = build
TESTGEN_WEIGHTS_IN := $(wildcard model/weights.sbs.*)
TESTGEN_WEIGHTS_OUT = $(TESTGEN_BUILD_DIR)/weights.sbs
TESTGEN_SRCS        = testgen.cc
TESTGEN_OBJS        = $(patsubst %.cc, $(TESTGEN_BUILD_DIR)/%.o, $(TESTGEN_SRCS))
TESTGEN_CPPFLAGS    = -isystem $(HIGHWAY_HDR_DIR) -isystem $(GEMMACPP_HDR_DIR)
TESTGEN_CXXFLAGS    = -std=c++20 -Wall -Wextra -Wpedantic -Werror $(OPTIMIZATIONS)
TESTGEN_LDFLAGS     = -static $(OPTIMIZATIONS)
TESTGEN_OUT         = testgen

DIRS_OUT = $(TESTGEN_BUILD_DIR) $(HIGHWAY_BUILD_DIR) $(SENTENCEPIECE_BUILD_DIR) $(GEMMACPP_BUILD_DIR)

.PHONY: all clean mrproper

all: $(DIRS_OUT) $(TESTGEN_WEIGHTS_OUT) $(TESTGEN_OUT)
	@:

$(DIRS_OUT):
	@echo "  $(PPO_MKDIR)   $@"
	@mkdir -p $@

$(TESTGEN_WEIGHTS_OUT): $(TESTGEN_WEIGHTS_IN)
	@for i in $^; do                     \
	  echo "  $(PPO_MERGE)   $$i >> $@"; \
	  cat $$i >> $@;                     \
	done

$(TESTGEN_OUT): $(HIGHWAY_OBJS) $(SENTENCEPIECE_OBJS) $(GEMMACPP_OBJS) $(TESTGEN_OBJS)
	@echo "  $(PPO_LD)      $@"
	@$(CXX) $^ $(TESTGEN_LDFLAGS) -o $@

$(HIGHWAY_BUILD_DIR)/%.o: $(HIGHWAY_SRC_DIR)/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(HIGHWAY_CPPFLAGS) $(HIGHWAY_CXXFLAGS) -c -MD $< -o $@

$(HIGHWAY_BUILD_DIR)/%.o: $(HIGHWAY_SRC_DIR)/contrib/sort/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(HIGHWAY_CPPFLAGS) $(HIGHWAY_CXXFLAGS) -c -MD $< -o $@

$(HIGHWAY_BUILD_DIR)/%.o: $(HIGHWAY_SRC_DIR)/contrib/thread_pool/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(HIGHWAY_CPPFLAGS) $(HIGHWAY_CXXFLAGS) -c -MD $< -o $@

$(SENTENCEPIECE_BUILD_DIR)/%.o: $(SENTENCEPIECE_SRC_DIR)/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(SENTENCEPIECE_CPPFLAGS) $(SENTENCEPIECE_CXXFLAGS) -c -MD $< -o $@

$(SENTENCEPIECE_BUILD_DIR)/%.o: $(SENTENCEPIECE_SRC_DIR)/builtin_pb/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(SENTENCEPIECE_CPPFLAGS) $(SENTENCEPIECE_CXXFLAGS) -c -MD $< -o $@

$(SENTENCEPIECE_BUILD_DIR)/%.o: $(SENTENCEPIECE_HDR_DIR)/third_party/protobuf-lite/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(SENTENCEPIECE_CPPFLAGS) $(SENTENCEPIECE_CXXFLAGS) -c -MD $< -o $@

$(SENTENCEPIECE_BUILD_DIR)/%.o: $(SENTENCEPIECE_HDR_DIR)/third_party/absl/flags/%.cc
	@echo "  $(PPO_CXX)     $@"
	@[ ! -e $(SENTENCEPIECE_HDR_DIR)/config.h ] && cp $(SENTENCEPIECE_HDR_DIR)/config.h.in $(SENTENCEPIECE_HDR_DIR)/config.h || true
	@sed -i -e 's/@PROJECT_VERSION@/$(SENTENCEPIECE_VERSION)/' -e 's/@PROJECT_NAME@/sentencepiece/' $(SENTENCEPIECE_HDR_DIR)/config.h
	@$(CXX) $(SENTENCEPIECE_CPPFLAGS) $(SENTENCEPIECE_CXXFLAGS) -c -MD $< -o $@

$(GEMMACPP_BUILD_DIR)/%.o: $(GEMMACPP_SRC_DIR)/gemma/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(GEMMACPP_CPPFLAGS) $(GEMMACPP_CXXFLAGS) -c -MD $< -o $@

$(GEMMACPP_BUILD_DIR)/%.o: $(GEMMACPP_SRC_DIR)/gemma/instantiations/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(GEMMACPP_CPPFLAGS) $(GEMMACPP_CXXFLAGS) -c -MD $< -o $@

$(GEMMACPP_BUILD_DIR)/%.o: $(GEMMACPP_SRC_DIR)/evals/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(GEMMACPP_CPPFLAGS) $(GEMMACPP_CXXFLAGS) -c -MD $< -o $@

$(GEMMACPP_BUILD_DIR)/%.o: $(GEMMACPP_SRC_DIR)/backprop/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(GEMMACPP_CPPFLAGS) $(GEMMACPP_CXXFLAGS) -c -MD $< -o $@

$(GEMMACPP_BUILD_DIR)/%.o: $(GEMMACPP_SRC_DIR)/compression/%.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(GEMMACPP_CPPFLAGS) $(GEMMACPP_CXXFLAGS) -c -MD $< -o $@

$(TESTGEN_BUILD_DIR)/%.o: %.cc
	@echo "  $(PPO_CXX)     $@"
	@$(CXX) $(TESTGEN_CPPFLAGS) $(TESTGEN_CXXFLAGS) -c -MD $< -o $@

-include $(TESTGEN_BUILD_DIR)/*.d
-include $(HIGHWAY_BUILD_DIR)/*.d
-include $(GEMMACPP_BUILD_DIR)/*.d
-include $(SENTENCEPIECE_BUILD_DIR)/*.d

clean:
	@if [ -d $(TESTGEN_BUILD_DIR) ]; then           \
	  echo "  $(PPO_CLEAN)   $(TESTGEN_BUILD_DIR)"; \
	  rm -r $(TESTGEN_BUILD_DIR);                   \
	fi

mrproper: clean
	@if [ -e testgen ]; then           \
	  echo "  $(PPO_CLEAN)   testgen"; \
	  rm testgen;                      \
	fi
