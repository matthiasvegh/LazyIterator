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

BOOST_AUTO_TEST_CASE(Map_Keys_Pipe_should_work_on_empty_range) {
	std::map<int, int> m;

	auto map_range = ph::make_iterator_range(m.begin(), m.end());
	auto range = map_range | ph::adaptor::map_keys;

	bool called = false;
	ph::for_each(range.begin(), range.end(), [&called](decltype(*range.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, false);

}


BOOST_AUTO_TEST_CASE(Map_Values_Range_should_be_empty_if_input_range_was_empty) {
	std::map<int, int> m;
	ph::adaptor::Map_Values<decltype(m.begin()), decltype(m.end())> mk{m.begin(), m.end()};
	bool called = false;
	ph::for_each(mk.begin(), mk.end(), [&called](decltype(*mk.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, false);
}

BOOST_AUTO_TEST_CASE(Map_Values_Range_should_not_be_empty_if_input_range_was_not_empty) {
	std::map<int, int> m;
	m.insert({1, 1});
	m.insert({2, 2});
	ph::adaptor::Map_Values<decltype(m.begin()), decltype(m.end())> mk{m.begin(), m.end()};
	bool called = false;
	ph::for_each(mk.begin(), mk.end(), [&called](decltype(*mk.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, true);
}

BOOST_AUTO_TEST_CASE(Map_Values_Pipe_should_work_on_empty_range) {
	std::map<int, int> m;

	auto map_range = ph::make_iterator_range(m.begin(), m.end());
	auto range = map_range | ph::adaptor::map_values;

	bool called = false;
	ph::for_each(range.begin(), range.end(), [&called](decltype(*range.begin()) v) {
			called = true;
			(void)v;
	});
	BOOST_CHECK_EQUAL(called, false);

}


BOOST_AUTO_TEST_SUITE_END()
