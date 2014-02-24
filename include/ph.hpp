#ifndef PH_HPP_
#define PH_HPP_

#include <tuple>
#include <type_traits>

#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/size.hpp>

namespace ph {

template<typename Begin, typename End, typename ValueType>
Begin find(Begin begin, End end, const ValueType& value) {
	for(; end != begin; ++begin) {
		if(*begin == value) {
			return begin;
		}
	}
	return begin;
}

template<typename Begin, typename End>
Begin max_element(Begin begin, End end) {
	Begin answerIterator = begin++;

	for(; end != begin; ++begin) {
		if(*begin > *answerIterator)
			answerIterator = begin;
	}

	return answerIterator;
}

template<typename Begin, typename End, class UnaryFunction>
UnaryFunction for_each(Begin begin, End end, UnaryFunction f) {
	for (; end != begin; ++begin) {
		f(*begin);
	}
	return f;
}

struct LazyStrIterator {
	template<typename Iterator>
	inline
	bool operator==(Iterator&& other) {
		return *other == '\0';
	}

	template<typename Iterator>
	inline
	bool operator!=(Iterator&& other) {
		return !(*this == std::forward<Iterator>(other));
	}
};

namespace detail {

	template<typename Iterator, typename ConstraintToCheck, bool onValue>
	struct CheckPredicateHelper;

	template<typename Iterator, typename ConstraintToCheck>
	struct CheckPredicateHelper<Iterator, ConstraintToCheck, true> {

		CheckPredicateHelper(ConstraintToCheck constraintToCheck) : constraintToCheck(constraintToCheck) {}

		bool operator()(Iterator it) const {
			return constraintToCheck(*it);
		}

		ConstraintToCheck constraintToCheck;
	};

	template<typename Iterator, typename ConstraintToCheck>
	struct CheckPredicateHelper<Iterator, ConstraintToCheck, false> {

		CheckPredicateHelper(ConstraintToCheck constraintToCheck) : constraintToCheck(constraintToCheck) {}

		bool operator()(Iterator it) const {
			return constraintToCheck(it);
		}

		ConstraintToCheck constraintToCheck;
	};

	template<typename ValueType>
	bool isElementOf(const ValueType& l, const ValueType& r) {
		return l == r;
	}

	template<typename ValueType, typename... ValueTypes>
	bool isElementOf(const ValueType& l, const ValueType& r, const ValueTypes&... os) {
		return l == r || isElementOf(l, os...);
	}

} // namespace detail

template<typename Iterator, typename ConstraintToCheck,
		bool runOnValue=std::is_same<
			typename std::decay<
				typename boost::mpl::at_c<
					typename boost::function_types::parameter_types<decltype(&ConstraintToCheck::operator())>
					,
					1
				>::type
			>::type
			,
			typename std::decay<decltype(*std::declval<Iterator>())>::type
		>::value
>
bool checkPredicate(Iterator it, ConstraintToCheck constraint) {
	return detail::CheckPredicateHelper<Iterator, ConstraintToCheck, runOnValue>{constraint}(it);
}

template<class Iterator, class T1, std::size_t Index = 0>
typename std::enable_if<Index >= std::tuple_size<T1>::value, bool>::type
checkPredicates(Iterator&& it, const T1& predicates) {
	return false;
}

template<class Iterator, class T1, std::size_t Index = 0>
typename std::enable_if<Index < std::tuple_size<T1>::value, bool>::type
checkPredicates(Iterator&& it, const T1& predicates) {
	return checkPredicate(it, std::get<Index>(predicates)) || checkPredicates<Iterator, T1, Index+1>(it, predicates);
}

template<class TupleType>
struct Until {
	Until(const TupleType& predicates) : predicates(predicates) {}

	template<typename Iterator>
	inline
	bool operator==(Iterator&& other) {
		return checkPredicates(std::forward<Iterator>(other), predicates);
	}

	template<typename Iterator>
	inline
	bool operator!=(Iterator&& other) {
		return !(*this == std::forward<Iterator>(other));
	}

	TupleType predicates;
};

template<class T1, class T2>
auto concatUntils(const Until<T1>& t1, const Until<T2>& t2) {
	return Until<decltype(std::tuple_cat(t1.predicates, t2.predicates))>{std::tuple_cat(t1.predicates, t2.predicates)};
}

template<class T1, class T2>
inline
auto operator||(const Until<T1>& t1, const Until<T2>& t2) {
	return concatUntils(t1, t2);
}

template<class T1, typename RealIterator>
inline
auto operator||(const Until<T1>& t1, RealIterator iterator) {
	auto lambda = [=](RealIterator it) { return it == iterator; };
	return concatUntils(t1, Until<std::tuple<decltype(lambda)>>(std::make_tuple(std::move(lambda))));
}

template<typename Predicate>
auto until(Predicate&& p) { return Until<std::tuple<Predicate>>{std::make_tuple(p)}; }

template<typename ValueType, typename... ValueTypes>
auto untilValue(ValueType vt, ValueTypes... vts) {
	auto lambda = [=](const ValueType& v) { return detail::isElementOf(v, vt, vts...); };
	return Until<std::tuple<decltype(lambda)>>(std::make_tuple(std::move(lambda)));
}

} // namespace ph


#endif
