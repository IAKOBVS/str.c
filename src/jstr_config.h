#ifndef JSTR_CONFIG__H
#define JSTR_CONFIG__H 1

#define PJSTR_MAX_UINT_DIGITS	    10
#define PJSTR_MAX_ULONG_DIGITS	    19
#define PJSTR_MAX_ULONG_LONG_DIGITS 19

#define PJSTR_MAX_INT_DIGITS	   (PJSTR_MAX_UINT_DIGITS + 1)
#define PJSTR_MAX_LONG_DIGITS	   (PJSTR_MAX_ULONG_DIGITS + 1)
#define PJSTR_MAX_LONG_LONG_DIGITS (PJSTR_MAX_ULONG_LONG_DIGITS + 1)

#define PJSTR_MIN_MMAP (128 * 1024)

#define PJSTR_MIN_CAP ((sizeof(size_t) == 8) ? 24 : 16)

#define PJSTR_MALLOC_ALIGNMENT (sizeof(size_t) + sizeof(size_t))

/*
   realloc() must malloc(), memcpy(), free() for mmaped chunks if realloc does not use mremap.
   Mremap may be available on  __linux__ || __GLIBC__.
   Enable JSTR_COPY_IF_NO_MREMAP if you know that mremap is not available.
*/
#ifndef JSTR_COPY_IF_NO_MREMAP
#	define JSTR_COPY_IF_NO_MREMAP 0
#endif

#ifndef PJSTR_NO_UNROLL
#	define PJSTR_NO_UNROLL 0
#endif

#ifndef PJARR_GROWTH
#	define PJARR_GROWTH 1.5
#endif

#ifndef PJARR_ALLOC_MULTIPLIER
#	define PJARR_ALLOC_MULTIPLIER 1.5
#endif

#ifndef JSTR_GROWTH
#	define JSTR_GROWTH 1.5
#endif

#ifndef JSTR_ALLOC_MULTIPLIER
#	define JSTR_ALLOC_MULTIPLIER 2
#endif

#ifndef JSTR_EXIT_ON_MALLOC_ERROR
#	define JSTR_EXIT_ON_MALLOC_ERROR 1
#endif

#ifndef JSTR_ERR_MSG_ON_MALLOC_ERROR
#	define JSTR_ERR_MSG_ON_MALLOC_ERROR 1
#endif

#ifndef JSTR_ERR_MSG_ON_REGEX_ERROR
#	define JSTR_ERR_MSG_ON_REGEX_ERROR 1
#endif

#ifndef JSTR_FREE_ON_DESTRUCTOR_CPP
#	define JSTR_FREE_ON_DESTRUCTOR_CPP 0
#endif

#ifndef JSTR_NULLIFY_PTR_ON_DESTRUCTOR_CPP
#	define JSTR_NULLIFY_PTR_ON_DESTRUCTOR_CPP 1
#endif

#ifndef JSTR_NULLIFY_PTR_ON_DELETE
#	define JSTR_NULLIFY_PTR_ON_DELETE 0
#endif

#ifndef PJARR_NULLIFY_PTR_ON_DELETE
#	define PJARR_NULLIFY_PTR_ON_DELETE 0
#endif

#if JSTR_FREE_ON_DESTRUCTOR_CPP
#	undef JSTR_NULLIFY_PTR_ON_DELETE
#	define JSTR_NULLIFY_PTR_ON_DELETE 1
#endif

#endif /* JSTR_CONFIG__H */
