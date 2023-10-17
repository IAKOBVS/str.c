#ifndef JSTR_H_REPLACE_DEF
#define JSTR_H_REPLACE_DEF 1

#include "jstr-macros.h"

PJSTR_BEGIN_DECLS
#include <stdlib.h>
#include <string.h>
PJSTR_END_DECLS

#include "jstr-builder.h"
#include "jstr-ctype.h"
#include "jstr-macros.h"
#include "jstr-string.h"

#define R JSTR_RESTRICT

PJSTR_BEGIN_DECLS

#define PJSTR_RMALL_IN_PLACE(dst, oldp, p, findlen)   \
	do {                                          \
		if (jstr_likely(dst != oldp))         \
			memmove(dst, oldp, p - oldp); \
		dst += p - oldp;                      \
		oldp += p - oldp;                     \
		oldp += findlen;                      \
		p += findlen;                         \
	} while (0)
#define PJSTR_RPLCALL_IN_PLACE(dst, oldp, p, rplc, rplclen, findlen) \
	do {                                                         \
		if (jstr_likely(findlen != rplclen && dst != oldp))  \
			memmove(dst, oldp, p - oldp);                \
		dst += p - oldp;                                     \
		oldp += p - oldp;                                    \
		oldp += findlen;                                     \
		p += findlen;                                        \
		memcpy(dst, rplc, rplclen);                          \
		dst += rplclen;                                      \
	} while (0)

/*
  Slip SRC into DST[AT].
  Return value:
  ptr to '\0' in S.
  Assume that S have enough space for SRC.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_slip_unsafe(char *R s,
		 const size_t at,
		 const char *R src,
		 const size_t sz,
		 const size_t srclen)
JSTR_NOEXCEPT
{
	memmove(s + at + srclen,
		s + at,
		sz - at + 1);
	memcpy(s + at, src, srclen);
}

JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_replaceat_len_unsafe(char *R s,
			  size_t *R sz,
			  const size_t at,
			  const char *R rplc,
			  const size_t rplclen,
			  const size_t findlen)
JSTR_NOEXCEPT
{
	memmove(s + at + rplclen,
		s + at + findlen,
		*sz - (at + findlen) + 1);
	memcpy(s + at, rplc, rplclen);
	*sz += rplclen - findlen;
	return s + at + rplclen;
}

/*
  Slip SRC into DST[AT].
  Return 0 on malloc error;
  otherwise 1.
*/
JSTR_FUNC
static int
jstr_slip_len(char *R *R s,
	      size_t *R sz,
	      size_t *R cap,
	      const size_t at,
	      const char *R rplc,
	      const size_t rplclen)
JSTR_NOEXCEPT
{
	JSTR_RESERVE(s, sz, cap, *sz + rplclen, return 0);
	jstr_slip_unsafe(*s, at, rplc, *sz, rplclen);
	*sz += rplclen;
	return 1;
}

JSTR_INLINE
JSTR_FUNC
static char *
pjstr_replaceat_len_higher(char *R *R s,
			   size_t *R sz,
			   size_t *R cap,
			   const size_t at,
			   const char *R rplc,
			   const size_t rplclen,
			   const size_t findlen)
JSTR_NOEXCEPT
{
	JSTR_RESERVE(s, sz, cap, *sz + rplclen - findlen, return 0);
	return jstr_replaceat_len_unsafe(*s, sz, at, rplc, rplclen, findlen);
}

/*
   Replace RPLC in S with FIND.
   Return value:
   ptr to RPLC in S + RPLCLEN;
   NULL on error.
*/
JSTR_FUNC
static char *
jstr_replaceat_len(char *R *R s,
		   size_t *R sz,
		   size_t *R cap,
		   const size_t at,
		   const char *R rplc,
		   const size_t rplclen,
		   const size_t findlen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(rplclen == findlen)) {
		memcpy(*s + at, rplc, rplclen);
		*sz += rplclen - findlen;
		return *s + at + rplclen;
	}
	return pjstr_replaceat_len_higher(s, sz, cap, at, rplc, rplclen, findlen);
}

