cmake_minimum_required(VERSION 3.10)
project(C_DSA_interactive_suite C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Source directories
set(SRC_DIRS
    src/data_structures
    src/expression_evaluation
    src/sorting_algorithms_n2
    src/advanced_sorting_algorithms
    src/searching_algorithms
    src/graph_traversals
    src/hashing
    src/utils
    src/trees
    src/error_correction_algorithms
    src/job_scheduling
    src/dynamic_programming
    src/string_algorithms
    src/backtracking
    src/process_synchronization
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

# Platform specific configurations
if(NOT WIN32)
    # Include TUI
    include_directories(src/tui)
    file(GLOB TUI_SRCS "src/tui/*.c")
    list(APPEND SOURCES ${TUI_SRCS})
    
    # Find and link ncurses
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    set(EXTRA_LIBS ${CURSES_LIBRARIES})
else()
    set(EXTRA_LIBS "")
endif()

# Build library containing all common object files
add_library(dsa_lib STATIC ${SOURCES})

# Build the main executable
add_executable(dsa src/main.c)
target_link_libraries(dsa dsa_lib ${EXTRA_LIBS})

# Enable testing support
enable_testing()

# Build all tests
file(GLOB TEST_FILES "tests/test_*.c")
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} dsa_lib ${EXTRA_LIBS})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
