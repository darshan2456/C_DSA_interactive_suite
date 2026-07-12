cmake_minimum_required(VERSION 3.10)
project(C_DSA_interactive_suite C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

option(SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(SANITIZE)
    message(STATUS "Sanitizers enabled: AddressSanitizer + UndefinedBehaviorSanitizer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif()

# Source directories
set(SRC_DIRS
    help
    src/data_structures
    src/expression_evaluation
    src/sorting_algorithms_n2
    src/advanced_sorting_algorithms
    src/searching_algorithms
    src/graph_traversals
    src/advanced_graph_algorithms
    src/advanced_heaps
    src/hashing
    src/utils
    src/trees
    src/error_correction_algorithms
    src/job_scheduling
    src/dynamic_programming
    src/string_algorithms
    src/backtracking
    src/process_synchronization
    features/memory_profiler
    features/debugger
    features/benchmark
    src/cache_simulator
    src/compression
    demos/data_structures
    demos/trees
)

# Header search paths
foreach(DIR ${SRC_DIRS})
    include_directories(${DIR})
endforeach()

# Collect source files
set(SOURCES "")
foreach(DIR ${SRC_DIRS})
    file(GLOB DIR_SRCS "${DIR}/*.c")
    list(APPEND SOURCES ${DIR_SRCS})
endforeach()

# Exclude src/data_structures/main.c from the library source files because it contains main()
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/io_utility.c")

# Platform specific configurations
if(NOT WIN32)
    # Include TUI
    include_directories(tui)
    file(GLOB TUI_SRCS "tui/*.c")
    list(APPEND SOURCES ${TUI_SRCS})
    
    # Find and link ncurses
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    set(EXTRA_LIBS ${CURSES_LIBRARIES})
else()
    set(EXTRA_LIBS "")
endif()

if(WIN32)
    list(APPEND EXTRA_LIBS "psapi")
endif()

# Build library containing all common object files
add_library(dsa_lib STATIC ${SOURCES})

# Build the main executable
add_executable(dsa src/main.c)
target_link_libraries(dsa dsa_lib ${EXTRA_LIBS})

# Enable testing support
enable_testing()

# Build all tests
file(GLOB TEST_FILES "tests/**/test_*.c")
set(TEST_TARGETS "")
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} dsa_lib ${EXTRA_LIBS})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    list(APPEND TEST_TARGETS ${TEST_NAME})
endforeach()

# Custom target to run the main application
add_custom_target(run
    COMMAND $<TARGET_FILE:dsa>
    DEPENDS dsa
    COMMENT "Running dsa..."
)

# Custom target to format source code using clang-format
add_custom_target(fmt
    COMMAND find . -name "*.c" -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" -o -name "*.h" -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" | xargs clang-format -i
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Formatting source files..."
)

# Custom target to run valgrind memory checks on all tests
set(VALGRIND_COMMANDS "")
foreach(TEST_TARGET ${TEST_TARGETS})
    list(APPEND VALGRIND_COMMANDS
        COMMAND echo "Running valgrind on ${TEST_TARGET}..."
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $<TARGET_FILE:${TEST_TARGET}>
    )
endforeach()

add_custom_target(valgrind
    ${VALGRIND_COMMANDS}
    COMMENT "Running Valgrind memory checks on all tests..."
)

