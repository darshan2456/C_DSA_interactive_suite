CC = gcc

VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

CFLAGS = -Wall -Wextra -Werror -std=c11 -g \
	-Isrc/data_structures \
	-Isrc/expression_evaluation \
	-Isrc/sorting_algorithms_n2 \
	-Isrc/advanced_sorting_algorithms \
	-Isrc/searching_algorithms \
	-Isrc/graph_traversals \
	-Isrc/hashing

SRCS = \
	src/data_structures/*.c \
	src/expression_evaluation/*.c \
	src/sorting_algorithms_n2/*.c \
	src/advanced_sorting_algorithms/*.c \
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
	$(RM) $(TARGET)$(EXE) test_circ_queue$(EXE) test_bst$(EXE) test_search$(EXE) test_hash_func$(EXE) test_sll$(EXE) test_dll$(EXE) test_array$(EXE)

valgrind:
	for t in $(TEST_BINS); do \
		echo "Running valgrind on $$t..."; \
		valgrind $(VGFLAGS) ./$$t || exit 1; \
	done

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

HASH_FUNCTION_TEST_SRC = \
	src/hashing/linear_probing.c \
	src/data_structures/safe_input_int.c \
	src/data_structures/array.c \
	tests/test_hash_function.c

SLL_TEST_SRC = \
    src/data_structures/sll.c \
	src/data_structures/safe_input_int.c \
    tests/test_sll.c

DLL_TEST_SRC = \
	src/data_structures/dll.c \
	src/data_structures/safe_input_int.c \
	tests/test_dll.c

ARRAY_TEST_SRC = \
	src/data_structures/array.c \
	src/data_structures/safe_input_int.c \
	tests/test_array.c

test_circ_queue:
	$(CC) $(CFLAGS) $(CIRC_QUEUE_TEST_SRC) -o test_circ_queue$(EXE)
	./test_circ_queue$(EXE)

test_bst:
	$(CC) $(CFLAGS) $(BST_TEST_SRC) -o test_bst$(EXE)
	./test_bst$(EXE)

test_search:
	$(CC) $(CFLAGS) $(SEARCH_TEST_SRC) -o test_search$(EXE)
	./test_search$(EXE)

test_hash_func:
	$(CC) $(CFLAGS) $(HASH_FUNCTION_TEST_SRC) -o test_hash_func$(EXE)
	./test_hash_func$(EXE)

test_sll:
	$(CC) $(CFLAGS) $(SLL_TEST_SRC) -o test_sll$(EXE)
	./test_sll$(EXE)

test_dll:
	$(CC) $(CFLAGS) $(DLL_TEST_SRC) -o test_dll$(EXE)
	./test_dll$(EXE)

test_array:
	$(CC) $(CFLAGS) $(ARRAY_TEST_SRC) -o test_array$(EXE)
	./test_array$(EXE)


TEST_BINS=test_circ_queue test_bst test_search test_hash_func test_sll test_dll test_array
test: $(TEST_BINS)

.PHONY: $(TARGET) test_circ_queue test_bst test_search test_hash_func test_sll test_dll test_array