
#include <boost/test/unit_test.hpp>
#include "ph.hpp"
#include "range.hpp"
#include "algorithm.hpp"

BOOST_AUTO_TEST_SUITE(algorithmTest)

BOOST_AUTO_TEST_CASE(normalIterator_find_test) {
	std::vector<int> v = { 1, 2, 3 };

	BOOST_CHECK(ph::find(v.begin(), v.end(), 2) == std::find(v.begin(), v.end(), 2));
}

BOOST_AUTO_TEST_CASE(normalIterator_find_if_test) {
	std::vector<int> v = { 1, 2, 3 };

	auto isEven = [](int i) { return i % 2 == 0; };

	BOOST_CHECK(ph::find_if(v.begin(), v.end(), isEven) == std::find_if(v.begin(), v.end(), isEven));
}

BOOST_AUTO_TEST_CASE(normalIterator_find_if_not_test) {
	std::vector<int> v = { 2, 3, 4 };

	auto isEven = [](int i) { return i % 2 == 0; };

	BOOST_CHECK(ph::find_if_not(v.begin(), v.end(), isEven) == std::find_if_not(v.begin(), v.end(), isEven));
}

BOOST_AUTO_TEST_CASE(rangeIterator_find_test) {
	std::vector<int> v = {1, 2, 3};

	auto r = ph::make_iterator_range(v.begin(), v.end());

	BOOST_CHECK(ph::find(r, 2) == std::find(v.begin(), v.end(), 2));
}


BOOST_AUTO_TEST_CASE(distance_test_for_iterator_pair) {
	std::vector<int> v = {1, 2, 3, 4, 5};

	auto endIterator = ph::untilValue(4);

	BOOST_CHECK(ph::distance(v.begin(), endIterator) ==
		std::distance(v.begin(), std::find(v.begin(), v.end(), 4)));
}

BOOST_AUTO_TEST_CASE(distance_test_for_iterator_range) {
	std::vector<int> v = {1, 2, 3, 4, 5};

	auto endIterator = ph::untilValue(4);
	auto range = ph::make_iterator_range(v.begin(), endIterator);

	BOOST_CHECK(ph::distance(range) ==
			std::distance(v.begin(), std::find(v.begin(), v.end(), 4)));
}

BOOST_AUTO_TEST_CASE(distance_test_for_empty_range) {
	std::vector<int> v = {};

	auto endIterator = ph::untilValue(4) || v.end();

	BOOST_CHECK(ph::distance(v.begin(), endIterator) ==
			std::distance(v.begin(), v.end()));
}

BOOST_AUTO_TEST_SUITE_END()

