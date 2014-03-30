#ifndef ADAPTORS_MAP_HPP_
#define ADAPTORS_MAP_HPP_
#include <type_traits>
#include "range.hpp"

namespace ph { namespace adaptor {

namespace detail {
	template<typename Iterator>
	class map_key_iterator: public std::decay<Iterator>::type {
	public:
		using Base = typename std::decay<Iterator>::type;
		map_key_iterator(Iterator it): Base(it) {}
		map_key_iterator() = default;

		const auto& operator*() const { return Base::operator*().first; }
		auto& operator*() { return Base::operator*().first; }

	};

	struct dummy_map_range { };
} // namespace detail

template<typename Begin, typename End>
class Map_Keys {
	detail::map_key_iterator<Begin> b;
	detail::map_key_iterator<End> e;

public:

	Map_Keys() = default;
	Map_Keys(Begin _b, End _e):
		b(detail::map_key_iterator<Begin>{_b}),
		e(detail::map_key_iterator<End>{_e}) {}

	detail::map_key_iterator<Begin>& begin() { return b; }
	const detail::map_key_iterator<Begin>& begin() const { return b; }

	detail::map_key_iterator<End>& end() { return e; }
	const detail::map_key_iterator<End>& end() const { return e; }

};

auto map_keys() { return detail::dummy_map_range{}; }

}} // namespace ph::adaptor

template<typename Range>
auto operator|(Range r, ph::adaptor::detail::dummy_map_range) {
	return ph::adaptor::Map_Keys<decltype(r.begin()), decltype(r.end())>{r.begin(), r.end()};
}



#endif /* ADAPTORS_MAP_HPP_ */
