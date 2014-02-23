
#include <boost/test/unit_test.hpp>
#include "../include/ph.hpp"

#include <algorithm>
#include <cstring>

BOOST_AUTO_TEST_SUITE(mainTests)


BOOST_AUTO_TEST_CASE(Find_With_LazyStrIterator_should_behave_as_if_using_strlen) {

	const char* str = "1234567890";

	auto posByStl = std::find(str, str+strlen(str), '6');

	auto posByPh = ph::find(str, ph::LazyStrIterator{}, '6');

	BOOST_REQUIRE(posByPh != str+strlen(str));

	BOOST_CHECK_EQUAL(*posByPh, '6');

}

BOOST_AUTO_TEST_SUITE_END()

