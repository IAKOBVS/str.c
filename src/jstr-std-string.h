#ifndef JSTR_STD_STRING_H
#define JSTR_STD_STRING_H 1

#include "jstr-macros.h"

PJSTR_BEGIN_DECLS
#include <stdlib.h>
#include <string.h>
PJSTR_END_DECLS

#include "jstr-string-fza.h"
#include "jstr-string-fzb.h"
#include "jstr-string-fzc.h"
#include "jstr-string-fzi.h"
#include "jstr-string-misc.h"
#include "jstr-string-opthr.h"
#include "jstr-string-optype.h"
#include "jstr-string-shift.h"

#define R JSTR_RESTRICT

#if JSTR_HAVE_ALLOCA
#	define jstr_strdupa_len(dst_ptr, src, n)             \
		do {                                          \
			const char *const *const p = dst_ptr; \
			*p = (char *)alloca(n + 1);           \
			jstr_strcpy_len(*p, src, n);          \
		} while (0)
#endif

PJSTR_BEGIN_DECLS

JSTR_INLINE
JSTR_FUNC_VOID
static void *
jstr_bzero(void *R s,
	   const size_t n)
JSTR_NOEXCEPT
{
	return memset(s, 0, n);
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_stpset(char *R s,
	    const int c)
JSTR_NOEXCEPT
{
	const size_t n = strlen(s);
	return (char *)memset(s, c, n) + n;
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_strset(char *R s,
	    const int c)
JSTR_NOEXCEPT
{
	const size_t n = strlen(s);
	return (char *)memset(s, c, n);
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_strzero(char *R s)
JSTR_NOEXCEPT
{
	return jstr_strset(s, 0);
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_stpzero(char *R s)
JSTR_NOEXCEPT
{
	return jstr_stpset(s, 0);
}

JSTR_FUNC_PURE
JSTR_INLINE
static size_t
jstr_strnlen(const char *R s,
	     const size_t maxlen)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRNLEN
	return strnlen(s, maxlen);
#else
	const char *const p = (char *)memchr(s, '\0', maxlen);
	return p ? (size_t)(p - s) : maxlen;
#endif
}

JSTR_INLINE
static const char *
jstr_sadd(uintptr_t x, uintptr_t y)
JSTR_NOEXCEPT
{
	return (const char *)(y > UINTPTR_MAX - x ? UINTPTR_MAX : x + y);
}

#if JSTR_HAVE_MEMRCHR
JSTR_INLINE
#endif
JSTR_FUNC_PURE
static void *
jstr_memrchr(const void *R s,
	     const int c,
	     const size_t n)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMRCHR
	return (void *)memrchr(s, c, n);
#else
	/* Taken from glibc memrchr released under the terms of the GNU Lesser General Public License.
	   Copyright (C) 1991-2023 Free Software Foundation, Inc. */
	if (jstr_unlikely(n == 0))
		return NULL;
	const jstr_word_ty *word_ptr = (jstr_word_ty *)JSTR_PTR_ALIGN_DOWN(s, sizeof(jstr_word_ty));
	uintptr_t s_int = (uintptr_t)s;
	jstr_word_ty word = jstr_word_toword(word_ptr);
	jstr_word_ty repeated_c = jstr_word_repeat_bytes(c);
	const char *lbyte = jstr_sadd(s_int, n - 1);
	const jstr_word_ty *lword = (const jstr_word_ty *)JSTR_PTR_ALIGN_DOWN(lbyte, sizeof(jstr_word_ty));
	jstr_word_ty mask = jstr_word_shift_find(jstr_word_find_eq_all(word, repeated_c), s_int);
	if (mask != 0) {
		char *ret = (char *)s + jstr_word_index_first(mask);
		return (ret <= lbyte) ? ret : NULL;
	}
	if (word_ptr == lword)
		return NULL;
	word = jstr_word_toword(++word_ptr);
	while (word_ptr != lword) {
		if (jstr_word_has_eq(word, repeated_c))
			return (char *)word_ptr + jstr_word_index_first_eq(word, repeated_c);
		word = jstr_word_toword(++word_ptr);
	}
	if (jstr_word_has_eq(word, repeated_c)) {
		char *ret = (char *)word_ptr + jstr_word_index_first_eq(word, repeated_c);
		if (ret <= lbyte)
			return ret;
	}
	return NULL;
#endif
}

JSTR_FUNC_PURE
static char *
jstr_strchrnul(const char *R s,
	       const int c)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	return (char *)strchrnul(s, c);
#else
	const char *const start = s;
	return (char *)((s = strchr(s, c)) ? s : start + strlen(start));
#endif
}

JSTR_FUNC_PURE
JSTR_INLINE
static char *
jstr_strnchr(const char *R s,
	     const int c,
	     const size_t n)
JSTR_NOEXCEPT
{
	return (char *)memchr(s, c, jstr_strnlen(s, n));
}

JSTR_FUNC_PURE
JSTR_INLINE
static void *
jstr_memchrnul(const void *R s,
	       const int c,
	       const size_t n)
JSTR_NOEXCEPT
{
	const void *const p = jstr_memrchr(s, c, n);
	return (void *)(p ? p : (char *)s + n);
}

JSTR_FUNC
JSTR_INLINE
static char *
jstr_strstr_len(const char *R hs,
		const size_t hslen,
		const char *R ne,
		const size_t nelen)
JSTR_NOEXCEPT
{
	return (char *)JSTR_MEMMEM(hs, hslen, ne, nelen);
	(void)hslen;
	(void)nelen;
}

/*
  Return value:
  ptr to '\0' in DST.
*/
JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static void *
jstr_mempcpy(void *R dst,
	     const void *R src,
	     const size_t n)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMPCPY
	return mempcpy(dst, src, n);
#else
	return (char *)memcpy(dst, src, n) + n;
#endif
}

