
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

BOOST_AUTO_TEST_CASE(until_with_value_predicate_should_compare_equal_to_iterator_pointing_to_that_value) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	auto iterator = ph::until([](const int& i) { return i == 2; });

	BOOST_CHECK(iterator == ++(v.begin()));
}

BOOST_AUTO_TEST_CASE(until_with_value_predicate_should_not_compare_equal_to_iterator_not_pointing_to_that_value) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	auto iterator = ph::until([](const int& i) { return i == 5; });

	BOOST_CHECK(iterator != ++(v.begin()));
}

BOOST_AUTO_TEST_CASE(When_untils_are_composed_find_should_stop_when_any_of_them_compare_equal) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	auto iterator1 = ph::until([](const int& i) { return i == 3; });
	auto iterator2 = ph::until([](const int& i) { return i == 4; });

	auto pos = ph::find(v.begin(), iterator1 || iterator2, 5);

	BOOST_CHECK_EQUAL(*pos, 3);

}

BOOST_AUTO_TEST_CASE(Using_find_until_composition_should_be_commutative) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	auto iterator1 = ph::until([](const int& i) { return i == 3; });
	auto iterator2 = ph::until([](const int& i) { return i == 4; });

	auto pos1 = ph::find(v.begin(), iterator1 || iterator2, 5);
	auto pos2 = ph::find(v.begin(), iterator2 || iterator1, 5);

	BOOST_CHECK_EQUAL(std::addressof(*pos1), std::addressof(*pos2));

}

BOOST_AUTO_TEST_CASE(Using_find_until_should_work_if_until_predicates_are_runtime) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	const volatile int delimiter = 3;
	auto iterator = ph::until([=](const int& i) { return i == delimiter; });

	auto posPh = ph::find(v.begin(), iterator, 4);
	auto posStl = std::find(v.begin(), v.begin()+2, 4);

	BOOST_CHECK_EQUAL(*posPh, *posStl);
}

BOOST_AUTO_TEST_CASE(Using_find_with_until_vals_should_work_as_if_many_untils_were_composed) {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);

	auto iterator1 = ph::until([](const int& i) { return i == 3; });
	auto iterator2 = ph::until([](const int& i) { return i == 4; });
	auto iterator3 = ph::until([](const int& i) { return i == 5; });

	auto posComposite = ph::find(v.begin(), iterator1 || iterator2 || iterator3, 6);
	auto posUntilVal = ph::find(v.begin(), ph::untilValue(3, 4, 5), 6);

	BOOST_CHECK(posComposite == posUntilVal);
}

BOOST_AUTO_TEST_SUITE_END()

