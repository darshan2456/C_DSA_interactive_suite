
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
    src/advanced_graph_algorithms
    src/hashing
    src/utils
    src/trees
    src/error_correction_algorithms
    src/job_scheduling
    src/dynamic_programming
    src/string_algorithms
    src/backtracking
    src/process_synchronization
    benchmark
)

# Visualization module directories
set(VIS_SRC_DIRS
    visualization/src
)

# Header search paths
foreach(DIR ${SRC_DIRS})
    include_directories(${DIR})
endforeach()

# Add visualization include directory
include_directories(visualization/include)

# Collect source files
set(SOURCES "")
foreach(DIR ${SRC_DIRS})
    file(GLOB DIR_SRCS "${DIR}/*.c")
    list(APPEND SOURCES ${DIR_SRCS})
endforeach()

# Collect visualization source files
foreach(DIR ${VIS_SRC_DIRS})
    file(GLOB VIS_SRCS "${DIR}/*.c")
    list(APPEND SOURCES ${VIS_SRCS})
endforeach()

# Exclude src/main.c from the library source files because it contains main()
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

if(WIN32)
    list(APPEND EXTRA_LIBS "psapi")
endif()

# Build library containing all common object files
add_library(dsa_lib STATIC ${SOURCES})

# Build the main executable
add_executable(dsa src/main.c)
target_link_libraries(dsa dsa_lib ${EXTRA_LIBS})

# ============================================================
# VISUALIZATION DEMO EXECUTABLES
# ============================================================

# Graph Visualization Demo
add_executable(vis_graph_demo visualization/examples/graph_demo.c)
target_link_libraries(vis_graph_demo dsa_lib ${EXTRA_LIBS})
target_include_directories(vis_graph_demo PRIVATE visualization/include)

# Tree Visualization Demo
add_executable(vis_tree_demo visualization/examples/tree_demo.c)
target_link_libraries(vis_tree_demo dsa_lib ${EXTRA_LIBS})
target_include_directories(vis_tree_demo PRIVATE visualization/include)

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

# ============================================================
# VISUALIZATION TESTS
# ============================================================

# Graph Visualizer Test
add_executable(test_graph_visualizer tests/visualization/test_graph_visualizer.c)
target_link_libraries(test_graph_visualizer dsa_lib ${EXTRA_LIBS})
target_include_directories(test_graph_visualizer PRIVATE visualization/include)
add_test(NAME test_graph_visualizer COMMAND test_graph_visualizer)

# Tree Visualizer Test
add_executable(test_tree_visualizer tests/visualization/test_tree_visualizer.c)
target_link_libraries(test_tree_visualizer dsa_lib ${EXTRA_LIBS})
target_include_directories(test_tree_visualizer PRIVATE visualization/include)
add_test(NAME test_tree_visualizer COMMAND test_tree_visualizer)

# ============================================================
# CUSTOM TARGETS
# ============================================================

# Custom target to run the main application
add_custom_target(run
    COMMAND $<TARGET_FILE:dsa>
    DEPENDS dsa
    COMMENT "Running dsa..."
)

# Custom target to run graph visualization demo
add_custom_target(run-vis-graph
    COMMAND $<TARGET_FILE:vis_graph_demo>
    DEPENDS vis_graph_demo
    COMMENT "Running Graph Visualization Demo..."
)

# Custom target to run tree visualization demo
add_custom_target(run-vis-tree
    COMMAND $<TARGET_FILE:vis_tree_demo>
    DEPENDS vis_tree_demo
    COMMENT "Running Tree Visualization Demo..."
)

# Custom target to run all visualization demos
add_custom_target(run-vis-demos
    COMMAND $<TARGET_FILE:vis_graph_demo>
    COMMAND $<TARGET_FILE:vis_tree_demo>
    DEPENDS vis_graph_demo vis_tree_demo
    COMMENT "Running all Visualization Demos..."
)

# Custom target to format source code using clang-format
add_custom_target(fmt
    COMMAND find . \\( -name "*.c" -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" -not -path "*/visualization/*" -o -name "*.h" -not -path "*/build/*" -not -path "*/object_files/*" -not -path "*/test_binaries/*" -not -path "*/visualization/*" \\) | xargs clang-format -i
    COMMAND find visualization \\( -name "*.c" -o -name "*.h" \\) -not -path "*/build/*" | xargs clang-format -i
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

# Add visualization tests to valgrind
list(APPEND VALGRIND_COMMANDS
    COMMAND echo "Running valgrind on test_graph_visualizer..."
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $<TARGET_FILE:test_graph_visualizer>
    COMMAND echo "Running valgrind on test_tree_visualizer..."
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $<TARGET_FILE:test_tree_visualizer>
)

add_custom_target(valgrind
    ${VALGRIND_COMMANDS}
    COMMENT "Running Valgrind memory checks on all tests..."
)

# ============================================================
# PRINT HELPER TARGET
# ============================================================

add_custom_target(help
    COMMAND echo "=================================================="
    COMMAND echo "Available targets:"
    COMMAND echo "  make dsa              - Build the main application"
    COMMAND echo "  make vis_graph_demo   - Build graph visualization demo"
    COMMAND echo "  make vis_tree_demo    - Build tree visualization demo"
    COMMAND echo "  make run              - Run the main application"
    COMMAND echo "  make run-vis-graph    - Run graph visualization demo"
    COMMAND echo "  make run-vis-tree     - Run tree visualization demo"
    COMMAND echo "  make run-vis-demos    - Run all visualization demos"
    COMMAND echo "  make test             - Run all tests"
    COMMAND echo "  make fmt              - Format source code"
    COMMAND echo "  make valgrind         - Run memory checks"
    COMMAND echo "  make help             - Show this help message"
    COMMAND echo "=================================================="
    COMMENT "Help target"
)