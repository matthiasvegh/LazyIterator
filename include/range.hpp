#ifndef RANGE_HPP_
#define RANGE_HPP_
#include <utility>

template<typename Begin, typename End>
class Range {
	Begin _begin;
	End _end;
public:

	Range(Begin _begin, End _end) :
		_begin(std::move(_begin)),
		_end(std::move(_end))
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


#endif /* RANGE_HPP_ */
