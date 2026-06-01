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

# maybe don't really need wildcard
SRCS = \
	$(wildcard src/data_structures/*.c) \
	$(wildcard src/expression_evaluation/*.c) \
	$(wildcard src/sorting_algorithms_n2/*.c) \
	$(wildcard src/advanced_sorting_algorithms/*.c) \
	$(wildcard src/searching_algorithms/*.c) \
	$(wildcard src/graph_traversals/*.c) \
	$(wildcard src/hashing/*.c)

BLD_DIR = build
OBJS = $(SRCS:%.c=%.o) # prolly worse, compiles more stuff
#OBJS = $(patsubst $(SRCS/%.c),$(BLD_DIR)/%.o,$(SRCS))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

DEPS = $(OBJS:%.o=%.d)

ifeq ($(OS),Windows_NT)
	RM = cmd /c del
	EXE = .exe
else
	RM = rm -f
	EXE =
endif

TARGET = dsa

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

bld_dir:
	@mkdir -p $(BLD_DIR)

$(BLD_DIR)/%.o: $(SRCS)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

fmt:
	find . \( -name "*.c" -o -name "*.h" \) -not -path "*/build/*" | xargs clang-format -i

clean: 
	$(RM) $(TARGET) $(BLD_DIR)/*.o $(BLD_DIR)/*.d $(addsuffix $(EXE), $(TEST_BINS))
	rmdir $(BLD_DIR) 2>/dev/null || true

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
	src/data_structures/bst.c \
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

STACK_TEST_SRC = \
	src/expression_evaluation/stack.c \
	src/data_structures/sll.c \
	src/data_structures/safe_input_int.c \
	tests/test_stack.c

TBT_TEST_SRC = \
	src/data_structures/safe_input_int.c \
	src/data_structures/tbt.c \
	tests/test_tbt.c

PRIORITY_QUEUE_SRC = \
	src/data_structures/array.c \
	src/data_structures/safe_input_int.c \
	src/data_structures/priority_queue.c \
	tests/test_priority_queue.c 

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

test_tbt:
	$(CC) $(CFLAGS) $(TBT_TEST_SRC) -o test_tbt$(EXE)
	./test_tbt$(EXE)

test_priority_queue:
	$(CC) $(CFLAGS) $(PRIORITY_QUEUE_SRC) -o test_priority_queue$(EXE)
	./test_priority_queue$(EXE)


TEST_BINS = test_circ_queue test_bst test_search test_hash_func \
	test_sll test_dll test_array test_stack test_tbt test_priority_queue

test: $(TEST_BINS)

.PHONY: all test bld_dir clean valgrind