/*
  Slip SRC after C in DST.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_INLINE
JSTR_FUNC
static int
jstr_slipafterchr_len(char *R *R s,
		      size_t *R sz,
		      size_t *R cap,
		      const int c,
		      const char *R src,
		      const size_t srclen)
JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p != NULL)
		return jstr_slip_len(s, sz, cap, p - *s + 1, src, srclen);
	return 1;
}

/*
  Slip SRC after all C in DST.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_slipafterallchr_len(char *R *R s,
			 size_t *R sz,
			 size_t *R cap,
			 const int c,
			 const char *R src,
			 const size_t srclen)
JSTR_NOEXCEPT
{
	size_t off = 0;
	const char *p;
	while ((p = (char *)memchr(*s + off, c, *sz - off))) {
		off = p - *s;
		if (jstr_unlikely(!jstr_slip_len(s, sz, cap, off, src, srclen)))
			return 0;
		off += srclen + 1;
	}
	return 1;
}

/*
  Slip SRC after end of NE in DST.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_slipafter_len(char *R *R s,
		   size_t *R sz,
		   size_t *R cap,
		   const char *R find,
		   const char *R src,
		   const size_t findlen,
		   const size_t srclen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 1))
		return jstr_slipafterchr_len(s, sz, cap, *find, src, srclen);
	if (jstr_unlikely(findlen == 0))
		return 1;
	const char *const p = jstr_strstr_len(*s, *sz, find, findlen);
	if (p != NULL)
		return jstr_slip_len(s, sz, cap, p - *s + findlen, src, srclen);
	return 1;
}

/*
  Slip SRC after all end of NE in DST.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_slipafterall_len(char *R *R s,
		      size_t *R sz,
		      size_t *R cap,
		      const char *R find,
		      const char *R src,
		      const size_t findlen,
		      const size_t srclen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 1))
		return jstr_slipafterallchr_len(s, sz, cap, *find, src, srclen);
	if (jstr_unlikely(findlen == 0))
		return 1;
	size_t off = 0;
	const char *p;
	while ((p = jstr_strstr_len(*s + off, *sz - off, find, findlen))) {
		off = p - *s;
		if (jstr_unlikely(!jstr_slip_len(s, sz, cap, p - *s + findlen, src, srclen)))
			return 0;
		off += findlen + srclen;
	}
	return 1;
}

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_removechr_len_p(char *R s,
		     const int c,
		     const size_t sz)
JSTR_NOEXCEPT
{
	const char *const start = s;
	s = (char *)memchr(s, c, sz);
	return s ? jstr_stpmove_len(s, s + 1, sz - (s - start)) : s + sz;
}

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_removechr_p(char *R s,
		 const int c)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	s = strchrnul(s, c);
	return *s ? jstr_stpmove_len(s, s + 1, strlen(s)) : s;
#else
	return jstr_removechr_len_p(s, c, strlen(s));
#endif /* HAVE_STRCHRNUL */
}

