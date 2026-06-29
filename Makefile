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
	-Isrc/process_synchronization \
	-Ibenchmark
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
	src/backtracking \
	src/process_synchronization \
	benchmark

# SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
# OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
	RM = cmd /c del
	RM_DIR = cmd /c rmdir /s /q
	EXE = .exe
	MKDIR_P = cmd /c if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
	LDFLAGS += -lpsapi
	CFLAGS += -D__USE_MINGW_ANSI_STDIO=1 -Wno-format

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

$(TARGET): $(OBJS) $(OBJ_DIR)/src/main.o
	$(CC) $(CFLAGS) $^ -o $(TARGET)$(EXE) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)$(EXE)


$(OBJ_DIR)/%.o: %.c
	@$(call MKDIR_P,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

fmt:
	find . \( -name "*.c" -o -name "*.h" \) -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" | xargs clang-format -i

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) $(TARGET)$(EXE)
	-$(RM_DIR) $(OBJ_DIR)
	-$(RM_DIR) $(TEST_DIR)
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
            test_deque test_astar test_avl test_segment_tree \
            test_greedy_bfs test_sorting_n2 test_advanced_sorting \
            test_history_logger test_shell_sort test_trie test_btree test_bplus_tree test_parity_bit \
            test_prim test_kruskal test_floyd_warshall test_mcm test_fibonacci test_knapsack test_lcs \
            test_string_algorithms test_expression_evaluation \
            test_fcfs test_sjf test_srtf test_round_robin test_priority_scheduling test_preemptive_priority \
            test_dining_philosophers test_petersons test_producer_consumer \
            test_dijkstra test_bellman_ford test_bfs test_dfs test_topological_sort test_benchmark test_scc test_ford_fulkerson test_edmonds_karp

ifneq ($(wildcard tests/benchmark/test_benchmark_sorting.c),)
TEST_BINS += test_benchmark_sorting
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_searching.c),)
TEST_BINS += test_benchmark_searching
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_graphs.c),)
TEST_BINS += test_benchmark_graphs
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_mst.c),)
TEST_BINS += test_benchmark_mst
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_scheduling.c),)
TEST_BINS += test_benchmark_scheduling
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_strings.c),)
TEST_BINS += test_benchmark_strings
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_dp.c),)
TEST_BINS += test_benchmark_dp
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_hashing.c),)
TEST_BINS += test_benchmark_hashing
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_trees.c),)
TEST_BINS += test_benchmark_trees
endif

ifneq ($(wildcard tests/benchmark/test_benchmark_backtracking.c),)
TEST_BINS += test_benchmark_backtracking
endif

test: $(TEST_BINS)

test_mcm: $(TEST_DIR)/test_mcm$(EXE)
	$(TEST_DIR)/test_mcm$(EXE)

$(TEST_DIR)/test_mcm$(EXE): $(OBJ_DIR)/src/dynamic_programming/mcm.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/dynamic_programming/test_mcm.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_fibonacci: $(TEST_DIR)/test_fibonacci$(EXE)
	$(TEST_DIR)/test_fibonacci$(EXE)

$(TEST_DIR)/test_fibonacci$(EXE): $(OBJ_DIR)/src/utils/mock_printf.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/safe_input_string.o $(OBJ_DIR)/src/utils/history_logger.o tests/dynamic_programming/test_fibonacci.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_knapsack: $(TEST_DIR)/test_knapsack$(EXE)
	$(TEST_DIR)/test_knapsack$(EXE)

$(TEST_DIR)/test_knapsack$(EXE): $(OBJ_DIR)/src/utils/mock_printf.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/safe_input_string.o $(OBJ_DIR)/src/utils/history_logger.o tests/dynamic_programming/test_knapsack.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_lcs: $(TEST_DIR)/test_lcs$(EXE)
	$(TEST_DIR)/test_lcs$(EXE)

$(TEST_DIR)/test_lcs$(EXE): $(OBJ_DIR)/src/utils/mock_printf.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/safe_input_string.o $(OBJ_DIR)/src/utils/history_logger.o tests/dynamic_programming/test_lcs.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_kruskal: $(TEST_DIR)/test_kruskal$(EXE)
	$(TEST_DIR)/test_kruskal$(EXE)

