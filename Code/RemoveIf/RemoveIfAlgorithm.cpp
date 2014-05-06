template <class ForwardIterator, class UnaryPredicate>
ForwardIterator remove_if (ForwardIterator first, ForwardIterator last,
	UnaryPredicate pred)
{
	ForwardIterator result = first;
	while (first!=last) {
		if (!pred(*first)) {
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}