/*
  Replace all REJECT in S with RPLC.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_replacespn_p(char *R s,
		  const char *R reject,
		  const int rplc)
JSTR_NOEXCEPT
{
	const char *p;
	while (*s && (*(s += strcspn(s, reject)))) {
		for (p = s + strspn(s, reject); s < p; *s++ = rplc)
			;
	}
	return s;
}

/*
  Replace all REJECT in S with RPLC.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_VOID
static void
jstr_replacespn_j(jstr_ty *R j,
		  const char *R reject,
		  const int rplc)
JSTR_NOEXCEPT
{
	j->size = jstr_replacespn_p(j->data, reject, rplc) - j->data;
}

/*
  Remove all REJECT in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removespn_p(char *R s,
		 const char *R reject)
JSTR_NOEXCEPT
{
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	size_t findlen;
	while (*p && (*(p += strcspn(p, reject)))) {
		findlen = strspn(p, reject);
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, findlen);
	}
	return jstr_stpmove_len_may_eq(dst, oldp, p - oldp);
}

/*
  Remove REJECT in S.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_removespn_j(jstr_ty *R j,
		 const char *R reject)
JSTR_NOEXCEPT
{
	j->size = jstr_removespn_p(j->data, reject) - j->data;
}

typedef enum {
	PJSTR_FLAG_USE_N = (1),
	PJSTR_FLAG_USE_NOT_N = (PJSTR_FLAG_USE_N << 1)
} pjstr_flag_use_n_ty;

JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static char *
pjstr_removeallchr_len_p(const pjstr_flag_use_n_ty flag,
			 char *R s,
			 const int c,
			 size_t n,
			 const size_t sz)
JSTR_NOEXCEPT
{
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	const char *const end = dst + sz;
	while ((flag & PJSTR_FLAG_USE_N ? n-- : 1)
	       && (p = (char *)memchr(p, c, end - p)))
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, 1);
	return (dst != s) ? jstr_stpmove_len(dst, oldp, end - oldp) : s + sz;
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removeallchr_len_p(char *R s,
			const int c,
			const size_t sz)
JSTR_NOEXCEPT
{
	return pjstr_removeallchr_len_p(PJSTR_FLAG_USE_NOT_N, s, c, 0, sz);
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removeallchr_p(char *R s,
		    const int c)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	while (*(p = strchrnul(p, c)))
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, 1);
	return (dst != s) ? jstr_stpmove_len(dst, oldp, p - oldp) : (char *)p;
#else
	return jstr_removeallchr_len_p(s, c, strlen(s));
#endif
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removenchr_len_p(char *R s,
		      const int c,
		      const size_t n,
		      const size_t sz)
JSTR_NOEXCEPT
{
	return pjstr_removeallchr_len_p(PJSTR_FLAG_USE_N, s, c, n, sz);
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_removenchr_p(char *R s,
		  const int c,
		  size_t n)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	while (n-- && *(p = strchrnul(p, c)))
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, 1);
	if (jstr_unlikely(dst == s))
		return s + n;
	return jstr_stpmove_len(dst, oldp, p - oldp);
#else
	return jstr_removenchr_len_p(s, c, n, strlen(s));
#endif /* HAVE_STRCHRNUL */
}

