CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g \
	-Idata_structures \
	-Iexpression_evaluation \
	-Isorting_algorithms_n2 \
	-Isearching_algorithms \
	-Igraph_traversals_bfs_dfs

SRCS = \
	data_structures/*.c \
	expression_evaluation/*.c \
	sorting_algorithms_n2/*.c \
	searching_algorithms/*.c \
	graph_traversals_bfs_dfs/*.c

TARGET = demo

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
	$(RM) $(TARGET)$(EXE)

.PHONY: all clean
