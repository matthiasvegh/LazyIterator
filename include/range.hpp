#ifndef RANGE_HPP_
#define RANGE_HPP_
#include <utility>
#include <iterator>

namespace ph {

template<typename Begin, typename End>
class Range {
	Begin _begin;
	End _end;
public:

	Range(const Begin _begin, const End& _end) :
		_begin(_begin),
		_end(_end)
	{}

	const Begin& begin() const { return _begin; }
	Begin& begin() { return _begin; }
	const End& end() const { return _end; }
	End& end() { return _end; }

};

template<typename Begin, typename End>
auto make_iterator_range(Begin b, End e) {
	return Range<Begin, End>{b, e};
}

template<typename Begin, typename End>
auto distance(Begin begin, End end) {
	typename std::iterator_traits<Begin>::difference_type answer = 0;

	for(; begin != end; ++begin) {
		++answer;
	}
	return answer;
}

template<typename Range>
auto distance(Range r) {
	using std::distance;
	return distance(r.begin(), r.end());
}

} // namespace ph

#endif /* RANGE_HPP_ */
