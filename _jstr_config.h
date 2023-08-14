#ifndef JSTR_CONFIG_DEF_H
#define JSTR_CONFIG_DEF_H

#define JSTR_MAX_INT_DIGITS	  11
#define JSTR_MAX_LONG_DIGITS	  20
#define JSTR_MAX_LONG_LONG_DIGITS 39

#define JSTR_MIN_MMAP (128 * 1024)

#define JSTR_MAX_UINT_DIGITS	   (JSTR_MAX_INT_DIGITS + JSTR_MAX_INT_DIGITS)
#define JSTR_MAX_ULONG_DIGITS	   (JSTR_MAX_LONG_DIGITS + JSTR_MAX_LONG_DIGITS)
#define JSTR_MAX_ULONG_LONG_DIGITS (JSTR_MAX_LONG_LONG_DIGITS + JSTR_MAX_LONG_LONG_DIGITS)

#define JSTR_MIN_CAP 24

#define JSTR_GROWTH_MULTIPLIER 1.5
#define JSTR_ALLOC_MULTIPLIER  2

#define JSTR_EXIT_ON_MALLOC_ERROR	   1
#define JSTR_PRINT_ERR_MSG_ON_MALLOC_ERROR 1
#define JSTR_PRINT_ERR_MSG_ON_REGEX_ERROR  1

#define JSTR_FREE_ON_DESTRUCTOR_CPP	   1
#define JSTR_NULLIFY_PTR_ON_DESTRUCTOR_CPP 1
#define JSTR_NULLIFY_PTR_ON_DELETE	   1

#if JSTR_FREE_ON_DESTRUCTOR_CPP
#	undef JSTR_NULLIFY_PTR_ON_DELETE
#	define JSTR_NULLIFY_PTR_ON_DELETE 1
#endif /* JSTR_FREE_ON_DESTRUCTOR_CPP */

#ifndef JSTR_GROWTH_MULTIPLIER
#	define JSTR_GROWTH_MULTIPLIER 2
#endif /* JSTR_GROWTH_MULTIPLIER */

#endif /* JSTR_CONFIG_DEF_H */
