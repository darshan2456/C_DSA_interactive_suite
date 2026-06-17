CC = gcc

OBJ_DIR = object_files
TEST_DIR = test_binaries

VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

CFLAGS = -Wall -Wextra -Werror -std=c11 -g \
	-Isrc/data_structures \
	-Isrc/expression_evaluation \
	-Isrc/sorting_algorithms_n2 \
	-Isrc/advanced_sorting_algorithms \
	-Isrc/searching_algorithms \
	-Isrc/graph_traversals \
	-Isrc/hashing \
	-Isrc/utils \
	-Isrc/trees \
	-Isrc/error_correction_algorithms \
	-Isrc/job_scheduling \
	-Isrc/dynamic_programming \
	-Isrc/string_algorithms \
	-Isrc/backtracking \
	-Isrc/job_scheduling
	# -Isrc/tui

# LDFLAGS = -lncurses

SRC_DIRS = \
	src/data_structures \
	src/expression_evaluation \
	src/sorting_algorithms_n2 \
	src/advanced_sorting_algorithms \
	src/searching_algorithms \
	src/graph_traversals \
	src/hashing \
	src/utils \
	src/trees \
	src/error_correction_algorithms \
	src/job_scheduling \
	src/dynamic_programming \
	src/string_algorithms \
	src/backtracking

# SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
# OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
	RM = cmd /c del
	RM_DIR = cmd /c rmdir /s /q
	EXE = .exe
	MKDIR_P = cmd /c if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
else
	RM = rm -f
	RM_DIR = rm -rf
	EXE =
	MKDIR_P = mkdir -p "$(1)"

	SRC_DIRS += src/tui 
	LDFLAGS += -lncurses
	CFLAGS += -Isrc/tui

endif

SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = dsa

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)$(EXE) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)$(EXE)


