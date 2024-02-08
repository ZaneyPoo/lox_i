.PHONY: all run clean valg debug test
export

CC       := gcc
CFLAGS   := -g -Wall -Wextra -std=gnu17
CPPFLAGS := -DUNITY_OUTPUT_COLOR
BIN      := zli

SRC_DIR  := src
H_DIR    := incl
LIB_DIR  := lib

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
H_FILES=$(wildcard $(H_DIR)/*.h)
OBJ_FILES=$(SRC_FILES:.c=.o)
CORE_OBJS=$(filter-out $(SRC_DIR)/main.o, $(OBJ_FILES))

SAMPLE_FILE := tests/sample.lox
VG_FLAGS := --tool=memcheck --leak-check=full --track-origins=yes

all: $(BIN)


# Compilation ------------------------------------

$(BIN): $(OBJ_FILES) 
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(OBJ_FILES)

%.o: %.c $(H_FILES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<


# Running ----------------------------------------

run: $(BIN)
	./$< $(SAMPLE_FILE)

repl: $(BIN)
	./$<

valg: $(BIN)
	valgrind $(VG_FLAGS) ./$<

debug: $(BIN)
	gdb -tui ./$<

clean:
	@rm ./$(BIN)
	@rm $(foreach obj, $(OBJ_FILES), $(obj))


# Testing ----------------------------------------


TEST_PREFIX := test_
TEST_DIR := tests

TEST_SRC_FILES=$(wildcard $(TEST_DIR)/$(TEST_PREFIX)*.c)
TEST_OBJS=$(TEST_SRC_FILES:.c=.o)
TEST_BINS=$(TEST_OBJS:%.c=%)

UNITY_H := $(LIB_DIR)/unity.h
UNITY := $(LIB_DIR)/unity.o

test: $(TEST_BINS) $(CORE_OBJS) 
	for test in $(TEST_BINS); do echo "Running $$test" && ./$$test; done

$(TEST_BINS): $(TEST_SRC_FILES)
	echo test bin build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(CORE_OBJS) $(UNITY)

$(TEST_PREFIX)%.o: %.c
	echo o file build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(CORE_OBJS) $(UNITY)

$(UNITY): $(UNITY_H)
	$(CC) $(CPPFLAG) $(CFLAGS) -o $@ -c $<


