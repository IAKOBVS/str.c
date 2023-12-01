#ifndef JSTR_FEATURES_H
#define JSTR_FEATURES_H 1

#if (defined _ISOC11_SOURCE || defined _ISOC2X_SOURCE || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L))
#	define JSTR_USE_ISOC11 1
#endif

#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE || defined _ISOC2X_SOURCE || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
#	define JSTR_USE_ISOC99 1
#endif

#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE || defined _ISOC2X_SOURCE || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199409L))
#	define JSTR_USE_ISOC95 1
#endif

#ifdef __cplusplus
#	if __cplusplus >= 201703L
#		define JSTR_USE_ISOC11 1
#	endif
#	if __cplusplus >= 201103L || defined __GXX_EXPERIMENTAL_CXX0X__
#		define JSTR_USE_ISOCXX11 1
#		define JSTR_USE_ISOC99   1
#	endif
#endif

#ifdef _DEFAULT_SOURCE
#	if !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE
#		define JSTR_USE_POSIX_IMPLICITLY 1
#	endif
#endif

#if (defined _POSIX_SOURCE || (defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1) || defined _XOPEN_SOURCE)
#	define JSTR_USE_POSIX 1
#endif

#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2 || defined _XOPEN_SOURCE
#	define JSTR_USE_POSIX2 1
#endif

#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199309L
#	define JSTR_USE_POSIX199309 1
#endif

#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199506L
#	define JSTR_USE_POSIX199506 1
#endif

#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 200112L
#	define JSTR_USE_XOPEN2K 1
#	undef JSTR_USE_ISOC95
#	define JSTR_USE_ISOC95 1
#	undef JSTR_USE_ISOC99
#	define JSTR_USE_ISOC99 1
#endif

#ifdef _XOPEN_SOURCE
#	define JSTR_USE_XOPEN 1
#	if (_XOPEN_SOURCE - 0) >= 500
#		define JSTR_USE_XOPEN_EXTENDED 1
#		define JSTR_USE_UNIX98         1
#		if (_XOPEN_SOURCE - 0) >= 600
#			if (_XOPEN_SOURCE - 0) >= 700
#				define JSTR_USE_XOPEN2K8    1
#				define JSTR_USE_XOPEN2K8XSI 1
#			endif
#			define JSTR_USE_XOPEN2K    1
#			define JSTR_USE_XOPEN2KXSI 1
#			undef JSTR_USE_ISOC95
#			define JSTR_USE_ISOC95 1
#			undef JSTR_USE_ISOC99
#			define JSTR_USE_ISOC99 1
#		endif
#	else
#		ifdef _XOPEN_SOURCE_EXTENDED
#			define JSTR_USE_XOPEN_EXTENDED 1
#		endif
#	endif
#endif

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#	define JSTR_USE_FILE_OFFSET64 1
#endif

#if defined _DEFAULT_SOURCE
#	define JSTR_USE_MISC 1
#endif

#ifdef _ATFILE_SOURCE
#	define JSTR_USE_ATFILE 1
#endif

#ifdef _DYNAMIC_STACK_SIZE_SOURCE
#	define JSTR_USE_DYNAMIC_STACK_SIZE 1
#endif

#ifdef _GNU_SOURCE
#	define JSTR_USE_GNU 1
#endif

#endif /* JSTR_FEATURES_H */
