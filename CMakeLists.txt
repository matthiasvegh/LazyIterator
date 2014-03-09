
cmake_minimum_required(VERSION 2.6)
project(LazyIterator)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.55.0 REQUIRED COMPONENTS unit_test_framework)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(demo)
add_subdirectory(ut)