/*
  Remove characters in REJECT in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_stripspn_p(char *R s,
		const char *R rjct)
JSTR_NOEXCEPT
{
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	while (*(p += strcspn(p, rjct)))
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, 1);
	return jstr_stpmove_len_may_eq(dst, oldp, p - oldp);
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_remove_len_p(char *R s,
		  const char *R find,
		  const size_t sz,
		  const size_t findlen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 0))
		return s + sz;
	char *const p = jstr_strstr_len(s, sz, find, findlen);
	if (jstr_unlikely(p == NULL))
		return s + sz;
	memmove(p, p + findlen, (s + sz) - p);
	return s + sz - findlen;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_replacechr_len(char *R s,
		    const int find,
		    const int rplc,
		    const size_t sz)
JSTR_NOEXCEPT
{
	s = (char *)memchr(s, find, sz);
	if (jstr_likely(s != NULL))
		*s = rplc;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_replacechr(char *R s,
		const int find,
		const int rplc)
JSTR_NOEXCEPT
{
	s = strchr(s, find);
	if (jstr_likely(s != NULL))
		*s = rplc;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_replaceallchr_len(char *R s,
		       const int find,
		       const int rplc,
		       const size_t sz)
JSTR_NOEXCEPT
{
	const char *R end = s + sz;
	while ((s = (char *)memchr(s, find, end - s)))
		*s++ = rplc;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_replaceallchr(char *R s,
		   const int find,
		   const int rplc)
JSTR_NOEXCEPT
{
	while ((s = (strchr(s, find))))
		*s++ = rplc;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_replacenchr_len(char *R s,
		     const int find,
		     const int rplc,
		     size_t n,
		     const size_t sz)
JSTR_NOEXCEPT
{
	const char *R end = s + sz;
	while (n-- && (s = (char *)memchr(s, find, end - s)))
		*s++ = rplc;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_replacenchr(char *R s,
		 const int find,
		 const int rplc,
		 size_t n)
JSTR_NOEXCEPT
{
	while (n-- && (s = strchr(s, find)))
		*s++ = rplc;
}

JSTR_FUNC
static int
pjstr_replace_len(char *R *R s,
		  size_t *R sz,
		  size_t *R cap,
		  const size_t start_idx,
		  const char *R find,
		  const char *R rplc,
		  const size_t findlen,
		  const size_t rplclen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(rplclen == 0)) {
		*sz = jstr_remove_len_p(*s + start_idx, find, *sz - start_idx, findlen) - *s;
		return 1;
	}
	if (jstr_unlikely(rplclen == 1)) {
		if (jstr_unlikely(findlen == 1)) {
			jstr_replacechr_len(*s + start_idx, *find, *rplc, *sz - start_idx);
			return 1;
		}
	}
	if (jstr_unlikely(findlen == 0))
		return 1;
	char *p = jstr_strstr_len(*s + start_idx, *sz - start_idx, find, findlen);
	if (jstr_unlikely(p == NULL))
		return 1;
	return jstr_slip_len(s, sz, cap, p - *s, rplc, rplclen);
}

/*
  Replace first SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replace_len(char *R *R s,
		 size_t *R sz,
		 size_t *R cap,
		 const char *R find,
		 const char *R rplc,
		 const size_t findlen,
		 const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replace_len(s, sz, cap, 0, find, rplc, findlen, rplclen);
}

/*
  Replace first SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replace_len_from(char *R *R s,
		      size_t *R sz,
		      size_t *R cap,
		      const size_t start_idx,
		      const char *R find,
		      const char *R rplc,
		      const size_t findlen,
		      const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replace_len(s, sz, cap, start_idx, find, rplc, findlen, rplclen);
}

/*
  Replace last SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replacelast_len(char *R *R s,
		     size_t *R sz,
		     size_t *R cap,
		     const char *R find,
		     const char *R rplc,
		     const size_t findlen,
		     const size_t rplclen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 0))
		return 1;
	char *p = (char *)jstr_strrstr_len(*s, *sz, find, findlen);
	if (jstr_unlikely(p == NULL))
		return 1;
	return jstr_replaceat_len(s, sz, cap, p - *s, rplc, rplclen, findlen) ? 1 : 0;
}

JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static char *
pjstr_removeall_len_p(const pjstr_flag_use_n_ty flag,
		      char *R s,
		      const char *R find,
		      size_t n,
		      size_t sz,
		      const size_t findlen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 1))
		return pjstr_removeallchr_len_p(flag, s, *find, n, sz);
	if (jstr_unlikely(findlen == 0))
		return s + sz;
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	const char *const end = dst + sz;
	while (((flag & PJSTR_FLAG_USE_N) ? n-- : 1)
	       && (p = (char *)JSTR_MEMMEM(p, end - p, find, findlen)))
		PJSTR_RMALL_IN_PLACE(dst, oldp, p, findlen);
	if (jstr_unlikely(dst == s))
		return s + sz;
	return jstr_stpmove_len(dst, oldp, end - oldp);
}

/*
  Remove N SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removen_len_p(char *R s,
		   const char *R find,
		   size_t n,
		   size_t sz,
		   const size_t findlen)
JSTR_NOEXCEPT
{
	return pjstr_removeall_len_p(PJSTR_FLAG_USE_N, s, find, n, sz, findlen);
}

/*
  Remove all SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_removeall_len_p(char *R s,
		     const char *R find,
		     size_t sz,
		     const size_t findlen)
JSTR_NOEXCEPT
{
	return pjstr_removeall_len_p(PJSTR_FLAG_USE_NOT_N, s, find, 0, sz, findlen);
}

/*
  Remove all SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_FUNC_RET_NONNULL
static char *
jstr_removeall_p(char *R s,
		 const char *R find,
		 const size_t findlen)
JSTR_NOEXCEPT
{
	return jstr_removeall_len_p(s, find, strlen(s), findlen);
}

/*
  Remove all SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_removeall_len_j(jstr_ty *R j,
		     const char *R find,
		     const size_t findlen)
JSTR_NOEXCEPT
{
	j->size = jstr_removeall_len_p(j->data, find, j->size, findlen) - j->data;
}

/*
  Remove all SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_removeall_j(jstr_ty *R j,
		 const char *R find)
JSTR_NOEXCEPT
{
	return jstr_removeall_len_j(j, find, strlen(find));
}

JSTR_INLINE
JSTR_FUNC
static int
pjstr_replaceall_len(const pjstr_flag_use_n_ty flag,
		     char *R *R s,
		     size_t *R sz,
		     size_t *R cap,
		     const size_t start_idx,
		     const char *R find,
		     const char *R rplc,
		     size_t n,
		     const size_t findlen,
		     const size_t rplclen)
JSTR_NOEXCEPT
{
	char *dst = *s + start_idx;
	if (jstr_unlikely(rplclen == 0)) {
		*sz = pjstr_removeall_len_p(flag, dst, find, n, *sz - start_idx, findlen) - *s;
		return 1;
	}
	if (jstr_unlikely(findlen == 1)) {
		if (jstr_unlikely(rplclen == 1)) {
			jstr_replacechr_len(dst, *find, *rplc, *sz - start_idx);
			return 1;
		}
	} else if (jstr_unlikely(findlen == 0))
		return 1;
	const char *p = dst;
	const char *oldp = p;
	while (((flag & PJSTR_FLAG_USE_N) ? n-- : 1)
	       && (p = jstr_strstr_len(p, (*s + *sz) - p, find, findlen))) {
		if (rplclen <= findlen)
			PJSTR_RPLCALL_IN_PLACE(dst, oldp, p, rplc, rplclen, findlen);
		else
			p = pjstr_replaceat_len_higher(s, sz, cap, p - *s, rplc, rplclen, findlen);
		if (jstr_unlikely(p == NULL))
			return 0;
	}
	if (jstr_unlikely(dst == *s))
		return 1;
	if (rplclen < findlen)
		*sz = jstr_stpmove_len(dst, oldp, *s + *sz - oldp) - *s;
	return 1;
}

/*
  Replace N SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replacen_len(char *R *R s,
		  size_t *R sz,
		  size_t *R cap,
		  const char *R find,
		  const char *R rplc,
		  size_t n,
		  const size_t findlen,
		  const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replaceall_len(PJSTR_FLAG_USE_N, s, sz, cap, 0, find, rplc, n, findlen, rplclen);
}

/*
  Replace N SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replacen_len_from(char *R *R s,
		       size_t *R sz,
		       size_t *R cap,
		       const size_t start_idx,
		       const char *R find,
		       const char *R rplc,
		       size_t n,
		       const size_t findlen,
		       const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replaceall_len(PJSTR_FLAG_USE_N, s, sz, cap, start_idx, find, rplc, n, findlen, rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replaceall_len(char *R *R s,
		    size_t *R sz,
		    size_t *R cap,
		    const char *R find,
		    const char *R rplc,
		    const size_t findlen,
		    const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replaceall_len(PJSTR_FLAG_USE_NOT_N, s, sz, cap, 0, find, rplc, 0, findlen, rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
  Return 0 on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static int
jstr_replaceall_len_from(char *R *R s,
			 size_t *R sz,
			 size_t *R cap,
			 const size_t start_idx,
			 const char *R find,
			 const char *R rplc,
			 const size_t findlen,
			 const size_t rplclen)
JSTR_NOEXCEPT
{
	return pjstr_replaceall_len(PJSTR_FLAG_USE_NOT_N, s, sz, cap, start_idx, find, rplc, 0, findlen, rplclen);
}

/*
  Reverse S.
*/
JSTR_FUNC_VOID
static void
jstr_rev_len(char *R s,
	     const size_t sz)
