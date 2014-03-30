#include "adaptors.hpp"
#include "algorithm.hpp"
#include <map>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(RangeAdaptorsTestSuite)

BOOST_AUTO_TEST_CASE(Map_Keys_Range_should_be_empty_if_input_range_was_empty) {
	std::map<int, int> m;
	ph::adaptor::Map_Keys<decltype(m.begin()), decltype(m.end())> mk{m.begin(), m.end()};
	bool called = false;
	ph::for_each(mk.begin(), mk.end(), [&called](decltype(*mk.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, false);
}

BOOST_AUTO_TEST_CASE(Map_Keys_Range_should_not_be_empty_if_input_range_was_not_empty) {
	std::map<int, int> m;
	m.insert({1, 1});
	m.insert({2, 2});
	ph::adaptor::Map_Keys<decltype(m.begin()), decltype(m.end())> mk{m.begin(), m.end()};
	bool called = false;
	ph::for_each(mk.begin(), mk.end(), [&called](decltype(*mk.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, true);
}


BOOST_AUTO_TEST_SUITE_END()