$(TEST_DIR)/test_kruskal$(EXE): $(OBJS) tests/graph_traversals/test_kruskal.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	

test_floyd_warshall: $(TEST_DIR)/test_floyd_warshall$(EXE)
	$(TEST_DIR)/test_floyd_warshall$(EXE)

$(TEST_DIR)/test_floyd_warshall$(EXE): $(OBJ_DIR)/src/graph_traversals/floyd_warshall.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/graph_traversals/test_floyd_warshall.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_prim: $(TEST_DIR)/test_prim$(EXE)
	$(TEST_DIR)/test_prim$(EXE)

$(TEST_DIR)/test_prim$(EXE): $(OBJS) tests/graph_traversals/test_prim.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	

test_tbt: $(TEST_DIR)/test_tbt$(EXE)
	$(TEST_DIR)/test_tbt$(EXE)

$(TEST_DIR)/test_tbt$(EXE): $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/trees/tbt.o tests/trees/test_tbt.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_circ_queue: $(TEST_DIR)/test_circ_queue$(EXE)
	$(TEST_DIR)/test_circ_queue$(EXE)

$(TEST_DIR)/test_circ_queue$(EXE): $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/data_structures/test_circ_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_bst: $(TEST_DIR)/test_bst$(EXE)
	$(TEST_DIR)/test_bst$(EXE)

$(TEST_DIR)/test_bst$(EXE): $(OBJ_DIR)/src/trees/bst.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/trees/test_bst.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_search: $(TEST_DIR)/test_search$(EXE)
	$(TEST_DIR)/test_search$(EXE)

$(TEST_DIR)/test_search$(EXE): $(OBJ_DIR)/src/searching_algorithms/linear_search.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/searching_algorithms/binary_search.o $(OBJ_DIR)/src/searching_algorithms/interpolation_search.o $(OBJ_DIR)/src/searching_algorithms/jump_search.o $(OBJ_DIR)/src/sorting_algorithms_n2/selection_sort.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/sorting_visualizer.o $(OBJ_DIR)/src/utils/cross_platform_timer.o $(OBJ_DIR)/src/utils/config.o tests/searching_algorithms/test_search.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_hash_func: $(TEST_DIR)/test_hash_func$(EXE)
	$(TEST_DIR)/test_hash_func$(EXE)

$(TEST_DIR)/test_hash_func$(EXE): $(OBJ_DIR)/src/hashing/linear_probing.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/searching_algorithms/linear_search.o tests/hashing/test_hash_function.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_sll: $(TEST_DIR)/test_sll$(EXE)
	$(TEST_DIR)/test_sll$(EXE)

$(TEST_DIR)/test_sll$(EXE): $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_sll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dll: $(TEST_DIR)/test_dll$(EXE)
	$(TEST_DIR)/test_dll$(EXE)

$(TEST_DIR)/test_dll$(EXE): $(OBJ_DIR)/src/data_structures/dll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_dll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_array: $(TEST_DIR)/test_array$(EXE)
	$(TEST_DIR)/test_array$(EXE)

$(TEST_DIR)/test_array$(EXE): $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_array.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_stack: $(TEST_DIR)/test_stack$(EXE)
	$(TEST_DIR)/test_stack$(EXE)

$(TEST_DIR)/test_stack$(EXE): $(OBJ_DIR)/src/data_structures/stack.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_stack.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


test_priority_queue: $(TEST_DIR)/test_priority_queue$(EXE)
	$(TEST_DIR)/test_priority_queue$(EXE)

$(TEST_DIR)/test_priority_queue$(EXE): $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/data_structures/priority_queue.o tests/data_structures/test_priority_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_scll: $(TEST_DIR)/test_scll$(EXE)
	$(TEST_DIR)/test_scll$(EXE)

$(TEST_DIR)/test_scll$(EXE): $(OBJ_DIR)/src/data_structures/scll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_scll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dcll: $(TEST_DIR)/test_dcll$(EXE)
	$(TEST_DIR)/test_dcll$(EXE)

$(TEST_DIR)/test_dcll$(EXE): $(OBJ_DIR)/src/data_structures/dcll.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_dcll.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_simple_queue: $(TEST_DIR)/test_simple_queue$(EXE)
	$(TEST_DIR)/test_simple_queue$(EXE)

