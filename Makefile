# Makefile
# Project: Cask Language
# Derek Tan

# compiler vars
CXX := clang -std=c11
CXXFLAGS := -Wall -Wextra -Werror

ifeq ($(DEBUG_BUILD),1)
	CXXFLAGS += -g
else
	CXXFLAGS += -O2
endif

# executable dir
BIN_DIR := ./bin

# important dirs
BUILD_DIR := ./build
MAIN_DIR := ./mains
SRC_DIR := ./src

# auto generate source targets
SRCS := $(shell find $(SRC_DIR) -name '*.c')
MAINS := $(shell find $(MAIN_DIR) -name '*.c')
ALL_SRCS := $(SRCS) $(MAINS)

# auto generate object targets
SRCS_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
MAINS_OBJS := $(patsubst $(MAIN_DIR)/%.c,$(BUILD_DIR)/%.o,$(MAINS))

# auto generate executable targets
EXECS := $(patsubst $(BUILD_DIR)/%.o,$(BIN_DIR)/%,$(MAINS_OBJS))

# search ./src and ./test for C sources
vpath %.c $(MAIN_DIR)
vpath %.{c,h} $(SRC_DIR)

.PHONY: sloc tell all objs execs clean_all clean_execs

# utility rule: show SLOC
sloc:
	@wc -l $(MAIN_DIR)/*.c $(SRC_DIR)/**/*.h $(SRC_DIR)/*.c

# test rule: show all targets and deps
tell:
	@echo "Module Sources:"
	@echo $(SRCS)
	@echo "Driver Sources:"
	@echo $(MAINS)
	@echo "All Objs"
	@echo $(SRCS_OBJS)
	@echo $(MAINS_OBJS)
	@echo "All Executables:"
	@echo $(EXECS)

# entry build rule
all: objs execs

# object file build stage
objs: $(SRCS_OBJS) $(MAINS_OBJS)

# executable link stage
execs: $(EXECS)

# sub-rules
$(BIN_DIR)/%: $(BUILD_DIR)/%.o $(SRCS_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -I$(SRC_DIR) -o $@

$(BUILD_DIR)/%.o: $(MAIN_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -I$(SRC_DIR) -o $@

# clean rules
clean_all:
	rm -f $(EXECS) $(BUILD_DIR)/*.o

clean_execs:
	rm -f $(EXECS)

clean_tests:
	rm -f $(BIN_DIR)/test_*
