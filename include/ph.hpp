#ifndef PH_HPP_
#define PH_HPP_
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

struct LazyStrIterator {
	template<typename Iterator>
	inline
	bool operator==(Iterator&& other) {

		return *std::forward<Iterator>(other) == '\0';
	}

	template<typename Iterator>
	inline
	bool operator!=(Iterator&& other) {
		return ! (this->operator==(std::forward<Iterator>(other)));
	}
};

template<typename Predicate, typename... Predicates>
struct Until_t {

	template<typename Iterator, typename ConstraintToCheck,
			bool runOnIterator=std::is_same<
				typename std::decay<
					typename boost::mpl::at_c<
						typename boost::function_types::parameter_types<decltype(&ConstraintToCheck::operator())>
						,
						1
					>::type
				>::type
				,
				decltype(*std::declval<Iterator>())
			>::value
	>
	bool
	checkPredicate(Iterator it) {
		return ConstraintToCheck{}(*it);
	}

	template<typename Iterator, typename ConstraintToCheck, typename... ConstraintsToCheck>
	bool
	checkPredicates(Iterator&& it) {
		return checkPredicate<Iterator, ConstraintToCheck>(it) || checkPredicates<Iterator, ConstraintsToCheck...>(it);
	}

	template<typename Iterator>
	bool
	checkPredicates(Iterator&& it) {
		return false;
	}


	template<typename Iterator>
	inline
	bool operator==(Iterator&& other) {
		return checkPredicates<Iterator, Predicate, Predicates...>(std::forward<Iterator>(other));
	}

	template<typename Iterator>
	inline
	bool operator!=(Iterator&& other) {
		return ! (this->operator==(std::forward<Iterator>(other)));
	}

	template<typename OtherPredicate, typename... OtherPredicates>
	inline
	auto operator||(Until_t<OtherPredicate> other) {

		return Until_t<Predicate, Predicates..., OtherPredicate, OtherPredicates...>{};
	}
/*
	template<typename RealIterator>
	inline
	auto operator||(RealIterator iterator) {
		auto lambda = [=](const decltype(*iterator)& r) { return r == iterator; };

		return (Until_t<decltype(lambda)>{})||(*this);
	}
*/
};

template<typename Predicate>
Until_t<Predicate> Until(Predicate&& p) { return Until_t<Predicate>{}; }





} // namespace ph


#endif
