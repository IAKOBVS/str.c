#ifndef JSTR_H_DEF
#define JSTR_H_DEF

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
}
#endif /* __cpluslus */

#ifndef __cplusplus
#	include "_jstr_pp_va_args_macros.h"
#endif /* __cplusplus */

#include "_jstr_builder.h"
#include "_jstr_config.h"
#include "_jstr_ctype.h"
#include "_jstr_macros.h"
#include "_jstr_regex.h"
#include "_jstr_replace.h"
#include "_jstr_string.h"

#ifdef __cplusplus
namespace jstr {
#endif /* __cpluslus */

/*
   Append SRC to DST.
   Use non-f version for bounds checking.
   Return value:
   New len of S.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_INLINE
JSTR_RETURNS_NONNULL
static char *jstr_append_mem_p_f(char *JSTR_RST const s,
				 const char *JSTR_RST const src,
				 const size_t sz,
				 const size_t srclen) JSTR_NOEXCEPT
{
	memcpy(s, src, srclen + 1);
	return s + sz + srclen;
}

/*
   Append SRC to DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_append_mem(char **JSTR_RST const s,
			    size_t *JSTR_RST const sz,
			    size_t *JSTR_RST const cap,
			    const char *JSTR_RST const src,
			    const size_t srclen) JSTR_NOEXCEPT
{
	if (*cap < *sz + srclen)
		JSTR_REALLOC(*s, *cap, *sz + srclen, return);
	*sz = jstr_append_mem_p_f(*s, src, *sz, srclen) - *s;
}

/*
  Count occurences of C in S.
  Return value:
  Occurences of C in S.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_countc(const char *JSTR_RST s,
		       const int c) JSTR_NOEXCEPT
{
	int count = 0;
	for (;; ++s)
		if (*s == c)
			++count;
		else if (unlikely(!*s))
			break;
	return count;
}

#ifdef JSTR_HAS_MEMMEM

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_count_mem(const char *JSTR_RST s,
			  const char *JSTR_RST const searc,
			  size_t sz,
			  const size_t searclen) JSTR_NOEXCEPT
{
	int count = 0;
	while ((s = (char *)memmem(s, sz, searc, searclen))) {
		++count;
		s += searclen;
		sz += searclen;
	}
	return count;
}

#else

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_count(const char *JSTR_RST s,
		      const char *JSTR_RST const searc) JSTR_NOEXCEPT
{
	int count = 0;
	while ((s = strstr(s, searc)))
		++count;
	return count;
}

#endif /* __JSTR_HAS_MEMMEM */

/*
  Reverse S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rev_mem(char *JSTR_RST s,
			 const size_t sz) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return;
	unsigned char *end = (unsigned char *)s + sz - 1;
	unsigned char *p = (unsigned char *)s;
	unsigned char tmp;
	do {
		tmp = *p;
		*p = *end;
		*end = tmp;
	} while (++p < --end);
}

/*
  Trim spaces in [ \t] from end of S.
  Return value:
  pointer to '\0' in S;
  S if SLEN is 0.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_trim_mem_p(char *JSTR_RST const s,
			     const size_t sz) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return s;
	unsigned char *end = (unsigned char *)s + sz - 1;
	unsigned char *start = (unsigned char *)s;
	do {
		switch (*end--) {
		case '\t':
		case ' ':
			continue;
		default:
			end += 2;
			*end = '\0';
		}
		break;
	} while (end >= start);
	return (char *)end;
}

/*
  Insert SRC into DST[AT].
  Assumes that S have enough space for SRC.
  Return value:
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insert_mem_f(char *JSTR_RST const s,
			      const size_t at,
			      const char *JSTR_RST const src,
			      const size_t srclen) JSTR_NOEXCEPT
{
	memcpy(s + at, src, srclen);
}

/*
  Insert SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insert_mem(char **JSTR_RST const s,
			    size_t *JSTR_RST const sz,
			    size_t *JSTR_RST const cap,
			    const size_t at,
			    const char *JSTR_RST const src,
			    const size_t srclen) JSTR_NOEXCEPT
{
	if (at + srclen > *sz) {
		JSTR_REALLOC(*s, *cap, at + srclen + 1, return);
		*sz = at + srclen;
		*(*s + *sz) = '\0';
	}
	jstr_insert_mem_f(*s, at, src, srclen);
}

/*
  Insert SRC after C in DST.
  Assumes that S have enough space for S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftc_mem_f(char *JSTR_RST const s,
				  const int c,
				  const char *JSTR_RST const src,
				  const size_t sz,
				  const size_t srclen) JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(s, c, sz);
	if (p)
		jstr_insert_mem_f(s, p - s + 1, src, srclen);
}

/*
  Insert SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftc_mem(char **JSTR_RST const s,
				size_t *JSTR_RST const sz,
				size_t *JSTR_RST const cap,
				const int c,
				const char *JSTR_RST const src,
				const size_t srclen) JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p)
		jstr_insert_mem(s, sz, cap, p - *s + 1, src, srclen);
}

/*
  Insert SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftallc_mem(char **JSTR_RST const s,
				   size_t *JSTR_RST const sz,
				   size_t *JSTR_RST const cap,
				   const int c,
				   const char *JSTR_RST const src,
				   const size_t srclen) JSTR_NOEXCEPT
{
	size_t off = 0;
	const char *p;
	while ((p = (char *)memchr(*s + off, c, *sz - off))) {
		jstr_insert_mem(s, sz, cap, p - *s + 1, src, srclen);
		off += *s - p + 1;
	}
}

/*
  Insert SRC after end of NE in DST.
  Assumes that S have enough space for SRC.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaft_mem_f(char *JSTR_RST const s,
				 const char *JSTR_RST const searc,
				 const char *JSTR_RST const src,
				 const size_t sz,
				 const size_t searclen,
				 const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0: return;
	case 1:
		jstr_insertaftc_mem_f(s, *searc, src, sz, srclen);
		return;
	default: {
		const char *const p = (char *)PRIVATE_JSTR_MEMMEM(s, sz, searc, searclen);
		if (p)
			jstr_insert_mem_f(s, p - s + searclen, src, srclen);
		return;
	}
	}
}

/*
  Insert SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaft_mem(char **JSTR_RST const s,
			       size_t *JSTR_RST const sz,
			       size_t *JSTR_RST const cap,
			       const char *JSTR_RST const searc,
			       const char *JSTR_RST const src,
			       const size_t searclen,
			       const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0: return;
	case 1:
		jstr_insertaftc_mem(s, sz, cap, *searc, src, srclen);
		return;
	default: {
		const char *const p = (char *)PRIVATE_JSTR_MEMMEM(*s, *sz, searc, searclen);
		if (p)
			jstr_insert_mem(s, sz, cap, p - *s + searclen, src, srclen);
		return;
	}
	}
}

/*
  Insert SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftall_mem(char **JSTR_RST const s,
				  size_t *JSTR_RST const sz,
				  size_t *JSTR_RST const cap,
				  const char *JSTR_RST const searc,
				  const char *JSTR_RST const src,
				  const size_t searclen,
				  const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0: return;
	case 1:
		jstr_insertaftallc_mem(s, sz, cap, *searc, src, srclen);
		return;
	default: {
		size_t off = 0;
		const char *p;
		while ((p = (char *)PRIVATE_JSTR_MEMMEM(*s + off, *sz - off, searc, searclen))) {
			jstr_insert_mem(s, sz, cap, p - *s + searclen, src, srclen);
			off += *s - p + searclen;
		}
		return;
	}
	}
}

/*
  Slip SRC into DST[AT].
  Return value:
  pointer to '\0' in S.
  Assumes that S have enough space for SRC.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_slip_mem_p_f(char *JSTR_RST const s,
			       const size_t at,
			       const char *JSTR_RST const src,
			       const size_t sz,
			       const size_t srclen) JSTR_NOEXCEPT
{
	memmove(s + at + srclen,
		s + at,
		sz - at + 1);
	memcpy(s + at, src, srclen);
	return s + sz + srclen;
}

/*
  Slip SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slip_mem(char **JSTR_RST const s,
			  size_t *JSTR_RST const sz,
			  size_t *JSTR_RST const cap,
			  const size_t at,
			  const char *JSTR_RST const src,
			  const size_t srclen) JSTR_NOEXCEPT
{
	if (*cap > *sz + srclen + 1) {
		memmove(*s + at + srclen,
			*s + at,
			*sz - at + 1);
		memcpy(*s + at, src, srclen);
	} else {
		JSTR_GROW(*cap, *sz + srclen + 1);
		char *const tmp = (char *)malloc(*cap);
		JSTR_MALLOC_ERR(tmp, return);
		memcpy(tmp, *s, at);
		memcpy(tmp + at, src, srclen);
		memcpy(tmp + at + srclen,
		       *s + at,
		       *sz - at + 1);
		free(*s);
		*s = tmp;
	}
	*sz += srclen;
}

/*
  Slip SRC after C in DST.
  Return value:
  pointer to '\0' in S.
  Assumes that S have enough space for SRC.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_slipaftc_mem_p_f(char *JSTR_RST const s,
				   const int c,
				   const char *JSTR_RST const src,
				   const size_t sz,
				   const size_t srclen) JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(s, c, sz);
	if (p)
		return jstr_slip_mem_p_f(s, p - s, src, sz - (p - s), srclen);
	return s + sz;
}

/*
  Slip SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftc_mem(char **JSTR_RST const s,
			      size_t *JSTR_RST const sz,
			      size_t *JSTR_RST const cap,
			      const int c,
			      const char *JSTR_RST const src,
			      const size_t srclen) JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p)
		jstr_slip_mem(s, sz, cap, p - *s + 1, src, srclen);
}

/*
  Slip SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_RETURNS_NONNULL
static char *jstr_slipaftallc_mem_p_f(char *JSTR_RST const s,
				      const int c,
				      const char *JSTR_RST const src,
				      size_t sz,
				      const size_t srclen) JSTR_NOEXCEPT
{
	size_t off = 0;
	const char *p;
	while ((p = (char *)memchr(s + off, c, sz - off))) {
		sz = jstr_slip_mem_p_f(s, p - s, src, sz, srclen) - s;
		off += p - s + 1;
	}
	return s + sz;
}

/*
  Slip SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftallc_mem(char **JSTR_RST const s,
				 size_t *JSTR_RST const sz,
				 size_t *JSTR_RST const cap,
				 const int c,
				 const char *JSTR_RST const src,
				 const size_t srclen) JSTR_NOEXCEPT
{
	size_t off = 0;
	const char *p;
	while ((p = (char *)memchr(*s + off, c, *sz - off))) {
		jstr_slip_mem(s, sz, cap, p - *s + 1, src, srclen);
		off += p - *s + 1;
	}
}

/*
  Slip SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_slipaft_mem_f(char *JSTR_RST const s,
				const char *JSTR_RST const searc,
				const char *JSTR_RST const src,
				const size_t sz,
				const size_t searclen,
				const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0: return s + sz;
	case 1: return jstr_slipaftc_mem_p_f(s, *searc, src, sz, srclen);
	default: {
		const char *const p = (char *)PRIVATE_JSTR_MEMMEM(s, sz, searc, searclen);
		if (p)
			return jstr_slip_mem_p_f(s, p - s, src, sz, srclen);
		return s + sz;
	}
	}
}

/*
  Slip SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaft_mem(char **JSTR_RST const s,
			     size_t *JSTR_RST const sz,
			     size_t *JSTR_RST const cap,
			     const char *JSTR_RST const searc,
			     const char *JSTR_RST const src,
			     const size_t searclen,
			     const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0: return;
	case 1:
		jstr_slipaftc_mem(s, sz, cap, *searc, src, srclen);
		return;
	default: {
		const char *const p = (char *)PRIVATE_JSTR_MEMMEM(*s, *sz, searc, searclen);
		if (p)
			jstr_slip_mem(s, sz, cap, p - *s + searclen, src, srclen);
		return;
	}
	}
}

/*
  Slip SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_RETURNS_NONNULL
static char *jstr_slipaftall_mem_p_f(char *JSTR_RST const s,
				     const char *JSTR_RST const searc,
				     const char *JSTR_RST const src,
				     size_t sz,
				     const size_t searclen,
				     const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0:
		return s + sz;
	case 1:
		return jstr_slipaftallc_mem_p_f(s, *searc, src, sz, srclen);
	default: {
		size_t off = 0;
		const char *p;
		while ((p = (char *)PRIVATE_JSTR_MEMMEM(s + off, sz - off, searc, searclen))) {
			sz = jstr_slip_mem_p_f(s, p - s, src, sz, srclen) - s;
			off += p - s + searclen;
		}
		return s + sz;
	}
	}
}

/*
  Slip SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftall_mem(char **JSTR_RST const s,
				size_t *JSTR_RST const sz,
				size_t *JSTR_RST const cap,
				const char *JSTR_RST const searc,
				const char *JSTR_RST const src,
				const size_t searclen,
				const size_t srclen) JSTR_NOEXCEPT
{
	switch (searclen) {
	case 0:
		return;
	case 1:
		jstr_slipaftallc_mem(s, sz, cap, *searc, src, srclen);
		return;
	default: {
		size_t off = 0;
		const char *p;
		while ((p = (char *)PRIVATE_JSTR_MEMMEM(*s + off, *sz - off, searc, searclen))) {
			jstr_slip_mem(s, sz, cap, p - *s + searclen, src, srclen);
			off += p - *s + searclen;
		}
	}
	}
}

/*
  Checks if S2 is in end of S1.
  Return value:
  0 if true;
  1 if false.
  Assumes that HS is longer than NE.
  Let memcmp do the bounds check.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_endswith_mem_f(const char *JSTR_RST const hs,
			       const char *JSTR_RST const ne,
			       const size_t hsz,
			       const size_t nelen) JSTR_NOEXCEPT
{
	return memcmp(hs + hsz - nelen, ne, nelen);
}

/*
  Checks if S2 is in end of S1.
  Return value:
  0 if true;
  1 if false.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_endswith_mem(const char *JSTR_RST const hs,
			     const char *JSTR_RST const ne,
			     const size_t hsz,
			     const size_t nelen) JSTR_NOEXCEPT
{
	return (hsz < nelen) ? 1 : memcmp(hs + hsz - nelen, ne, nelen);
}

/*
  Converts int to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_itoa(char *JSTR_RST dst, int num, unsigned char base)
{
#define PRIVATE_JSTR_NUMTOSTR(max_digits)                                       \
	do {                                                                    \
		unsigned char *d = (unsigned char *)dst;                        \
		unsigned char sbuf[max_digits];                                 \
		unsigned char *JSTR_RST s = (unsigned char *)sbuf;              \
		unsigned char neg = (num < 0) ? (num = -num, 1) : 0;            \
		unsigned char *const end = (unsigned char *)s + max_digits - 1; \
		s = end;                                                        \
		do                                                              \
			*s-- = num % base + '0';                                \
		while (num /= 10);                                              \
		if (neg)                                                        \
			*s = '-';                                               \
		else                                                            \
			++s;                                                    \
		while (s <= end)                                                \
			*d++ = *s++;                                            \
		*d = '\0';                                                      \
		return (char *)d;                                               \
	} while (0)
	PRIVATE_JSTR_NUMTOSTR(JSTR_MAX_INT_DIGITS);
}

/*
  Converts long to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_ltoa(char *JSTR_RST dst, long num, unsigned char base)
{
	PRIVATE_JSTR_NUMTOSTR(JSTR_MAX_LONG_DIGITS);
}

/*
  Converts long long to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_lltoa(char *JSTR_RST dst, long long num, unsigned char base)
{
	PRIVATE_JSTR_NUMTOSTR(JSTR_MAX_LONG_DIGITS);
}

/*
  Converts unsigned int to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_utoa(char *JSTR_RST dst, unsigned int num, unsigned char base)
{
#define PRIVATE_JSTR_UNUMTOSTR(max_digits)                     \
	do {                                                   \
		unsigned char *d = (unsigned char *)dst;       \
		unsigned char sbuf[max_digits];                \
		unsigned char *JSTR_RST s = sbuf;              \
		unsigned char *const end = s + max_digits - 1; \
		s = end;                                       \
		do                                             \
			*s-- = num % base + '0';               \
		while (num /= 10);                             \
		++s;                                           \
		while (s <= end)                               \
			*d++ = *s++;                           \
		*d = '\0';                                     \
		return (char *)d;                              \
	} while (0)
	PRIVATE_JSTR_UNUMTOSTR(JSTR_MAX_UINT_DIGITS);
}

/*
  Converts unsigned long to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_ultoa(char *JSTR_RST dst, unsigned long num, unsigned char base)
{
	PRIVATE_JSTR_UNUMTOSTR(JSTR_MAX_ULONG_DIGITS);
}

/*
  Converts unsigned long long to string.
  Return value:
  new pointer to '\0' in dst.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_ulltoa(char *JSTR_RST dst, unsigned long long num, unsigned char base)
{
	PRIVATE_JSTR_UNUMTOSTR(JSTR_MAX_ULONG_LONG_DIGITS);
}

#ifdef __cplusplus
} /* namespace jstr */
#endif /* __cpluslus */

#undef JSTR_EXIT_ON_MALLOC_ERROR
#undef JSTR_PRINT_ERR_MSG_ON_MALLOC_ERROR
#undef JSTR_MAX_INT_DIGITS
#undef JSTR_MAX_LONG_DIGITS
#undef JSTR_MAX_LONG_LONG_DIGITS
#undef JSTR_MAX_UINT_DIGITS
#undef JSTR_MAX_ULONG_DIGITS
#undef JSTR_MAX_ULONG_LONG_DIGITS
#undef PRIVATE_JSTR_NUMTOSTR
#undef PRIVATE_JSTR_UNUMTOSTR
#undef JSTR_RST
#undef JSTR_REPLACE
#undef JSTR_PRIVATE
#undef PRIVATE_JSTR_MEMMEM5_SHIFTS
#undef JSTR_HASH2
#undef DEFINE_JSTR_MEMMEMN_FUNCS

#endif /* JSTR_H_DEF */
