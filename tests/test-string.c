#include "test.h"
#include "test-array.h"
#include "../src/jstr-string.h"

#define TOLOWER(c) (unsigned char)(((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))
#define TOUPPER(c) (unsigned char)(((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))

static char *
simple_strrstr_len(const char *h,
                   const size_t hl,
                   const char *n,
                   const size_t nl)
{
	if (nl == 0)
		return (char *)h + hl;
	if (hl < nl)
		return NULL;
	const char *p = h + hl - nl;
	for (; p >= h; --p)
		if (*p == *n && !memcmp(p, n, nl))
			return (char *)p;
	return NULL;
}

JSTR_ATTR_MAYBE_UNUSED
static char *
simple_strrstr(const char *h,
               const char *n)
{
	return simple_strrstr_len(h, strlen(h), n, strlen(n));
}

static char *
simple_memmem(const void *hs,
              const size_t hl,
              const void *ne,
              const size_t nl)
{
	const unsigned char *h = (const unsigned char *)hs;
	const unsigned char *n = (const unsigned char *)ne;
	if (nl == 0)
		return (char *)h;
	if (hl == 0)
		return NULL;
	if (hl < nl)
		return NULL;
	const unsigned char *const end = h + hl - nl;
	for (; h <= end; ++h)
		if (*h == *n && !memcmp(h, n, nl))
			return (char *)h;
	return NULL;
}

static char *
simple_strnstr(const char *hs,
               const char *ne,
               size_t n)
{
	return (char *)simple_memmem(hs, jstr_strnlen(hs, n), ne, strlen(ne));
}

JSTR_ATTR_MAYBE_UNUSED
static char *
simple_strstr(const char *h,
              const char *n)
{
	return simple_memmem(h, strlen(h), n, strlen(n));
}

static int
simple_strcasecmp(const char *s1,
                  const char *s2)
{
	int ret;
	for (; (ret = (TOLOWER(*s1) - TOLOWER(*s2))) == 0 && *s1; ++s1, ++s2)
		;
	return ret;
}

static int
simple_strncasecmp(const char *s1,
                   const char *s2,
                   size_t n)
{
	if (n == 0)
		return 0;
	int ret;
	for (; n && (ret = (TOLOWER(*s1) - TOLOWER(*s2))) == 0 && *s1; --n, ++s1, ++s2)
		;
	if (n == 0)
		return 0;
	return ret;
}

static char *
simple_strcasestr_len(const char *h,
                      const size_t hl,
                      const char *n,
                      const size_t nl)
{
	if (nl == 0)
		return (char *)h;
	if (hl < nl)
		return NULL;
	const char *end = h + hl - nl;
	for (; h <= end; ++h)
		if (!simple_strncasecmp(h, n, nl))
			return (char *)h;
	return NULL;
}

static char *
simple_strcasestr(const char *h,
                  const char *n)
{
	return simple_strcasestr_len(h, strlen(h), n, strlen(n));
}

#define T_DEBUG(hs, ne, hs_len, ne_len, n, result, expected)                               \
	do {                                                                               \
		if (jstr_unlikely(result != expected)) {                                   \
			size_t hl = hs_len, nl = ne_len;                                   \
			if (hl == 0)                                                       \
				hl = strlen(hs);                                           \
			if (nl == 0)                                                       \
				nl = strlen(ne);                                           \
			PRINTERR("hsn:\n");                                                \
			PRINTERR("hs:\n%s\n", hs);                                         \
			PRINTERR("ne:\n%s\n", ne);                                         \
			fwrite(hs, 1, jstr_strnlen(hs, n), stderr);                        \
			PRINTERR("\n");                                                    \
			PRINTERR("hs_len:\n%zu\n", hl);                                    \
			PRINTERR("nl:\n%zu\n", nl);                                        \
			PRINTERR("n:\n%zu\n", (size_t)n);                                  \
			PRINTERR("expected:\n%s\n", EMPTY((char *)expected));              \
			PRINTERR("expected_len:\n%zu\n", strlen(EMPTY((char *)expected))); \
			PRINTERR("result:\n%s\n", EMPTY((char *)result));                  \
			PRINTERR("result_len:\n%zu\n", strlen(EMPTY((char *)result)));     \
			assert(result == expected);                                        \
		}                                                                          \
	} while (0)

#define T_HS(test, i) ((test)[i].hs)
#define T_NE(test, i) ((test)[i].ne)
#define T_S1(test, i) ((test)[i].s2)
#define T_S2(test, i) ((test)[i].s2)

#define T(fn, simple_fn, test_array)                                             \
	do {                                                                     \
		TESTING(fn);                                                     \
		T_FOREACHI(test_array, i)                                        \
		{                                                                \
			const char *hs = T_HS(test_array, i);                    \
			const char *ne = T_NE(test_array, i);                    \
			T_DEBUG(hs, ne, 0, 0, 0, fn(hs, ne), simple_fn(hs, ne)); \
		}                                                                \
	} while (0)

