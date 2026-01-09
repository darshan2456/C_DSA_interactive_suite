CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g \
	-Idata_structures \
	-Iexpression_evaluation \
	-Isorting_algorithms \
	-Isearching_algorithms

SRCS = \
	data_structures/*.c \
	expression_evaluation/*.c \
	sorting_algorithms/*.c \
	searching_algorithms/*.c

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