$(TEST_DIR)/test_simple_queue$(EXE): $(OBJ_DIR)/src/data_structures/simple_queue.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/data_structures/test_simple_queue.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_deque: $(TEST_DIR)/test_deque$(EXE)
	$(TEST_DIR)/test_deque$(EXE)

$(TEST_DIR)/test_deque$(EXE): $(OBJ_DIR)/src/data_structures/deque.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/returnMallocVal.o tests/data_structures/test_deque.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_segment_tree: $(TEST_DIR)/test_segment_tree$(EXE)
	$(TEST_DIR)/test_segment_tree$(EXE)

$(TEST_DIR)/test_segment_tree$(EXE): $(OBJ_DIR)/src/data_structures/segment_tree.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/data_structures/test_segment_tree.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_astar: $(TEST_DIR)/test_astar$(EXE)
	$(TEST_DIR)/test_astar$(EXE)

$(TEST_DIR)/test_astar$(EXE): $(OBJ_DIR)/src/graph_traversals/astar.o $(OBJ_DIR)/src/graph_traversals/dijkstra.o $(OBJ_DIR)/src/graph_traversals/bfs.o $(OBJ_DIR)/src/utils/returnMallocVal.o $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/graph_traversals/graph_io.o tests/graph_traversals/test_astar.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_avl: $(TEST_DIR)/test_avl$(EXE)
	$(TEST_DIR)/test_avl$(EXE)

$(TEST_DIR)/test_avl$(EXE): $(OBJ_DIR)/src/trees/avl.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/trees/test_avl.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_trie: $(TEST_DIR)/test_trie$(EXE)
	$(TEST_DIR)/test_trie$(EXE)

$(TEST_DIR)/test_trie$(EXE): $(OBJ_DIR)/src/trees/trie.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/trees/test_trie.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_btree: $(TEST_DIR)/test_btree$(EXE)
	$(TEST_DIR)/test_btree$(EXE)

$(TEST_DIR)/test_btree$(EXE): $(OBJ_DIR)/src/trees/btree.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/trees/test_btree.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_greedy_bfs: $(TEST_DIR)/test_greedy_bfs$(EXE)
	$(TEST_DIR)/test_greedy_bfs$(EXE)

$(TEST_DIR)/test_greedy_bfs$(EXE): $(OBJ_DIR)/src/graph_traversals/greedy_best_first_search.o $(OBJ_DIR)/src/graph_traversals/dijkstra.o $(OBJ_DIR)/src/graph_traversals/bfs.o $(OBJ_DIR)/src/utils/returnMallocVal.o $(OBJ_DIR)/src/data_structures/circular_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/graph_traversals/graph_io.o tests/graph_traversals/test_greedy_best_first_search.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_history_logger: $(TEST_DIR)/test_history_logger$(EXE)
	$(TEST_DIR)/test_history_logger$(EXE)

$(TEST_DIR)/test_history_logger$(EXE): $(OBJ_DIR)/src/utils/history_logger.o tests/utils/test_history_logger.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark: $(TEST_DIR)/test_benchmark$(EXE)
	$(TEST_DIR)/test_benchmark$(EXE)

$(TEST_DIR)/test_benchmark$(EXE): $(OBJ_DIR)/benchmark/benchmark.o tests/benchmark/test_benchmark.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_sorting: $(TEST_DIR)/test_benchmark_sorting$(EXE)
	$(TEST_DIR)/test_benchmark_sorting$(EXE)

$(TEST_DIR)/test_benchmark_sorting$(EXE): $(OBJS) tests/benchmark/test_benchmark_sorting.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_searching: $(TEST_DIR)/test_benchmark_searching$(EXE)
	$(TEST_DIR)/test_benchmark_searching$(EXE)

$(TEST_DIR)/test_benchmark_searching$(EXE): $(OBJS) tests/benchmark/test_benchmark_searching.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_graphs: $(TEST_DIR)/test_benchmark_graphs$(EXE)
	$(TEST_DIR)/test_benchmark_graphs$(EXE)

$(TEST_DIR)/test_benchmark_graphs$(EXE): $(OBJS) tests/benchmark/test_benchmark_graphs.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_mst: $(TEST_DIR)/test_benchmark_mst$(EXE)
	$(TEST_DIR)/test_benchmark_mst$(EXE)