JSTR_NOEXCEPT
{
	int c;
	char *e = s + sz - 1;
	while (s < e) {
		c = *s;
		*s++ = *e;
		*e-- = c;
	}
}

JSTR_INLINE
JSTR_MAYBE_UNUSED
static void
jstr_rev(char *R s)
JSTR_NOEXCEPT
{
	jstr_rev_len(s, strlen(s));
}

/*
  Reverse S.
  Return value:
  ptr to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rev_p(char *R s)
JSTR_NOEXCEPT
{
	const size_t len = strlen(s);
	jstr_rev_len(s, len);
	return s + len;
}

/*
  Trim leading and trailing [\n\t\v\r ] in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_trim_len_p(char *R s,
		const size_t sz)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return s + sz;
	typedef unsigned char u;
	unsigned char *end = (u *)s + sz - 1;
	unsigned char *const start = (u *)s - 1;
	while (jstr_isspace(*end)
	       && --end != start)
		;
	*++end = '\0';
	while (jstr_isspace(*s++))
		;
	--s;
	return jstr_stpmove_len(start + 1, s, end - (u *)s);
}

/*
  Trim leading and trailing [\n\t\v\r ] in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
JSTR_INLINE
static char *
jstr_trim_p(char *R s)
JSTR_NOEXCEPT
{
	return jstr_trim_len_p(s, strlen(s));
}

/*
  Trim leading and trailing [\n\t\v\r ] in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_trim_j(jstr_ty *R j)
JSTR_NOEXCEPT
{
	j->size = jstr_trim_len_p(j->data, j->size) - j->data;
}

/*
  Insert SRC into DST[AT].
  Assume that S have enough space for SRC.
  Return value:
*/
JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_insert_len_unsafe(char *R s,
		       const size_t at,
		       const char *R src,
		       const size_t srclen)
