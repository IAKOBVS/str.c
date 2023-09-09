#ifndef PJSTR_PP_VA_ARGS_MACROS_DEF_H
#define PJSTR_PP_VA_ARGS_MACROS_DEF_H 1

#include "jstr_pp_st_assert.h"
#include "jstr_pp_strcpy_va_args.h"
#include "jstr_pp_strlen_va_args.h"
#include "jstr_pp_va_args_tools.h"

#define PJSTR_PP_CONCATENATE(A, B) \
	PJSTR_PP_CONCATENATE_(A, B)

#define PJSTR_PP_CONCATENATE_(A, B) \
	A##B

#ifndef __cplusplus
#	define PJSTR_PP_PRINTF_FORMAT(x) _Generic((x),                       \
						       unsigned char              \
						       : "%hhu\n",                \
							 unsigned char *          \
						       : "%s\n",                  \
							 char                     \
						       : "%c\n",                  \
							 char *                   \
						       : "%s\n",                  \
							 short                    \
						       : "%hd\n",                 \
							 unsigned short           \
						       : "%hu\n",                 \
							 unsigned int             \
						       : "%u\n",                  \
							 int                      \
						       : "%d\n",                  \
							 double                   \
						       : "%f\n",                  \
							 long double              \
						       : "%Lf\n",                 \
							 float                    \
						       : "%f\n",                  \
							 size_t                   \
						       : "%zu\n",                 \
							 long                     \
						       : "%ld\n",                 \
							 long long                \
						       : "%lld\n",                \
							 unsigned long long       \
						       : "%llu\n",                \
                                                                                  \
							 const unsigned char      \
						       : "%hhu\n",                \
							 const unsigned char *    \
						       : "%s\n",                  \
							 const char               \
						       : "%c\n",                  \
							 const char *             \
						       : "%s\n",                  \
							 const short              \
						       : "%hd\n",                 \
							 const unsigned short     \
						       : "%hu\n",                 \
							 const unsigned int       \
						       : "%u\n",                  \
							 const int                \
						       : "%d\n",                  \
							 const double             \
						       : "%f\n",                  \
							 const long double        \
						       : "%Lf\n",                 \
							 const float              \
						       : "%f\n",                  \
							 const size_t             \
						       : "%zu\n",                 \
							 const long               \
						       : "%ld\n",                 \
							 const long long          \
						       : "%lld\n",                \
							 const unsigned long long \
						       : "%llu\n")
#	define jstr_pp_cout(x) printf(PJSTR_PP_PRINTF_FORMAT(x), x)
#else
#	define jstr_pp_cout(x)
#endif /* __cplusplus */

#endif /* PJSTR_PP_VA_ARGS_MACROS_DEF_H */
