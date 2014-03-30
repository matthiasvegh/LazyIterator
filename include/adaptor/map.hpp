#ifndef ADAPTORS_MAP_HPP_
#define ADAPTORS_MAP_HPP_
#include "range.hpp"

namespace ph { namespace adaptor {

namespace detail {
	template<typename Iterator>
	class map_key_iterator: public Iterator {
	public:
		map_key_iterator(Iterator it): Iterator(it) {}
		map_key_iterator() = default;

		const auto& operator*() const { return Iterator::operator*().first; }
		auto& operator*() { return Iterator::operator*().first; }

	};
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


}} // namespace ph::adaptor

#endif /* ADAPTORS_MAP_HPP_ */
