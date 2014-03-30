#ifndef ADAPTOR_FILTERED_HPP_
#define ADAPTOR_FILTERED_HPP_
#include "ph.hpp"
#include "range.hpp"

namespace ph { namespace adaptor {

namespace detail {

template<typename Begin, typename End, typename UnaryPredicate>
class FilteredBegin {
	Begin b;
	End e;
	UnaryPredicate p;
public:
	FilteredBegin() = default;
	FilteredBegin(Begin b, End e, UnaryPredicate p):
		b(b), e(e), p(p)
	{
		// prepare b.
		// If it is already end, do nothing.
		// if not, increment it till to a valid pos.

		for(; this->b!= e && !p(*this->b); ++this->b) {}
	}
	auto& operator++() {
		for(++b; b!=e && !p(*b); ++b) {}
		return *this;
	}

	auto& operator*() {
		return *b;
	}

	const auto& operator*() const {
		return *b;
	}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return it == b;
	}

};

template<typename UnaryPredicate>
struct dummy_filtered_range {UnaryPredicate predicate; };

} // namespace detail


template<typename Begin, typename End, typename UnaryPredicate>
class FilteredRange: public Range<Begin, End> {
	using Base = Range<Begin, End>;
	UnaryPredicate pred;
public:

	FilteredRange(const Begin& begin, const End& end, UnaryPredicate pred):
		Base(begin, end),
		pred(pred)
	{}

	auto begin() const {
		return detail::FilteredBegin<Begin, End, UnaryPredicate>(
			Base::begin(),
			Base::end(),
			pred);
	}

	auto& end() {
		return Base::end();
	}

	const auto& end() const {
		return Base::end();
	}


};

template<typename UnaryPredicate>
auto filtered(UnaryPredicate p) {
	return detail::dummy_filtered_range<UnaryPredicate>{p};
}

} // namespace ph::adaptor
template<typename Begin, typename End, typename UnaryPredicate>
struct IsNode<adaptor::detail::FilteredBegin<Begin, End, UnaryPredicate>> :
std::true_type {};

template<typename Range, typename UnaryPredicate>
auto operator|(Range r, ph::adaptor::detail::dummy_filtered_range<UnaryPredicate> fr) {
	return ph::adaptor::FilteredRange<
		decltype(r.begin()), decltype(r.end()), UnaryPredicate>(
				r.begin(), r.end(), fr.predicate);
}

} // namespace ph



#endif /* ADAPTOR_FILTERED_HPP_ */