$(TEST_DIR)/test_benchmark_mst$(EXE): $(OBJS) tests/benchmark/test_benchmark_mst.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_scheduling: $(TEST_DIR)/test_benchmark_scheduling$(EXE)
	$(TEST_DIR)/test_benchmark_scheduling$(EXE)

$(TEST_DIR)/test_benchmark_scheduling$(EXE): $(OBJS) tests/benchmark/test_benchmark_scheduling.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_strings: $(TEST_DIR)/test_benchmark_strings$(EXE)
	$(TEST_DIR)/test_benchmark_strings$(EXE)

$(TEST_DIR)/test_benchmark_strings$(EXE): $(OBJS) tests/benchmark/test_benchmark_strings.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_dp: $(TEST_DIR)/test_benchmark_dp$(EXE)
	$(TEST_DIR)/test_benchmark_dp$(EXE)

$(TEST_DIR)/test_benchmark_dp$(EXE): $(OBJS) tests/benchmark/test_benchmark_dp.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_hashing: $(TEST_DIR)/test_benchmark_hashing$(EXE)
	$(TEST_DIR)/test_benchmark_hashing$(EXE)

$(TEST_DIR)/test_benchmark_hashing$(EXE): $(OBJS) tests/benchmark/test_benchmark_hashing.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_trees: $(TEST_DIR)/test_benchmark_trees$(EXE)
	$(TEST_DIR)/test_benchmark_trees$(EXE)

$(TEST_DIR)/test_benchmark_trees$(EXE): $(OBJS) tests/benchmark/test_benchmark_trees.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_benchmark_backtracking: $(TEST_DIR)/test_benchmark_backtracking$(EXE)
	$(TEST_DIR)/test_benchmark_backtracking$(EXE)

$(TEST_DIR)/test_benchmark_backtracking$(EXE): $(OBJS) tests/benchmark/test_benchmark_backtracking.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_shell_sort: $(TEST_DIR)/test_shell_sort$(EXE)
	$(TEST_DIR)/test_shell_sort$(EXE)

$(TEST_DIR)/test_shell_sort$(EXE): $(OBJ_DIR)/src/sorting_algorithms_n2/shell_sort.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/utils/sorting_visualizer.o $(OBJ_DIR)/src/utils/cross_platform_timer.o $(OBJ_DIR)/src/utils/config.o tests/sorting_algorithms_n2/test_shell_sort.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_sorting_n2: $(TEST_DIR)/test_sorting_n2$(EXE)
	$(TEST_DIR)/test_sorting_n2$(EXE)

$(TEST_DIR)/test_sorting_n2$(EXE): $(OBJ_DIR)/src/sorting_algorithms_n2/bubble_sort.o $(OBJ_DIR)/src/sorting_algorithms_n2/insertion_sort.o $(OBJ_DIR)/src/sorting_algorithms_n2/selection_sort.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/utils/sorting_visualizer.o $(OBJ_DIR)/src/utils/cross_platform_timer.o $(OBJ_DIR)/src/utils/config.o tests/sorting_algorithms_n2/test_sorting_n2.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_advanced_sorting: $(TEST_DIR)/test_advanced_sorting$(EXE)
	$(TEST_DIR)/test_advanced_sorting$(EXE)

$(TEST_DIR)/test_advanced_sorting$(EXE): $(OBJ_DIR)/src/advanced_sorting_algorithms/quick_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/merge_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/heap_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/radix_sort.o $(OBJ_DIR)/src/advanced_sorting_algorithms/bucket_sort.o $(OBJ_DIR)/src/data_structures/priority_queue.o $(OBJ_DIR)/src/data_structures/sll.o $(OBJ_DIR)/src/data_structures/array.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o $(OBJ_DIR)/src/utils/sorting_visualizer.o $(OBJ_DIR)/src/utils/cross_platform_timer.o $(OBJ_DIR)/src/utils/config.o tests/advanced_sorting_algorithms/test_advanced_sorting.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_backtracking: $(TEST_DIR)/test_backtracking$(EXE)
	$(TEST_DIR)/test_backtracking$(EXE)

