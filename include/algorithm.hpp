#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_

// The below algorithms are ports from
// http://en.cppreference.com/w/cpp/header/algorithm
//
// The main changes to them are that instead of requiring a single iterator
// type for both the begin and the end iterator, the simply require that the
// begin be a forward iterator, and that begin is comparable to end.

#include <utility>
#include <iterator>
#include <algorithm>

#include "range.hpp"

namespace ph {

// Non-modifying sequence operations.

template<typename Iterator, typename ValueType>
Iterator find(Iterator begin, Iterator end, const ValueType& value) {
	return std::find(begin, end, value);
}

template<typename Begin, typename End, typename ValueType>
Begin find(Begin begin, End end, const ValueType& value) {
	for(; begin != end; ++begin) {
		if(*begin == value) {
			return begin;
		}
	}
	return begin;
}

template<typename Begin, typename End, typename ValueType>
typename std::enable_if<std::is_same<Begin, End>::value, Begin>::type
find(Range<Begin, End> r, const ValueType& v) {
	return std::find<Begin, ValueType>(r.begin(), r.end(), v);
}

template<typename Begin, typename End, typename ValueType>
typename std::enable_if<!std::is_same<Begin, End>::value, Begin>::type
find(Range<Begin, End> r, const ValueType& v) {
	return ph::find<Begin, End, ValueType>(r.begin(), r.end(), v);
}

template<typename Iterator, typename UnaryPredicate>
Iterator find_if(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::find_if(begin, end, p);
}

template<typename Begin, typename End, typename UnaryPredicate>
Begin find_if(Begin begin, End end, UnaryPredicate p) {
	for(; begin != end; ++begin) {
		if(p(*begin))
			return begin;
	}
	return begin;
}

template<typename Begin, typename End, typename UnaryPredicate>
Begin find_if(Range<Begin, End> r, UnaryPredicate p) {
	return find_if(r.begin(), r.end(), p);
}

template<typename Iterator, typename UnaryPredicate>
Iterator find_if_not(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::find_if_not(begin, end, p);
}

template<typename Begin, typename End, typename UnaryPredicate>
Begin find_if_not(Begin begin, End end, UnaryPredicate q) {
	for(; begin != end; ++begin) {
		if(!q(*begin))
			return begin;
	}
	return begin;
}

template<typename Iterator, typename UnaryPredicate>
bool all_of(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::all_of(begin, end, p);
}

template<typename Begin, typename End, typename UnaryPredicate>
bool all_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if_not(begin, end, p) == end;
}

template<typename Iterator, typename UnaryPredicate>
bool any_of(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::any_of(begin, end, p);
}

template<typename Begin, typename End, typename UnaryPredicate>
bool any_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if(begin, end, p) != end;
}

template<typename Iterator, typename UnaryPredicate>
bool none_of(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::none_of(begin, end, p);
}

template<typename Begin, typename End, typename UnaryPredicate>
bool none_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if(begin, end, p) == end;
}

template<typename Iterator, typename UnaryFunction>
UnaryFunction for_each(Iterator begin, Iterator end, UnaryFunction f) {
	return std::for_each(begin, end, f);
}

template<typename Begin, typename End, typename UnaryFunction>
UnaryFunction for_each(Begin begin, End end, UnaryFunction f) {
	for (; begin != end; ++begin) {
		f(*begin);
	}
	return std::move(f);
}

template<typename Iterator, typename T>
typename std::iterator_traits<Iterator>::difference_type count(Iterator begin, Iterator end, const T& value) {
	return std::count(begin, end, value);
}

template<typename Begin, typename End, typename T>
typename std::iterator_traits<Begin>::difference_type count(Begin begin, End end, const T& value) {
	typename std::iterator_traits<Begin>::difference_type ret = 0;
	for (; begin != end; ++begin) {
		if (*begin == value) {
			++ret;
		}
	}
	return ret;
}

template<typename Iterator, typename UnaryPredicate>
typename std::iterator_traits<Iterator>::difference_type count_if(Iterator begin, Iterator end, UnaryPredicate p) {
	return std::count_if(begin, end, p);
}

template<class Begin, class End, class UnaryPredicate>
typename std::iterator_traits<Begin>::difference_type count_if(Begin begin, End end, UnaryPredicate p) {
	typename std::iterator_traits<Begin>::difference_type ret = 0;
	for (; begin != end; ++begin) {
		if (p(*begin)) {
			++ret;
		}
	}
	return ret;
}

template<typename Iterator1, typename Iterator2>
bool equal(Iterator1 begin1, Iterator1 end1, Iterator2 begin2) {
	return std::equal(begin1, end1, begin2);
}

template<typename Begin1, typename Begin2, typename End1>
bool equal(Begin1 begin1, End1 end1, Begin2 begin2) {
	while (begin1 != end1) {
		if (*begin1++ != *begin2++) {
			return false;
		}
	}
	return true;
}

template<typename Iterator1, typename Iterator2, typename UnaryPredicate>
bool equal(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, UnaryPredicate p) {
	return std::equal(begin1, end1, begin2, p);
}

template<typename Begin1, typename Begin2, typename End1, typename UnaryPredicate>
bool equal(Begin1 begin1, End1 end1, Begin2 begin2, UnaryPredicate p) {
	while (begin1 != end1) {
		if (!p(*begin1++, *begin2++)) {
			return false;
		}
	}
	return true;
}

// TODO: 4 iterator version of equal

// TODO: mismatch, and all its overloads.

// TODO: find_end and all its overloads.

// TODO: find_first_of and all its overloads.

// TODO: adjacent_find and all its overloads.

// TODO: search(_n) all their overloads.


// TODO: Other operation categories.

template<typename Iterator>
Iterator max_element(Iterator begin, Iterator end) {
	return std::max_element(begin, end);
}

template<typename Begin, typename End>
Begin max_element(Begin begin, End end) {
	Begin answerIterator = begin++;

	for(; begin != end; ++begin) {
		if(*answerIterator < *begin)
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Iterator, typename Comp>
Iterator max_element(Iterator begin, Iterator end, Comp comp) {
	return std::max_element(begin, end, comp);
}

template<typename Begin, typename End, typename Comp>
Begin max_element(Begin begin, End end, Comp comp) {
	Begin answerIterator = begin++;

	for(; begin != end; ++begin) {
		if(comp(*answerIterator, *begin))
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Iterator>
Iterator min_element(Iterator begin, Iterator end) {
	return std::min_element(begin, end);
}

template<typename Begin, typename End>
Begin min_element(Begin begin, End end) {
	Begin answerIterator = begin++;

	for(; begin != end; ++begin) {
		if(*begin < *answerIterator)
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Iterator, typename Comp>
Iterator min_element(Iterator begin, Iterator end, Comp comp) {
	return std::min_element(begin, end, comp);
}

template<typename Begin, typename End, typename Comp>
Begin min_element(Begin begin, End end, Comp comp) {
	Begin answerIterator = begin++;

	for(; begin != end; ++begin) {
		if(comp(*begin, *answerIterator))
			answerIterator = begin;
	}

	return answerIterator;
}

} // namespace ph

#endif /* ALGORITHM_HPP_ */
