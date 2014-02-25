#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_

// The below algorithms are ports from
// http://en.cppreference.com/w/cpp/header/algorithm
//
// The main changes to them are that instead of requiring a single iterator
// type for both the begin and the end iterator, the simply require that the
// begin be a forward iterator, and that begin is comparable to end.

#include <utility>

namespace ph {

// Non-modifying sequence operations.

template<typename Begin, typename End, typename ValueType>
Begin find(Begin begin, End end, const ValueType& value) {
	for(; end != begin; ++begin) {
		if(*begin == value) {
			return begin;
		}
	}
	return begin;
}

template<typename Begin, typename End, typename UnaryPredicate>
Begin find_if(Begin begin, End end, UnaryPredicate p) {
	for(; end != begin; ++begin) {
		if(p(*begin))
			return begin;
	}
	return begin;
}

template<typename Begin, typename End, typename UnaryPredicate>
Begin find_if_not(Begin begin, End end, UnaryPredicate q) {
	for(; end != begin; ++begin) {
		if(!q(*begin))
			return begin;
	}
	return begin;
}

template<typename Begin, typename End, typename UnaryPredicate>
bool all_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if_not(begin, end, p) == end;
}

template<typename Begin, typename End, typename UnaryPredicate>
bool any_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if(begin, end, p) != end;
}

template<typename Begin, typename End, typename UnaryPredicate>
bool none_of(Begin begin, End end, UnaryPredicate p) {
	return ph::find_if(begin, end, p) == end;
}

template<typename Begin, typename End, class UnaryFunction>
UnaryFunction for_each(Begin begin, End end, UnaryFunction f) {
	for (; end != begin; ++begin) {
		f(*begin);
	}
	return std::move(f);
}

template<class Begin, class End, class T>
std::size_t count(Begin begin, End end, const T& value) {
	std::size_t ret = 0;
	for (; end != begin; ++begin) {
		if (*begin == value) {
			++ret;
		}
	}
	return ret;
}

template<class Begin, class End, class UnaryPredicate>
std::size_t count_if(Begin begin, End end, UnaryPredicate p) {
	std::size_t ret = 0;
	for (; end != begin; ++begin) {
		if (p(*begin)) {
			++ret;
		}
	}
	return ret;
}

// TODO: mismatch, and all its overloads.

// TODO: equal and all its overloads.

// TODO: find_end and all its overloads.

// TODO: find_first_of and all its overloads.

// TODO: adjacent_find and all its overloads.

// TODO: search(_n) all their overloads.


// TODO: Other operation categories.

template<typename Begin, typename End>
Begin max_element(Begin begin, End end) {
	Begin answerIterator = begin++;

	for(; end != begin; ++begin) {
		if(*answerIterator < *begin)
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Begin, typename End, typename Comp>
Begin max_element(Begin begin, End end, Comp comp) {
	Begin answerIterator = begin++;

	for(; end != begin; ++begin) {
		if(comp(*answerIterator, *begin))
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Begin, typename End>
Begin min_element(Begin begin, End end) {
	Begin answerIterator = begin++;

	for(; end != begin; ++begin) {
		if(*begin < *answerIterator)
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Begin, typename End, typename Comp>
Begin min_element(Begin begin, End end, Comp comp) {
	Begin answerIterator = begin++;

	for(; end != begin; ++begin) {
		if(comp(*begin, *answerIterator))
			answerIterator = begin;
	}

	return answerIterator;
}

} // namespace ph

#endif /* ALGORITHM_HPP_ */
