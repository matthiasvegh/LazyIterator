
cmake_minimum_required(VERSION 2.6)
project(LAZYITERATOR)

find_package(Boost 1.55.0 REQUIRED COMPONENTS unit_test_framework)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(demo)

