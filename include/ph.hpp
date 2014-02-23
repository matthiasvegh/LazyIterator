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

template<typename Predicate>
struct Until_t {
	template<typename Iterator>
	inline
	bool operator==(Iterator&& other) {
		return Predicate{}(*std::forward<Iterator>(other));
	}

	template<typename Iterator>
	inline
	bool operator!=(Iterator&& other) {
		return ! (this->operator==(std::forward<Iterator>(other)));
	}

	template<typename OtherPredicate>
	inline
	auto operator||(Until_t<OtherPredicate> other) {
		using signature = decltype(&OtherPredicate::operator());

		using argumentTypes = typename boost::function_types::parameter_types<signature>;

		const int one = 2;
		static_assert(boost::mpl::size<argumentTypes>::value == one, "only unary predicates are allowed");

		using value_type = typename boost::mpl::at_c<argumentTypes, 1>::type;


		auto lambda = [](value_type v) { return Predicate{}(v) || OtherPredicate{}(v); };


		return Until_t<decltype(lambda)>{};
	}


};

template<typename Predicate>
Until_t<Predicate> Until(Predicate&& p) { return Until_t<Predicate>{}; }





} // namespace ph


#endif