JSTR_FUNC
JSTR_INLINE
static void *
jstr_mempmove(void *dst,
	      const void *src,
	      const size_t n)
JSTR_NOEXCEPT
{
	return (char *)memmove(dst, src, n) + n;
}

JSTR_FUNC_VOID
JSTR_INLINE
static char *
jstr_strmove_len(void *dst,
		 const void *src,
		 const size_t n)
JSTR_NOEXCEPT
{
	*(char *)jstr_mempmove(dst, src, n) = '\0';
	return (char *)dst;
}

JSTR_FUNC_VOID
JSTR_INLINE
static char *
jstr_strmove(void *dst,
	     const void *src)
JSTR_NOEXCEPT
{
	*(char *)jstr_mempmove(dst, src, strlen((char *)src)) = '\0';
	return (char *)dst;
}

JSTR_FUNC
JSTR_INLINE
static char *
jstr_stpmove_len(void *dst,
		 const void *src,
		 const size_t n)
JSTR_NOEXCEPT
{
	return jstr_strmove_len(dst, src, n) + n;
}

JSTR_FUNC
JSTR_INLINE
static char *
jstr_stpmove(void *dst,
	     const void *src)
JSTR_NOEXCEPT
{
	return jstr_stpmove_len(dst, src, strlen((char *)src));
}

/*
   Avoid strmove if DST == SRC.
*/
JSTR_FUNC
JSTR_INLINE
static char *
jstr_stpmove_len_may_eq(void *dst,
			const void *src,
			const size_t n)
JSTR_NOEXCEPT
{
	return ((dst != src) ? jstr_stpmove_len(dst, src, n) : (char *)dst) + n;
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_strcpy_len(void *R dst,
		const void *R src,
		const size_t n)
JSTR_NOEXCEPT
{
	*(char *)jstr_mempcpy(dst, src, n) = '\0';
	return (char *)dst;
}

JSTR_INLINE
JSTR_FUNC_VOID
static char *
jstr_stpcpy_len(void *R dst,
		const void *R src,
		const size_t n)
JSTR_NOEXCEPT
{
	return jstr_strcpy_len(dst, src, n) + n;
}

/*
  Return value:
  ptr to '\0' in DST.
*/
JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static char *
jstr_stpcpy(char *R dst,
	    const char *R src)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STPCPY
	return stpcpy(dst, src);
#else
	return jstr_stpcpy_len(dst, src, strlen(src));
#endif /* !JSTR_HAVE_STPCPY */
}

/* Copy until either N is 0 or C is found */
JSTR_FUNC
JSTR_INLINE
static void *
jstr_memccpy(void *R dst,
	     const void *R src,
	     const int c,
	     const size_t n)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMCCPY
	return memccpy(dst, src, c, n);
#else
	const void *const p = (void *)memchr(src, c, n);
	if (p != NULL) {
		return jstr_stpcpy_len(dst, src, (char *)p - (char *)src);
	}
	memcpy(dst, src, n);
	return NULL;
#endif /* HAVE_MEMCPY */
}

/*
  Return value:
  Pointer to '\0' in DST.
*/
JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static char *
jstr_stpcat(char *R dst,
	    const char *R src)
JSTR_NOEXCEPT
{
	dst += strlen(dst);
	return jstr_stpcpy(dst, src);
}

