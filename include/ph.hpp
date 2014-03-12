#ifndef PH_HPP_
#define PH_HPP_

#include <utility>
#include <type_traits>
#include <boost/type_traits/has_equal_to.hpp>

namespace ph {


template<typename Iterator, typename Constraint>
typename std::enable_if<boost::has_equal_to<Iterator, Constraint, bool>::value, bool>::type checkPredicate(Iterator it, Constraint constraint) {
	return it == constraint;
}

template<typename Iterator, typename Constraint>
typename std::enable_if<!boost::has_equal_to<Iterator, Constraint, bool>::value, bool>::type checkPredicate(Iterator it, Constraint constraint) {
	return constraint(*it);
}

template<class Constraint>
struct LeafNode {

	LeafNode(const Constraint& constraint) : constraint(constraint) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return checkPredicate(std::forward<Iterator>(it), constraint);
	}

	Constraint constraint;
};

template<typename LeftNode, typename RightNode>
struct OrNode {

	OrNode(const LeftNode& leftNode, const RightNode& rightNode) : leftNode(leftNode), rightNode(rightNode) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return leftNode(std::forward<Iterator>(it)) || rightNode(std::forward<Iterator>(it));
	}

	LeftNode leftNode;
	RightNode rightNode;
};

template<typename LeftNode, typename RightNode>
struct AndNode {

	AndNode(const LeftNode& leftNode, const RightNode& rightNode) : leftNode(leftNode), rightNode(rightNode) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return leftNode(std::forward<Iterator>(it)) && rightNode(std::forward<Iterator>(it));
	}

	LeftNode leftNode;
	RightNode rightNode;
};

template<typename OperandNode>
struct NotNode {

	NotNode(const OperandNode& operandNode) : operandNode(operandNode) {}

	template<typename Iterator>
	bool operator()(Iterator&& it) const {
		return !operandNode(std::forward<Iterator>(it));
	}

	OperandNode operandNode;
};

template<class T>
struct IsNode : std::false_type {};

template<typename R, typename L>
struct IsNode<OrNode<R, L>> : std::true_type {};

template<typename R, typename L>
struct IsNode<AndNode<R, L>> : std::true_type {};

template<typename T>
struct IsNode<NotNode<T>> : std::true_type {};

template<typename T>
struct IsNode<LeafNode<T>> : std::true_type {};

template<typename It, typename Node>
typename std::enable_if<IsNode<Node>::value, bool>::type operator==(const Node& node, It&& it) {
	return node(std::forward<It>(it));
}

template<typename It, typename Node>
typename std::enable_if<IsNode<Node>::value, bool>::type operator==(It&& it, const Node& node) {
	return node == std::forward<It>(it);
}

template<typename It, typename Node>
typename std::enable_if<IsNode<Node>::value, bool>::type operator!=(const Node& node, It&& it) {
	return !(node == std::forward<It>(it));
}

template<typename It, typename Node>
typename std::enable_if<IsNode<Node>::value, bool>::type operator!=(It&& it, const Node& node) {
	return !(node == std::forward<It>(it));
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	IsNode<LeftNode>::value && IsNode<RightNode>::value,
	OrNode<LeftNode, RightNode>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeftNode, RightNode>(leftNode, rightNode);
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	IsNode<LeftNode>::value && !IsNode<RightNode>::value,
	OrNode<LeftNode, LeafNode<RightNode>>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeftNode, LeafNode<RightNode>>(leftNode, LeafNode<RightNode>(rightNode));
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	!IsNode<LeftNode>::value && IsNode<RightNode>::value,
	OrNode<LeafNode<LeftNode>, LeafNode<RightNode>>>::type operator||(const LeftNode& leftNode, const RightNode& rightNode) {
	return OrNode<LeafNode<LeftNode>, RightNode>(LeafNode<LeftNode>(leftNode), rightNode);
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	IsNode<LeftNode>::value && IsNode<RightNode>::value,
	AndNode<LeftNode, RightNode>>::type operator&&(const LeftNode& leftNode, const RightNode& rightNode) {
	return AndNode<LeftNode, RightNode>(leftNode, rightNode);
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	IsNode<LeftNode>::value && !IsNode<RightNode>::value,
	AndNode<LeftNode, LeafNode<RightNode>>>::type operator&&(const LeftNode& leftNode, const RightNode& rightNode) {
	return AndNode<LeftNode, LeafNode<RightNode>>(leftNode, LeafNode<RightNode>(rightNode));
}

template<typename LeftNode, typename RightNode>
typename std::enable_if<
	!IsNode<LeftNode>::value && IsNode<RightNode>::value,
	AndNode<LeafNode<LeftNode>, LeafNode<RightNode>>>::type operator&&(const LeftNode& leftNode, const RightNode& rightNode) {
	return AndNode<LeafNode<LeftNode>, RightNode>(LeafNode<LeftNode>(leftNode), rightNode);
}

template<typename OperandNode>
typename std::enable_if<
	IsNode<OperandNode>::value,
	NotNode<OperandNode>>::type operator!(const OperandNode& operandNode) {
	return NotNode<OperandNode>(operandNode);
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