JSTR_NOEXCEPT
{
	memcpy(s + at, src, srclen);
}

/*
  Insert SRC into DST[AT].
  Return 0 on malloc error;
  otherwise 1.
*/
JSTR_INLINE
JSTR_FUNC
static int
jstr_insert_len(char *R *R s,
		size_t *R sz,
		size_t *R cap,
		const size_t at,
		const char *R src,
		const size_t srclen)
JSTR_NOEXCEPT
{
	if (at + srclen > *sz) {
		JSTR_RESERVE_ALWAYS(s, sz, cap, at + srclen, return 0);
		*sz = at + srclen;
		*(*s + *sz) = '\0';
	}
	jstr_insert_len_unsafe(*s, at, src, srclen);
	return 1;
}

JSTR_INLINE
JSTR_MAYBE_UNUSED
static int
jstr_insert(char *R *R s,
	    size_t *R sz,
	    size_t *R cap,
	    const size_t at,
	    const char *R src)
JSTR_NOEXCEPT
{
	return jstr_insert_len(s, sz, cap, at, src, strlen(src));
}

/*
  Insert SRC after C in DST.
  Return 0 on malloc error;
  otherwise 1.
*/
JSTR_INLINE
JSTR_FUNC
static int
jstr_insertafterchr_len(char *R *R s,
			size_t *R sz,
			size_t *R cap,
			const int c,
			const char *R src,
			const size_t srclen)
JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p != NULL)
		return jstr_insert_len(s, sz, cap, p - *s + 1, src, srclen);
	return 1;
}

JSTR_INLINE
JSTR_MAYBE_UNUSED
static int
jstr_insertafterchr(char *R *R s,
		    size_t *R sz,
		    size_t *R cap,
		    const int c,
		    const char *R src)
JSTR_NOEXCEPT
{
	return jstr_insertafterchr_len(s, sz, cap, c, src, strlen(src));
}

/*
  Insert SRC after end of NE in DST.
  Return 0 on malloc error;
  otherwise 1.
*/
JSTR_FUNC
static int
jstr_insertafter_len(char *R *R s,
		     size_t *R sz,
		     size_t *R cap,
		     const char *R find,
		     const char *R src,
		     const size_t findlen,
		     const size_t srclen)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(findlen == 1))
		return jstr_insertafterchr_len(s, sz, cap, *find, src, srclen);
	if (jstr_unlikely(findlen == 0))
		return 1;
	const char *const p = jstr_strstr_len(*s, *sz, find, findlen);
	if (p != NULL)
		return jstr_insert_len(s, sz, cap, p - *s + findlen, src, srclen);
	return 1;
}

JSTR_INLINE
JSTR_FUNC
static int
jstr_insertafter(char *R *R s,
		 size_t *R sz,
		 size_t *R cap,
		 const char *R find,
		 const char *R src)
JSTR_NOEXCEPT
{
	return jstr_insertafter_len(s, sz, cap, find, src, strlen(find), strlen(src));
}

/* Escape any ESC character in S with a backslash. */
JSTR_FUNC_VOID
static void
jstr_escapespn_unsafe(char *R s,
		      size_t sz,
		      const char *R esc)
{
	const char *end = s + sz;
	while (*(s += strspn(s, esc))
	       && *(s += strcspn(s, esc))) {
		jstr_strmove_len(s + 1, s, end++ - s);
		*s = '\\';
		s += 2;
	}
}

PJSTR_END_DECLS

#undef R

#endif /* JSTR_H_REPLACE_DEF */
