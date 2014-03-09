
file(GLOB test_SRC tests/*cpp)

add_executable(unitTest ${test_SRC} testMain.cpp)
target_link_libraries(unitTest ${Boost_LIBRARIES})
