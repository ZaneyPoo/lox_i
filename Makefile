.PHONY: all run clean valg debug test

CC       := gcc
CFLAGS   := -g -Wall -Wextra -std=gnu17
CPPFLAGS := -DUNITY_OUTPUT_COLOR
BIN      := zbc

SRC_DIR  := src
H_DIR    := incl
LIB_DIR  := lib

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
H_FILES=$(wildcard $(H_DIR)/*.h)
OBJ_FILES=$(SRC_FILES:.c=.o)

VG_FLAGS := --tool=memcheck --leak-check=full --track-origins=yes

UNITY_H=$(LIB_DIR)/unity.h
UNITY=$(LIB_DIR)/unity.o

TEST_DIR := tests
TEST_SRC_FILES=$(wildcard $(TEST_DIR)/*.c)
TEST_OBJS=$(TEST_SRC_FILES:.c=.o)
TEST_OBJS+=$(filter-out $(SRC_DIR)/main.o, $(OBJ_FILES))
TEST_BINS=$(TEST_SRC_FILES:.c=.test)

SAMPLE_FILE := tests/sample.lox

all: $(BIN)

# Compilation ------------------------------------

$(BIN): $(OBJ_FILES) 
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

%.test: %.c $(TEST_OBJS) $(UNITY_H)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TEST_OBJS) $(UNITY)

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) -o $@ -c $<

# Running ----------------------------------------

run: $(BIN)
	./$< $(SAMPLE_FILE)

test: $(UNITY) $(TEST_BINS) 
	for test in $(TEST_BINS); do echo "Running $$test" && ./$$test; done

valg: $(BIN)
	valgrind $(VG_FLAGS) ./$<

debug: $(BIN)
	gdb -tui ./$<

clean:
	rm ./$(BIN)
	rm $(foreach obj, $(OBJ_FILES), $(obj))
	rm $(foreach test_obj, $(TEST_OBJS), $(test_obj))

