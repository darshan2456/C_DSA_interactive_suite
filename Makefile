CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g \
	-Isrc/data_structures \
	-Isrc/expression_evaluation \
	-Isrc/sorting_algorithms_n2 \
	-Isrc/searching_algorithms \
	-Isrc/graph_traversals \
	-Isrc/hashing

SRCS = \
	src/data_structures/*.c \
	src/expression_evaluation/*.c \
	src/sorting_algorithms_n2/*.c \
	src/searching_algorithms/*.c \
	src/graph_traversals/*.c \
	src/hashing/*.c

TARGET = dsa

ifeq ($(OS),Windows_NT)
	RM = cmd /c del
	EXE = .exe
else
	RM = rm -f
	EXE =
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)$(EXE)

clean:
	$(RM) $(TARGET)$(EXE) test_circ_queue$(EXE) test_bst$(EXE) test_search$(EXE) test_hash$(EXE)

# =========================
# Test Section
# =========================

CIRC_QUEUE_TEST_SRC = \
	src/data_structures/circular_queue.c \
	src/data_structures/safe_input_int.c \
	tests/test_circ_queue.c

BST_TEST_SRC = \
	src/data_structures/binary_search_tree.c \
	src/data_structures/safe_input_int.c \
	tests/test_bst.c

SEARCH_TEST_SRC = \
	src/searching_algorithms/linear_search.c \
	src/data_structures/safe_input_int.c \
	src/searching_algorithms/binary_search.c \
	src/sorting_algorithms_n2/selection_sort.c \
	src/data_structures/array.c \
	tests/test_search.c

HASH_TEST_SRC = \
	src/hashing/linear_probing.c \
	src/data_structures/safe_input_int.c \
	src/data_structures/array.c \
	tests/test_hash.c

test_circ_queue:
	$(CC) $(CFLAGS) $(CIRC_QUEUE_TEST_SRC) -o test_circ_queue$(EXE)
	./test_circ_queue$(EXE)

test_bst:
	$(CC) $(CFLAGS) $(BST_TEST_SRC) -o test_bst$(EXE)
	./test_bst$(EXE)

test_search:
	$(CC) $(CFLAGS) $(SEARCH_TEST_SRC) -o test_search$(EXE)
	./test_search$(EXE)

test_hash:
	$(CC) $(CFLAGS) $(HASH_TEST_SRC) -o test_hash$(EXE)
	./test_hash$(EXE)

test: test_circ_queue test_bst test_search test_hash

.PHONY: all clean test test_circ_queue test_bst test_search test_hash