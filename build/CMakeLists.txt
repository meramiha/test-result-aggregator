cmake_minimum_required(VERSION 3.14)
project (testRunsAggregation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../include)
add_executable (aggregator ../src/test_t.cpp ../src/main.cpp)