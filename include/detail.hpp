#ifndef DETAIL_HPP_
#define DETAIL_HPP_

namespace ph {

namespace detail {

	template<typename Iterator, typename ConstraintToCheck, bool onValue>
	struct CheckPredicateHelper;

	template<typename Iterator, typename ConstraintToCheck>
	struct CheckPredicateHelper<Iterator, ConstraintToCheck, true> {

		CheckPredicateHelper(ConstraintToCheck constraintToCheck) : constraintToCheck(constraintToCheck) {}

		bool operator()(Iterator it) const {
			return constraintToCheck(*it);
		}

		ConstraintToCheck constraintToCheck;
	};

	template<typename Iterator, typename ConstraintToCheck>
	struct CheckPredicateHelper<Iterator, ConstraintToCheck, false> {

		CheckPredicateHelper(ConstraintToCheck constraintToCheck) : constraintToCheck(constraintToCheck) {}

		bool operator()(Iterator it) const {
			return constraintToCheck(it);
		}

		ConstraintToCheck constraintToCheck;
	};

} // namespace detail

} // namespace ph

#endif /* DETAIL_HPP_ */
