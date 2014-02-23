
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

BOOST_AUTO_TEST_CASE(LazyStrIterator_should_compare_equal_to_an_iterator_pointing_to_NULL) {
	const char* c = "\0";

	BOOST_CHECK(ph::LazyStrIterator{} == c);

}

BOOST_AUTO_TEST_CASE(LazyStrIterator_should_not_compare_equal_to_an_iterator_not_pointing_to_NULL) {
	const char* c = "a";

	BOOST_CHECK(ph::LazyStrIterator{} != c);

}

BOOST_AUTO_TEST_CASE(Until_with_value_predicate_should_compare_equal_to_iterator_pointing_to_that_value) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	auto iterator = ph::Until([](const int& i) { return i == 2; });

	BOOST_CHECK(iterator == ++(v.begin()));
}

BOOST_AUTO_TEST_CASE(Until_with_value_predicate_should_not_compare_equal_to_iterator_not_pointing_to_that_value) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	auto iterator = ph::Until([](const int& i) { return i == 5; });

	BOOST_CHECK(iterator != ++(v.begin()));
}

BOOST_AUTO_TEST_SUITE_END()

