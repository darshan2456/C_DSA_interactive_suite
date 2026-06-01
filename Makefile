# Variables

# Compiler
CC = gcc

#Vlagrind flags
VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

# Compile flags
# \ for continue line, 
# -I includes the directory, includes all the src dierctories
CFLAGS = -Wall -Wextra -Werror -std=c11 -g \
	-Isrc/data_structures \
	-Isrc/expression_evaluation \
	-Isrc/sorting_algorithms_n2 \
	-Isrc/advanced_sorting_algorithms \
	-Isrc/searching_algorithms \
	-Isrc/graph_traversals \
	-Isrc/hashing

# source files are all .c files in src
SRCS = \
	src/data_structures/*.c \
	src/expression_evaluation/*.c \
	src/sorting_algorithms_n2/*.c \
	src/advanced_sorting_algorithms/*.c \
	src/searching_algorithms/*.c \
	src/graph_traversals/*.c \
	src/hashing/*.c

# checks operating system, works for all of these?
ifeq ($(OS),Windows_NT) # If on windows, rm command in windows, output file is .exe
	RM = cmd /c del
	EXE = .exe
else # if not on windows, i.e. linux, macos, unix, rm command on linux?
	RM = rm -f
	EXE =
endif

# EXEC / name of the output file, the one we want to run: dsa
TARGET = dsa 

# dafault target, $(target) accesses the target
all: $(TARGET) # dsa is dependency

# target: we compile dsa, dependencies: all .c source files in src
# recipe: gcc (compiler flags) (.c files) -o dsa.exe
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)$(EXE)

# find . : search recursively from current directory
# \( -name "*.c" -o -name "*.h" \) : find all files that end in .c and .h, -o means or
# -not -path "*/build/*" : exclude everything with /build/ in the path
# | is a pipe in bash, it sends the output of find as an input to xargs
# xargs : takes filenames from standard input (output of find), and appends to the command clang-format
# -i : edits the files in-place, rewrites files directly without printing formatted code to the terminal
fmt:
	find . \( -name "*.c" -o -name "*.h" \) -not -path "*/build/*" | xargs clang-format -i

#clean:
# 	$(RM) $(TARGET)$(EXE) test_circ_queue$(EXE) test_bst$(EXE) test_search$(EXE) test_hash_func$(EXE) test_sll$(EXE) test_dll$(EXE) test_array$(EXE) test_stack$(EXE)

# changed clean target to clean TEST_BINS variable
clean: 
	$(RM) $(TARGET)$(EXE) $(addsuffix $(EXE),$(TEST_BINS))

# valgrind command
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

# compiles the tests and then runs them
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


# TEST_BINS=test_circ_queue test_bst test_search test_hash_func test_sll test_dll test_array test_stack
# test: $(TEST_BINS)

# changed to 
TEST_BINS = \
	test_circ_queue \
	test_bst \
	test_search \
	test_hash_func \
	test_sll \
	test_dll \
	test_array \
	test_stack
test: $(TEST_BINS)

# phony targets are not associated with other files, recipe is always executed when called with make
# specifies which targets shouldn't be considered as files

# .PHONY: $(TARGET) $(TEST_BINS)

.PHONY: $(TARGET) $(TEST_BINS) clean


# can we replace TARGET with dsa?
# add clean to phony?
# should we have a .gitignore file so we can just do git add . everytime cuz right now it's adding 
# dsa when I run git add .