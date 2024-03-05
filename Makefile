##################
# === MACROS === #
##################
NAME = carbon

include config.mk

# Pretty Printing Output (PPO)
PPO_MKDIR = MKDIR
PPO_CLEAN = CLEAN
PPO_CC    = CC
PPO_AR    = AR
PPO_LD    = LD

# Directories
SRC_DIR  = src
HDR_DIR  = include
TEST_DIR = test
BUILD_DIR = build
TEST_BUILD_DIR = $(BUILD_DIR)/$(TEST_DIR)

# Files
SRCS      := $(wildcard $(SRC_DIR)/*.c)
HDRS      := $(wildcard $(HDR_DIR)/*.h)
OBJS      := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TEST_SRCS := $(wildcard $(TEST_DIR)/$(SRC_DIR)/*.c)
TEST_HDRS := $(wildcard $(TEST_DIR)/$(HDR_DIR)/*.h)
TEST_OBJS := $(patsubst $(TEST_DIR)/$(SRC_DIR)/%.c, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS))

# Build flags
CC            = gcc
AR            = ar -rc
COV           = --coverage
CPPFLAGS      = -I $(HDR_DIR)
CPPFLAGS_TEST = $(CPPFLAGS) -I $(TEST_DIR)/$(HDR_DIR)
CFLAGS        = -Wall -Wextra -pedantic -Werror -fanalyzer -ggdb
LDFLAGS_TEST  = -Wl,--build-id -L $(BUILD_DIR) -l$(NAME)

# Build output
OUT      = $(BUILD_DIR)/lib$(NAME).a
TEST_OUT = $(TEST_BUILD_DIR)/test

# Build targets
TGTS = carbon check
DIR_TGTS = $(BUILD_DIR) $(TEST_BUILD_DIR)


###################
# === TARGETS === #
###################
.PHONY: all $(TGTS) clean

all: $(TGTS)
	$(Q):

carbon: $(BUILD_DIR) $(OUT)
	@echo "Carbon: $(OUT) is ready"

check: $(TEST_BUILD_DIR) $(TEST_OUT)
	@echo
	$(Q)./$(TEST_OUT)
	@echo
	$(Q)gcov -n -abdkq $(OBJS)

$(BUILD_DIR):
	@echo "  $(PPO_MKDIR)   $@"
	$(Q)mkdir -p $@

$(TEST_BUILD_DIR):
	@echo "  $(PPO_MKDIR)   $@"
	$(Q)mkdir -p $@

# ****************** 'carbon': link ****************** #
$(OUT): $(OBJS)
	@echo "  $(PPO_AR)      $@"
	$(Q)$(AR) $@ $^
# **************************************************** #

# ****************** 'check': link ******************* #
$(TEST_OUT): $(TEST_OBJS)
	@echo "  $(PPO_LD)      $@"
	$(Q)$(CC) $(COV) $^ $(LDFLAGS_TEST) -o $@
# **************************************************** #

# *********** 'carbon': compile & assembly *********** #
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "  $(PPO_CC)      $@"
	$(Q)$(CC) $(COV) $(CPPFLAGS) $(CFLAGS) -c -MD $< -o $@

-include $(BUILD_DIR)/*.d
# **************************************************** #

# *********** 'check': compile & assembly ************ #
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/$(SRC_DIR)/%.c
	@echo "  $(PPO_CC)      $@"
	$(Q)$(CC) $(CPPFLAGS_TEST) $(CFLAGS) -c -MD $< -o $@

-include $(TEST_BUILD_DIR)/*.d
# **************************************************** #

clean:
	@if [ -d $(BUILD_DIR) ]; then           \
	  echo "  $(PPO_CLEAN)   $(BUILD_DIR)"; \
	  rm -r $(BUILD_DIR);                   \
	fi