#define T_LEN(fn, simple_fn, test_array)                                                                                      \
	do {                                                                                                                  \
		TESTING(fn);                                                                                                  \
		T_FOREACHI(test_array, i)                                                                                     \
		{                                                                                                             \
			const char *hs = T_HS(test_array, i);                                                                 \
			const char *ne = T_NE(test_array, i);                                                                 \
			const size_t hs_len = strlen(hs);                                                                     \
			const size_t ne_len = strlen(ne);                                                                     \
			T_DEBUG(hs, ne, hs_len, ne_len, 0, fn(hs, hs_len, ne, ne_len), simple_fn(hs, hs_len, ne, ne_len));    \
			if (i < hs_len)                                                                                       \
				T_DEBUG(hs, ne, i, ne_len, 0, fn(hs, hs_len, ne, ne_len), simple_fn(hs, hs_len, ne, ne_len)); \
			if (i < ne_len)                                                                                       \
				T_DEBUG(hs, ne, hs_len, i, 0, fn(hs, hs_len, ne, ne_len), simple_fn(hs, hs_len, ne, ne_len)); \
		}                                                                                                             \
	} while (0)

#define T_N(fn, simple_fn, test_array)                                                                   \
	do {                                                                                             \
		TESTING(fn);                                                                             \
		T_FOREACHI(test_array, i)                                                                \
		{                                                                                        \
			size_t n = strlen(test_array[i].hs);                                             \
			const char *hs = T_HS(test_array, i);                                            \
			const char *ne = T_NE(test_array, i);                                            \
			const size_t hs_len = strlen(hs);                                                \
			const size_t ne_len = strlen(ne);                                                \
			T_DEBUG(hs, ne, hs_len, ne_len, n, fn(hs, ne, n), simple_fn(hs, ne, n));         \
			if (i < n)                                                                       \
				T_DEBUG(hs, ne, hs_len, ne_len, i, fn(hs, ne, n), simple_fn(hs, ne, n)); \
			if (i < hs_len)                                                                  \
				T_DEBUG(hs, ne, i, ne_len, n, fn(hs, ne, n), simple_fn(hs, ne, n));      \
			if (i < ne_len)                                                                  \
				T_DEBUG(hs, ne, hs_len, i, n, fn(hs, ne, n), simple_fn(hs, ne, n));      \
		}                                                                                        \
	} while (0)

#define T_CMP_LEN(fn, simple_fn, test_array)                                                                         \
	do {                                                                                                         \
		TESTING(fn);                                                                                         \
		T_FOREACHI(test_array, i)                                                                            \
		{                                                                                                    \
			const char *s1 = T_S1(test_array, i);                                                        \
			const char *s2 = T_S2(test_array, i);                                                        \
			const size_t s1_len = strlen(s1);                                                            \
			const size_t s2_len = strlen(s2);                                                            \
			size_t n = JSTR_MIN(s1_len, JSTR_MIN(i, s2_len));                                            \
			T_DEBUG(s1, s2, s1_len, s2_len, n, s1 + !fn(s1, s2, n), s1 + !simple_fn(s1, s2, n));         \
			if (i < n)                                                                                   \
				T_DEBUG(s1, s2, s1_len, s2_len, i, s1 + !fn(s1, s2, n), s1 + !simple_fn(s1, s2, n)); \
		}                                                                                                    \
	} while (0)

#define T_CMP(fn, simple_fn, test_array)                                                   \
	do {                                                                               \
		TESTING(fn);                                                               \
		T_FOREACHI(test_array, i)                                                  \
		{                                                                          \
			const char *s1 = T_S1(test_array, i);                              \
			const char *s2 = T_S2(test_array, i);                              \
			T_DEBUG(s1, s2, 0, 0, 0, s1 + fn(s1, s2), s1 + simple_fn(s1, s2)); \
		}                                                                          \
	} while (0)

int
main(int argc, char **argv)
{
	START();
	T_CMP_LEN(!jstr_memcmpeq_loop, !memcmp, test_array_memcmp);
	T_CMP_LEN(jstr_strncasecmp, simple_strncasecmp, test_array_memcmp);
	T_CMP_LEN(!jstr_strcasecmpeq_len, !simple_strncasecmp, test_array_memcmp);
	T_CMP_LEN(!jstr_strcasecmpeq_len_loop, !simple_strncasecmp, test_array_memcmp);
	T_CMP(jstr_strcasecmp, simple_strcasecmp, test_array_memcmp);
	T_CMP(!jstr_strcasecmpeq, !simple_strcasecmp, test_array_memcmp);
	T_CMP(!jstr_strcasecmpeq_loop, !simple_strcasecmp, test_array_memcmp);
	T(jstr_strcasestr, simple_strcasestr, test_array_memmem);
	T_LEN(jstr_strcasestr_len, simple_strcasestr_len, test_array_memmem);
	T_LEN(jstr_memmem, simple_memmem, test_array_memmem);
	T_LEN(jstr_strrstr_len, simple_strrstr_len, test_array_memmem);
	T_N(jstr_strnstr, simple_strnstr, test_array_memmem);
	SUCCESS();
	return EXIT_SUCCESS;
	(void)argc;
}