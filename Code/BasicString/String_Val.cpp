		// TEMPLATE CLASS _String_iterator
template<class _Elem,
	class _Traits,
	class _Alloc>
	class _String_iterator
		: public _String_const_iterator<_Elem, _Traits, _Alloc>
	{	// iterator for mutable string

		//...

	size_type _Mysize;	// current length of string
	size_type _Myres;	// current storage reserved for string
	_Alty _Alval;	// allocator object for strings
	};
