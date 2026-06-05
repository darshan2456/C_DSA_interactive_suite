CC = gcc

VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

CFLAGS = -Wall -Wextra -Werror -std=c11 -g \
	-Isrc/data_structures \
	-Isrc/expression_evaluation \
	-Isrc/sorting_algorithms_n2 \
	-Isrc/advanced_sorting_algorithms \
	-Isrc/searching_algorithms \
	-Isrc/graph_traversals \
	-Isrc/hashing \
	-Isrc/utils

SRCS = \
	src/data_structures/*.c \
	src/expression_evaluation/*.c \
	src/sorting_algorithms_n2/*.c \
	src/advanced_sorting_algorithms/*.c \
	src/searching_algorithms/*.c \
	src/graph_traversals/*.c \
	src/hashing/*.c \
	src/utils/*.c

ifeq ($(OS),Windows_NT)
	RM = cmd /c del
	EXE = .exe
else
	RM = rm -f
	EXE =
endif

TARGET = dsa

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)$(EXE)

fmt:
	find . \( -name "*.c" -o -name "*.h" \) -not -path "*/build/*" | xargs clang-format -i

clean:
	$(RM) $(TARGET)$(EXE) $(foreach bin,$(TEST_BINS),$(bin)$(EXE))

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
	src/utils/safe_input_int.c \
	tests/test_circ_queue.c

BST_TEST_SRC = \
	src/data_structures/bst.c \
	src/utils/safe_input_int.c \
	tests/test_bst.c

SEARCH_TEST_SRC = \
	src/searching_algorithms/linear_search.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	src/searching_algorithms/binary_search.c \
	src/sorting_algorithms_n2/selection_sort.c \
	src/data_structures/array.c \
	tests/test_search.c

HASH_FUNCTION_TEST_SRC = \
	src/hashing/linear_probing.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	src/data_structures/array.c \
	src/searching_algorithms/linear_search.c \
	tests/test_hash_function.c

SLL_TEST_SRC = \
    src/data_structures/sll.c \
	src/utils/safe_input_int.c \
    tests/test_sll.c

DLL_TEST_SRC = \
	src/data_structures/dll.c \
	src/utils/safe_input_int.c \
	tests/test_dll.c

ARRAY_TEST_SRC = \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	tests/test_array.c

STACK_TEST_SRC = \
	src/expression_evaluation/stack.c \
	src/data_structures/sll.c \
	src/utils/safe_input_int.c \
	tests/test_stack.c

TBT_TEST_SRC = \
	src/utils/safe_input_int.c \
	src/data_structures/tbt.c \
	tests/test_tbt.c

PRIORITY_QUEUE_SRC = \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/data_structures/priority_queue.c \
	tests/test_priority_queue.c

SCLL_TEST_SRC = \
	src/data_structures/scll.c \
	src/utils/safe_input_int.c \
	tests/test_scll.c

SIMPLE_QUEUE_TEST_SRC = \
	src/data_structures/simple_queue.c \
	src/utils/safe_input_int.c \
	tests/test_simple_queue.c

DEQUE_TEST_SRC = \
	src/data_structures/deque.c \
	src/utils/safe_input_int.c \
	tests/test_deque.c

ASTAR_TEST_SRC = \
	src/graph_traversals/astar.c \
	src/graph_traversals/dijkstra.c \
	src/graph_traversals/graph_io.c \
	src/utils/safe_input_int.c \
	tests/test_astar.c
AVL_TEST_SRC = \
	src/data_structures/avl.c \
	src/utils/safe_input_int.c \
	tests/test_avl.c

TRIE_TEST_SRC = \
	src/data_structures/trie.c \
	src/utils/safe_input_int.c \
	tests/test_trie.c



GREEDY_BFS_TEST_SRC = \
	src/graph_traversals/greedy_best_first_search.c \
	src/graph_traversals/dijkstra.c \
	src/graph_traversals/graph_io.c \
	src/utils/safe_input_int.c \
	tests/test_greedy_best_first_search.c

HISTORY_LOGGER_TEST_SRC = \
	src/utils/history_logger.c \
	tests/test_history_logger.c

SORTING_N2_TEST_SRC = \
	src/sorting_algorithms_n2/bubble_sort.c \
	src/sorting_algorithms_n2/insertion_sort.c \
	src/sorting_algorithms_n2/selection_sort.c \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	tests/test_sorting_n2.c

ADVANCED_SORTING_TEST_SRC = \
	src/advanced_sorting_algorithms/quick_sort.c \
	src/advanced_sorting_algorithms/merge_sort.c \
	src/advanced_sorting_algorithms/heap_sort.c \
	src/advanced_sorting_algorithms/radix_sort.c \
	src/data_structures/priority_queue.c \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	
SHELL_SORT_TEST_SRC = \
	src/sorting_algorithms_n2/shell_sort.c \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	tests/test_shell_sort.c

test_tbt:
	$(CC) $(CFLAGS) $(TBT_TEST_SRC) -o test_tbt$(EXE)
	./test_tbt$(EXE)

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

test_stack:
	$(CC) $(CFLAGS) $(STACK_TEST_SRC) -o test_stack$(EXE)
	./test_stack$(EXE)

test_priority_queue:
	$(CC) $(CFLAGS) $(PRIORITY_QUEUE_SRC) -o test_priority_queue$(EXE)
	./test_priority_queue$(EXE)

test_scll:
	$(CC) $(CFLAGS) $(SCLL_TEST_SRC) -o test_scll$(EXE)
	./test_scll$(EXE)

test_simple_queue:
	$(CC) $(CFLAGS) $(SIMPLE_QUEUE_TEST_SRC) -o test_simple_queue$(EXE)
	./test_simple_queue$(EXE)

test_deque:
	$(CC) $(CFLAGS) $(DEQUE_TEST_SRC) -o test_deque$(EXE)
	./test_deque$(EXE)

test_astar:
	$(CC) $(CFLAGS) $(ASTAR_TEST_SRC) -o test_astar$(EXE)
	./test_astar$(EXE)


test_avl:
	$(CC) $(CFLAGS) $(AVL_TEST_SRC) -o test_avl$(EXE)
	./test_avl$(EXE)

test_trie:
	$(CC) $(CFLAGS) $(TRIE_TEST_SRC) -o test_trie$(EXE)
	./test_trie$(EXE)



test_greedy_bfs:
	$(CC) $(CFLAGS) $(GREEDY_BFS_TEST_SRC) -o test_greedy_bfs$(EXE)
	./test_greedy_bfs$(EXE)

test_history_logger:
	$(CC) $(CFLAGS) $(HISTORY_LOGGER_TEST_SRC) -o test_history_logger$(EXE)
	./test_history_logger$(EXE)

test_shell_sort:
	$(CC) $(CFLAGS) $(SHELL_SORT_TEST_SRC) -o test_shell_sort$(EXE)
	./test_shell_sort$(EXE)

# ---- sorting algorithm unit tests (issue #92) ----
SORTING_N2_TEST_SRC = \
	src/sorting_algorithms_n2/bubble_sort.c \
	src/sorting_algorithms_n2/insertion_sort.c \
	src/sorting_algorithms_n2/selection_sort.c \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	tests/test_sorting_n2.c

ADVANCED_SORTING_TEST_SRC = \
	src/advanced_sorting_algorithms/quick_sort.c \
	src/advanced_sorting_algorithms/merge_sort.c \
	src/advanced_sorting_algorithms/heap_sort.c \
	src/advanced_sorting_algorithms/radix_sort.c \
	src/data_structures/priority_queue.c \
	src/data_structures/array.c \
	src/utils/safe_input_int.c \
	src/utils/history_logger.c \
	tests/test_advanced_sorting.c

test_sorting_n2:
	$(CC) $(CFLAGS) $(SORTING_N2_TEST_SRC) -o test_sorting_n2$(EXE)
	./test_sorting_n2$(EXE)

test_advanced_sorting:
	$(CC) $(CFLAGS) $(ADVANCED_SORTING_TEST_SRC) -o test_advanced_sorting$(EXE)
	./test_advanced_sorting$(EXE)

TEST_BINS = test_circ_queue test_bst test_search test_hash_func \
            test_sll test_dll test_array test_stack test_tbt \
            test_priority_queue test_scll test_simple_queue \
            test_deque test_astar test_avl \
            test_greedy_bfs test_sorting_n2 test_advanced_sorting \
            test_history_logger test_shell_sort test_trie

test: $(TEST_BINS)

.PHONY: $(TARGET) $(TEST_BINS)