/*
   strlen(S) is saved to SZ.
*/
JSTR_MALLOC
JSTR_FUNC
JSTR_INLINE
static char *
jstr_strdup_p(const char *R s,
	      size_t *R n)
JSTR_NOEXCEPT
{
	*n = strlen(s);
	char *const p = (char *)malloc(*n + 1);
	if (jstr_likely(p != NULL)) {
		jstr_strcpy_len(p, s, *n);
		return p;
	}
	return NULL;
}

JSTR_MALLOC
JSTR_FUNC
JSTR_INLINE
static char *
jstr_memdup(const char *R s,
	    const size_t n)
JSTR_NOEXCEPT
{
	char *const p = (char *)malloc(n);
	if (jstr_likely(p != NULL)) {
		memcpy(p, s, n);
		return p;
	}
	return NULL;
}

JSTR_MALLOC
JSTR_FUNC
JSTR_INLINE
static char *
jstr_strdup_len(const char *R s,
		const size_t n)
JSTR_NOEXCEPT
{
	char *const p = (char *)malloc(n + 1);
	if (jstr_likely(p != NULL))
		return jstr_strcpy_len(p, s, n);
	return NULL;
}

JSTR_MALLOC
JSTR_FUNC
JSTR_INLINE
static char *
jstr_strdup(const char *R s)
JSTR_NOEXCEPT
{
	return jstr_strdup_len(s, strlen(s));
}

JSTR_FUNC_PURE
JSTR_INLINE
static char *
jstr_strstrnul_len(const char *R hs,
		   const size_t hslen,
		   const char *R ne,
		   const size_t nelen)
JSTR_NOEXCEPT
{
	const char *const p = jstr_strstr_len(hs, hslen, ne, nelen);
	return (char *)(p ? p : hs + hslen);
	(void)nelen;
}

JSTR_FUNC_PURE
JSTR_INLINE
static char *
jstr_strstrnul(const char *R hs,
	       const char *R ne)
JSTR_NOEXCEPT
{
	const char *const p = strstr(hs, ne);
	return (char *)(p ? p : hs + strlen(hs));
}

JSTR_FUNC_PURE
JSTR_INLINE
static char *
jstr_strpbrk(const char *R s,
	     const char *R accept)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRPBRK_OPTIMIZED
	return (char *)strpbrk(s, accept);
#else
	s += strcspn(s, accept);
	return *s ? (char *)s : NULL;
#endif
}

/*
   Non-destructive strtok.
   END must be NUL terminated.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok_ne_len(const char **const save_ptr,
		   const char *const end,
		   const char *R ne,
		   const size_t nelen)
JSTR_NOEXCEPT
{
	const char *s = *save_ptr;
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	if (!strncmp(s, ne, nelen))
		s += nelen;
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	*save_ptr = jstr_strstrnul_len(s, end - s, ne, nelen);
	return (char *)s;
}

/*
   Non-destructive strtok.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok_ne(const char **const save_ptr,
	       const char *R ne)
JSTR_NOEXCEPT
{
	const char *s = *save_ptr;
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	const size_t nelen = strlen(ne);
	if (!strncmp(s, ne, nelen))
		s += nelen;
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	*save_ptr = jstr_strstrnul(s, ne);
	return (char *)s;
}

/*
   Non-destructive strtok.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok(const char *R *R save_ptr,
	    const char *R delim)
JSTR_NOEXCEPT
{
	const char *s = *save_ptr;
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	s += strspn(s, delim);
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	*save_ptr = s + strcspn(s, delim);
	return (char *)s;
}

JSTR_FUNC_PURE
JSTR_INLINE
static int
jstr_atoi(const char *R s)
JSTR_NOEXCEPT
{
	return strtol(s, NULL, 0);
}

JSTR_FUNC_PURE
JSTR_INLINE
static long
jstr_atol(const char *R s)
JSTR_NOEXCEPT
{
	return strtol(s, NULL, 0);
}

JSTR_FUNC_PURE
JSTR_INLINE
static long long
jstr_atoll(const char *R s)
JSTR_NOEXCEPT
{
	return strtoll(s, NULL, 0);
}

JSTR_FUNC_PURE
JSTR_INLINE
static double
jstr_atod(const char *R s)
JSTR_NOEXCEPT
{
	return strtod(s, NULL);
}

JSTR_FUNC_PURE
JSTR_INLINE
static float
jstr_atof(const char *R s)
JSTR_NOEXCEPT
{
	return strtof(s, NULL);
}

PJSTR_END_DECLS

#undef R

#endif /* JSTR_STD_STRING_H */
