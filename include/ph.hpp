#ifndef PH_HPP_
#define PH_HPP_

#include <tuple>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include <boost/mpl/at.hpp>
#include <boost/function_types/parameter_types.hpp>

#include "detail.hpp"


namespace ph {

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
checkPredicates(Iterator&& /*it*/, const T1& /*predicates*/) {
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

template<typename Range>
auto untilValue(const Range& vts) {
	auto lambda = [=](const typename Range::value_type& v) {
		return std::find(vts.begin(), vts.end(), v) != vts.end();
	};
	return Until<std::tuple<decltype(lambda)>>(std::make_tuple(std::move(lambda)));
}

template<class Value>
auto untilValue(std::initializer_list<Value> vts) {
	return untilValue<std::initializer_list<Value>>(vts);
}

} // namespace ph


#endif
