cmake_minimum_required(VERSION 3.14)
project (testRunsAggregation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# add_library(ncurses)


# SET( CMAKE_CXX_COMPILER g++-12)

include_directories(include)
add_executable (aggregator src/test_t.cpp src/aggregator.cpp src/main.cpp src/utils.cpp src/ui.cpp src/ui-classes.cpp)

target_link_libraries(aggregator ncurses)