$(OBJ_DIR)/%.o: %.c
	@$(call MKDIR_P,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

fmt:
	find . \( -name "*.c" -o -name "*.h" \) -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" | xargs clang-format -i

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(TARGET)$(EXE)
	$(RM_DIR) $(OBJ_DIR)
	$(RM_DIR) $(TEST_DIR)
else
	$(RM) $(TARGET)$(EXE)
	$(RM_DIR) $(OBJ_DIR)
	$(RM_DIR) $(TEST_DIR)
endif

valgrind:
	for t in $(TEST_BINS); do \
		echo "Running valgrind on $$t..."; \
		valgrind $(VGFLAGS) $(TEST_DIR)/$$t$(EXE) || exit 1; \
	done


# =========================
# Test Section
# =========================

TEST_BINS = test_circ_queue test_bst test_search test_hash_func \
            test_sll test_dll test_array test_stack test_tbt \
            test_priority_queue test_scll test_dcll test_simple_queue \
            test_deque test_astar test_avl \
            test_greedy_bfs test_sorting_n2 test_advanced_sorting \
            test_history_logger test_shell_sort test_trie test_btree test_bplus_tree test_parity_bit \
            test_prim test_kruskal test_floyd_warshall test_mcm \
            test_string_algorithms

test: $(TEST_BINS)

test_mcm: $(TEST_DIR)/test_mcm$(EXE)
	$(TEST_DIR)/test_mcm$(EXE)

$(TEST_DIR)/test_mcm$(EXE): $(OBJ_DIR)/src/dynamic_programming/mcm.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_mcm.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_kruskal: $(TEST_DIR)/test_kruskal$(EXE)
	$(TEST_DIR)/test_kruskal$(EXE)

$(TEST_DIR)/test_kruskal$(EXE): $(filter-out $(OBJ_DIR)/src/data_structures/main.o, $(OBJS)) tests/test_kruskal.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	

test_floyd_warshall: $(TEST_DIR)/test_floyd_warshall$(EXE)
	$(TEST_DIR)/test_floyd_warshall$(EXE)

$(TEST_DIR)/test_floyd_warshall$(EXE): $(OBJ_DIR)/src/graph_traversals/floyd_warshall.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_floyd_warshall.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_prim: $(TEST_DIR)/test_prim$(EXE)
	$(TEST_DIR)/test_prim$(EXE)

$(TEST_DIR)/test_prim$(EXE): $(filter-out $(OBJ_DIR)/src/data_structures/main.o, $(OBJS)) tests/test_prim.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	

test_tbt: $(TEST_DIR)/test_tbt$(EXE)
	$(TEST_DIR)/test_tbt$(EXE)

$(TEST_DIR)/test_tbt$(EXE): $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/trees/tbt.o tests/test_tbt.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_circ_queue: $(TEST_DIR)/test_circ_queue$(EXE)
	$(TEST_DIR)/test_circ_queue$(EXE)

$(TEST_DIR)/test_circ_queue$(EXE): $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/test_circ_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_bst: $(TEST_DIR)/test_bst$(EXE)
	$(TEST_DIR)/test_bst$(EXE)

$(TEST_DIR)/test_bst$(EXE): $(OBJ_DIR)/src/trees/bst.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_bst.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_search: $(TEST_DIR)/test_search$(EXE)
	$(TEST_DIR)/test_search$(EXE)

$(TEST_DIR)/test_search$(EXE): $(OBJ_DIR)/src/searching_algorithms/linear_search.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/searching_algorithms/binary_search.o $(OBJ_DIR)/src/searching_algorithms/interpolation_search.o $(OBJ_DIR)/src/searching_algorithms/jump_search.o $(OBJ_DIR)/src/sorting_algorithms_n2/selection_sort.o $(OBJ_DIR)/src/data_structures/array.o tests/test_search.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_hash_func: $(TEST_DIR)/test_hash_func$(EXE)
	$(TEST_DIR)/test_hash_func$(EXE)

$(TEST_DIR)/test_hash_func$(EXE): $(OBJ_DIR)/src/hashing/linear_probing.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/searching_algorithms/linear_search.o tests/test_hash_function.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_sll: $(TEST_DIR)/test_sll$(EXE)
	$(TEST_DIR)/test_sll$(EXE)

$(TEST_DIR)/test_sll$(EXE): $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_sll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dll: $(TEST_DIR)/test_dll$(EXE)
	$(TEST_DIR)/test_dll$(EXE)

$(TEST_DIR)/test_dll$(EXE): $(OBJ_DIR)/src/data_structures/dll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_dll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_array: $(TEST_DIR)/test_array$(EXE)
	$(TEST_DIR)/test_array$(EXE)

$(TEST_DIR)/test_array$(EXE): $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_array.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_stack: $(TEST_DIR)/test_stack$(EXE)
	$(TEST_DIR)/test_stack$(EXE)

$(TEST_DIR)/test_stack$(EXE): $(OBJ_DIR)/src/expression_evaluation/stack.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_stack.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_priority_queue: $(TEST_DIR)/test_priority_queue$(EXE)
	$(TEST_DIR)/test_priority_queue$(EXE)

$(TEST_DIR)/test_priority_queue$(EXE): $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/data_structures/priority_queue.o tests/test_priority_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_scll: $(TEST_DIR)/test_scll$(EXE)
	$(TEST_DIR)/test_scll$(EXE)

$(TEST_DIR)/test_scll$(EXE): $(OBJ_DIR)/src/data_structures/scll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_scll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dcll: $(TEST_DIR)/test_dcll$(EXE)
	$(TEST_DIR)/test_dcll$(EXE)

$(TEST_DIR)/test_dcll$(EXE): $(OBJ_DIR)/src/data_structures/dcll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_dcll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_simple_queue: $(TEST_DIR)/test_simple_queue$(EXE)
	$(TEST_DIR)/test_simple_queue$(EXE)

$(TEST_DIR)/test_simple_queue$(EXE): $(OBJ_DIR)/src/data_structures/simple_queue.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/test_simple_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_deque: $(TEST_DIR)/test_deque$(EXE)
	$(TEST_DIR)/test_deque$(EXE)

$(TEST_DIR)/test_deque$(EXE): $(OBJ_DIR)/src/data_structures/deque.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/test_deque.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_astar: $(TEST_DIR)/test_astar$(EXE)
	$(TEST_DIR)/test_astar$(EXE)

$(TEST_DIR)/test_astar$(EXE): $(OBJ_DIR)/src/graph_traversals/astar.o $(OBJ_DIR)/src/graph_traversals/dijkstra.o $(OBJ_DIR)/src/graph_traversals/bfs.o $(OBJ_DIR)/src/utils/returnMallocVal.o $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/graph_traversals/graph_io.o tests/test_astar.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_avl: $(TEST_DIR)/test_avl$(EXE)
	$(TEST_DIR)/test_avl$(EXE)

$(TEST_DIR)/test_avl$(EXE): $(OBJ_DIR)/src/trees/avl.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_avl.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_trie: $(TEST_DIR)/test_trie$(EXE)
	$(TEST_DIR)/test_trie$(EXE)

$(TEST_DIR)/test_trie$(EXE): $(OBJ_DIR)/src/trees/trie.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_trie.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_btree: $(TEST_DIR)/test_btree$(EXE)
	$(TEST_DIR)/test_btree$(EXE)

$(TEST_DIR)/test_btree$(EXE): $(OBJ_DIR)/src/trees/btree.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_btree.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_greedy_bfs: $(TEST_DIR)/test_greedy_bfs$(EXE)
	$(TEST_DIR)/test_greedy_bfs$(EXE)

$(TEST_DIR)/test_greedy_bfs$(EXE): $(OBJ_DIR)/src/graph_traversals/greedy_best_first_search.o $(OBJ_DIR)/src/graph_traversals/dijkstra.o $(OBJ_DIR)/src/graph_traversals/bfs.o $(OBJ_DIR)/src/utils/returnMallocVal.o $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/graph_traversals/graph_io.o tests/test_greedy_best_first_search.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_history_logger: $(TEST_DIR)/test_history_logger$(EXE)
	$(TEST_DIR)/test_history_logger$(EXE)

$(TEST_DIR)/test_history_logger$(EXE): $(OBJ_DIR)/src/utils/history_logger.o tests/test_history_logger.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_shell_sort: $(TEST_DIR)/test_shell_sort$(EXE)
	$(TEST_DIR)/test_shell_sort$(EXE)

$(TEST_DIR)/test_shell_sort$(EXE): $(OBJ_DIR)/src/sorting_algorithms_n2/shell_sort.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_shell_sort.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_sorting_n2: $(TEST_DIR)/test_sorting_n2$(EXE)
	$(TEST_DIR)/test_sorting_n2$(EXE)

$(TEST_DIR)/test_sorting_n2$(EXE): $(OBJ_DIR)/src/sorting_algorithms_n2/bubble_sort.o $(OBJ_DIR)/src/sorting_algorithms_n2/insertion_sort.o $(OBJ_DIR)/src/sorting_algorithms_n2/selection_sort.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_sorting_n2.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_advanced_sorting: $(TEST_DIR)/test_advanced_sorting$(EXE)
	$(TEST_DIR)/test_advanced_sorting$(EXE)

$(TEST_DIR)/test_advanced_sorting$(EXE): $(OBJ_DIR)/src/advanced_sorting_algorithms/quick_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/merge_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/heap_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/radix_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/bucket_sort.o $(OBJ_DIR)/src/data_structures/priority_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_advanced_sorting.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_string_algorithms: $(TEST_DIR)/test_string_algorithms$(EXE)
	$(TEST_DIR)/test_string_algorithms$(EXE)

$(TEST_DIR)/test_string_algorithms$(EXE): $(OBJ_DIR)/src/string_algorithms/naive_string_matching.o $(OBJ_DIR)/src/string_algorithms/kmp.o $(OBJ_DIR)/src/string_algorithms/rabin_karp.o $(OBJ_DIR)/src/utils/safe_input_string.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_string_algorithms.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@

test_bplus_tree: $(TEST_DIR)/test_bplus_tree$(EXE)
	$(TEST_DIR)/test_bplus_tree$(EXE)

$(TEST_DIR)/test_bplus_tree$(EXE): $(OBJ_DIR)/src/trees/bplus_tree.o $(OBJ_DIR)/src/trees/mwst_utils.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/test_bplus_tree.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_parity_bit: $(TEST_DIR)/test_parity_bit$(EXE)
	$(TEST_DIR)/test_parity_bit$(EXE)

$(TEST_DIR)/test_parity_bit$(EXE): $(OBJ_DIR)/src/error_correction_algorithms/parity_bit.o $(OBJ_DIR)/src/error_correction_algorithms/checksum.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/test_parity_bit.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: run fmt clean valgrind
