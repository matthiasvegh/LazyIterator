
find_package(Boost 1.55.0 REQUIRED)

include_directories(../include)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_executable(demo main.cpp)

