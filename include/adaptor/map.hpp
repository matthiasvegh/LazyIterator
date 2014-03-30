#ifndef ADAPTORS_MAP_HPP_
#define ADAPTORS_MAP_HPP_
#include <type_traits>
#include "range.hpp"
#include "ph.hpp"

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

	template<typename Iterator>
	class map_value_iterator: public std::decay<Iterator>::type {
	public:
		using Base = typename std::decay<Iterator>::type;
		map_value_iterator(Iterator it): Base(it) {}
		map_value_iterator() = default;

		const auto& operator*() const { return Base::operator*().second; }
		auto& operator*() { return Base::operator*().second; }

	};

	struct dummy_map_keys_range { };
	struct dummy_map_values_range { };
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

template<typename Begin, typename End>
class Map_Values {
	detail::map_key_iterator<Begin> b;
	detail::map_key_iterator<End> e;

public:

	Map_Values() = default;
	Map_Values(Begin _b, End _e):
		b(detail::map_value_iterator<Begin>{_b}),
		e(detail::map_value_iterator<End>{_e}) {}

	detail::map_key_iterator<Begin>& begin() { return b; }
	const detail::map_key_iterator<Begin>& begin() const { return b; }

	detail::map_key_iterator<End>& end() { return e; }
	const detail::map_key_iterator<End>& end() const { return e; }

};

const static auto map_keys = detail::dummy_map_keys_range{};
const static auto map_values = detail::dummy_map_values_range{};

} // namespace ph::adaptor

template<typename T>
struct IsNode<adaptor::detail::map_key_iterator<T>> : std::true_type {};

template<typename T>
struct IsNode<adaptor::detail::map_value_iterator<T>> : std::true_type {};

} // namespace ph

template<typename Range>
auto operator|(Range r, ph::adaptor::detail::dummy_map_keys_range) {
	return ph::adaptor::Map_Keys<decltype(r.begin()), decltype(r.end())>{r.begin(), r.end()};
}

template<typename Range>
auto operator|(Range r, ph::adaptor::detail::dummy_map_values_range) {
	return ph::adaptor::Map_Keys<decltype(r.begin()), decltype(r.end())>{r.begin(), r.end()};
}



#endif /* ADAPTORS_MAP_HPP_ */