$(TEST_DIR)/test_backtracking$(EXE): \
	$(OBJ_DIR)/src/backtracking/n_queens.o \
	$(OBJ_DIR)/src/backtracking/sudoku.o \
	$(OBJ_DIR)/src/backtracking/graph_coloring.o \
	$(OBJ_DIR)/src/backtracking/rat_in_maze.o \
	$(OBJ_DIR)/src/backtracking/knights_tour.o \
	$(OBJ_DIR)/src/utils/config.o \
	$(OBJ_DIR)/src/utils/safe_input_int.o\
	$(OBJ_DIR)/src/utils/clear_screen.o \
	$(OBJ_DIR)/src/utils/cross_platform_timer.o \
	tests/backtracking/test_backtracking.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_string_algorithms: $(TEST_DIR)/test_string_algorithms$(EXE)
	$(TEST_DIR)/test_string_algorithms$(EXE)

$(TEST_DIR)/test_string_algorithms$(EXE): \
    $(OBJ_DIR)/src/string_algorithms/naive_string_matching.o \
    $(OBJ_DIR)/src/string_algorithms/kmp.o \
    $(OBJ_DIR)/src/string_algorithms/rabin_karp.o \
    $(OBJ_DIR)/src/utils/safe_input_string.o \
    $(OBJ_DIR)/src/utils/history_logger.o \
    $(OBJ_DIR)/src/utils/clear_screen.o \
	$(OBJ_DIR)/src/utils/safe_input_int.o \
    $(OBJ_DIR)/src/utils/cross_platform_timer.o \
	$(OBJ_DIR)/src/utils/config.o \
    tests/string_algorithms/test_string_algorithms.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_bplus_tree: $(TEST_DIR)/test_bplus_tree$(EXE)
	$(TEST_DIR)/test_bplus_tree$(EXE)

$(TEST_DIR)/test_bplus_tree$(EXE): $(OBJ_DIR)/src/trees/bplus_tree.o $(OBJ_DIR)/src/trees/mwst_utils.o $(OBJ_DIR)/src/utils/safe_input_int.o tests/trees/test_bplus_tree.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_parity_bit: $(TEST_DIR)/test_parity_bit$(EXE)
	$(TEST_DIR)/test_parity_bit$(EXE)

$(TEST_DIR)/test_parity_bit$(EXE): $(OBJ_DIR)/src/error_correction_algorithms/parity_bit.o $(OBJ_DIR)/src/error_correction_algorithms/checksum.o $(OBJ_DIR)/src/utils/safe_input_int.o $(OBJ_DIR)/src/utils/history_logger.o tests/error_correction_algorithms/test_parity_bit.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)                                                                                                                                                                                                                                                                                         

test_expression_evaluation: $(TEST_DIR)/test_expression_evaluation$(EXE)
	$(TEST_DIR)/test_expression_evaluation$(EXE)

$(TEST_DIR)/test_expression_evaluation$(EXE): \
	$(OBJ_DIR)/src/data_structures/stack.o \
	$(OBJ_DIR)/src/expression_evaluation/infix_to_postfix.o \
	$(OBJ_DIR)/src/expression_evaluation/safe_input_infix.o \
	$(OBJ_DIR)/src/data_structures/sll.o \
	$(OBJ_DIR)/src/utils/safe_input_int.o \
	$(OBJ_DIR)/src/utils/clear_screen.o \
	$(OBJ_DIR)/src/utils/cross_platform_timer.o \
	$(OBJ_DIR)/src/utils/config.o \
	tests/expression_evaluation/test_expression_evaluation.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


test_fcfs: $(TEST_DIR)/test_fcfs$(EXE)
	$(TEST_DIR)/test_fcfs$(EXE)
$(TEST_DIR)/test_fcfs$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/fcfs.o,$(OBJS)) tests/job_scheduling/test_fcfs.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_sjf: $(TEST_DIR)/test_sjf$(EXE)
	$(TEST_DIR)/test_sjf$(EXE)
$(TEST_DIR)/test_sjf$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/sjf.o,$(OBJS)) tests/job_scheduling/test_sjf.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_srtf: $(TEST_DIR)/test_srtf$(EXE)
	$(TEST_DIR)/test_srtf$(EXE)
$(TEST_DIR)/test_srtf$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/srtf.o,$(OBJS)) tests/job_scheduling/test_srtf.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_round_robin: $(TEST_DIR)/test_round_robin$(EXE)
	$(TEST_DIR)/test_round_robin$(EXE)
