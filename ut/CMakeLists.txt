
add_executable(unitTest tests/lazyStrIteratorTest.cpp tests/untilTest.cpp testMain.cpp)
target_link_libraries(unitTest ${Boost_LIBRARIES})
