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
	bool operator==(Iterator&& other) const {
		return *other == '\0';
	}
};

template<typename It> bool operator==(It&& it, const LazyStrIterator& s) { return s == std::forward<It>(it); }
template<typename It> bool operator!=(const LazyStrIterator& s, It&& it) { return !(s == std::forward<It>(it)); }
template<typename It> bool operator!=(It&& it, const LazyStrIterator& s) { return !(s == std::forward<It>(it)); }

template<typename Iterator, typename Constraint>
typename std::enable_if<std::is_same<Iterator, Constraint>::value, bool>::type checkPredicate(Iterator it, Constraint constraint) {
	return it == constraint;
}

template<typename Iterator, typename Constraint>
typename std::enable_if<!std::is_same<Iterator, Constraint>::value, bool>::type checkPredicate(Iterator it, Constraint constraint) {
	return constraint(*it);
}

struct BaseNode {};

template<class Constraint>
struct LeafNode : BaseNode {

	LeafNode(const Constraint& constraint) : constraint(constraint) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return checkPredicate(std::forward<Iterator>(it), constraint);
	}

	Constraint constraint;
};

template<typename LeftNode, typename RightNode>
struct OrNode : BaseNode {

	OrNode(const LeftNode& leftNode, const RightNode& rightNode) : leftNode(leftNode), rightNode(rightNode) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return leftNode(std::forward<Iterator>(it)) || rightNode(std::forward<Iterator>(it));
	}

	LeftNode leftNode;
	RightNode rightNode;
};

template<typename It, typename Node>
typename std::enable_if<std::is_base_of<BaseNode, Node>::value, bool>::type operator==(const Node& node, It&& it) {
	return node(std::forward<It>(it));
}

template<typename It, typename Node>
typename std::enable_if<std::is_base_of<BaseNode, Node>::value, bool>::type operator==(It&& it, const Node& node) {
	return node == std::forward<It>(it);
}

template<typename It, typename Node>
typename std::enable_if<std::is_base_of<BaseNode, Node>::value, bool>::type operator!=(const Node& node, It&& it) {
	return !(node == std::forward<It>(it));
}

template<typename It, typename Node>
typename std::enable_if<std::is_base_of<BaseNode, Node>::value, bool>::type operator!=(It&& it, const Node& node) {
	return !(node == std::forward<It>(it));
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	std::is_base_of<BaseNode, LeftNode>::value && std::is_base_of<BaseNode, RightNode>::value,
	OrNode<LeftNode, RightNode>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeftNode, RightNode>(leftNode, rightNode);
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	std::is_base_of<BaseNode, LeftNode>::value && !std::is_base_of<BaseNode, RightNode>::value,
	OrNode<LeftNode, LeafNode<RightNode>>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeftNode, LeafNode<RightNode>>(leftNode, LeafNode<RightNode>(rightNode));
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	!std::is_base_of<BaseNode, LeftNode>::value && std::is_base_of<BaseNode, RightNode>::value,
	OrNode<LeafNode<LeftNode>, LeafNode<RightNode>>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeafNode<LeftNode>, RightNode>(LeafNode<LeftNode>(leftNode), rightNode);
}

template<typename Constraint>
LeafNode<Constraint> until(Constraint&& c) {
	return LeafNode<Constraint>(c);
}

template<typename Value>
auto untilValue(const Value& value) {
	return until([value](const Value& v) { return v == value; });
}

template<typename Value, typename... Values>
auto untilValue(const Value& value, const Values&... values) {
	return untilValue(value) || untilValue(values...);
}

} // namespace ph


#endif
