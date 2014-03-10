
#include <utility>

namespace ph {

struct LazyStrIterator {
	template<typename Iterator>
	bool operator==(Iterator&& other) const {
		return *other == '\0';
	}
};

template<typename It> bool operator==(It&& it, const LazyStrIterator& s) { return s == std::forward<It>(it); }
template<typename It> bool operator!=(const LazyStrIterator& s, It&& it) { return !(s == std::forward<It>(it)); }
template<typename It> bool operator!=(It&& it, const LazyStrIterator& s) { return !(s == std::forward<It>(it)); }

}

