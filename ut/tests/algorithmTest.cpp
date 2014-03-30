
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

BOOST_AUTO_TEST_SUITE_END()