$(TEST_DIR)/test_round_robin$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/round_robin.o,$(OBJS)) tests/job_scheduling/test_round_robin.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_priority_scheduling: $(TEST_DIR)/test_priority_scheduling$(EXE)
	$(TEST_DIR)/test_priority_scheduling$(EXE)
$(TEST_DIR)/test_priority_scheduling$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/priority_scheduling.o,$(OBJS)) tests/job_scheduling/test_priority_scheduling.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_preemptive_priority: $(TEST_DIR)/test_preemptive_priority$(EXE)
	$(TEST_DIR)/test_preemptive_priority$(EXE)
$(TEST_DIR)/test_preemptive_priority$(EXE): $(filter-out $(OBJ_DIR)/src/job_scheduling/preemptive_priority.o,$(OBJS)) tests/job_scheduling/test_preemptive_priority.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dining_philosophers: $(TEST_DIR)/test_dining_philosophers$(EXE)
	$(TEST_DIR)/test_dining_philosophers$(EXE)
$(TEST_DIR)/test_dining_philosophers$(EXE): $(filter-out $(OBJ_DIR)/src/process_synchronization/dining_philosophers.o,$(OBJS)) tests/process_synchronization/test_dining_philosophers.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_petersons: $(TEST_DIR)/test_petersons$(EXE)
	$(TEST_DIR)/test_petersons$(EXE)
$(TEST_DIR)/test_petersons$(EXE): $(filter-out $(OBJ_DIR)/src/process_synchronization/petersons_algorithm.o,$(OBJS)) tests/process_synchronization/test_petersons.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_producer_consumer: $(TEST_DIR)/test_producer_consumer$(EXE)
	$(TEST_DIR)/test_producer_consumer$(EXE)
$(TEST_DIR)/test_producer_consumer$(EXE): $(filter-out $(OBJ_DIR)/src/process_synchronization/producer_consumer.o,$(OBJS)) tests/process_synchronization/test_producer_consumer.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dijkstra: $(TEST_DIR)/test_dijkstra$(EXE)
	$(TEST_DIR)/test_dijkstra$(EXE)
$(TEST_DIR)/test_dijkstra$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/dijkstra.o,$(OBJS)) tests/graph_traversals/test_dijkstra.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_bellman_ford: $(TEST_DIR)/test_bellman_ford$(EXE)
	$(TEST_DIR)/test_bellman_ford$(EXE)
$(TEST_DIR)/test_bellman_ford$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/bellman_ford.o,$(OBJS)) tests/graph_traversals/test_bellman_ford.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_bfs: $(TEST_DIR)/test_bfs$(EXE)
	$(TEST_DIR)/test_bfs$(EXE)
$(TEST_DIR)/test_bfs$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/bfs.o,$(OBJS)) tests/graph_traversals/test_bfs.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dfs: $(TEST_DIR)/test_dfs$(EXE)
	$(TEST_DIR)/test_dfs$(EXE)
$(TEST_DIR)/test_dfs$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/dfs.o,$(OBJS)) tests/graph_traversals/test_dfs.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_topological_sort: $(TEST_DIR)/test_topological_sort$(EXE)
	$(TEST_DIR)/test_topological_sort$(EXE)
$(TEST_DIR)/test_topological_sort$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/topological_sort.o,$(OBJS)) tests/graph_traversals/test_topological_sort.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_scc: $(TEST_DIR)/test_scc$(EXE)
	$(TEST_DIR)/test_scc$(EXE)
$(TEST_DIR)/test_scc$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/scc.o,$(OBJS)) tests/graph_traversals/test_scc.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_ford_fulkerson: $(TEST_DIR)/test_ford_fulkerson$(EXE)
	$(TEST_DIR)/test_ford_fulkerson$(EXE)
$(TEST_DIR)/test_ford_fulkerson$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/ford_fulkerson.o,$(OBJS)) tests/graph_traversals/test_ford_fulkerson.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_edmonds_karp: $(TEST_DIR)/test_edmonds_karp$(EXE)
	$(TEST_DIR)/test_edmonds_karp$(EXE)
$(TEST_DIR)/test_edmonds_karp$(EXE): $(filter-out $(OBJ_DIR)/src/graph_traversals/edmonds_karp.o,$(OBJS)) tests/graph_traversals/test_edmonds_karp.c
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: run fmt clean valgrind
