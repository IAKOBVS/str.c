/* Copyright (c) 2023 James Tirta Halim <tirtajames45 at gmail dot com>
   This file is part of the jstring library.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   MIT License (Expat) */

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

#define PJSTR_RMALLINPLACE(dst, oldp, p, find_len)        \
	do {                                              \
		const size_t n_ = JSTR_PTR_DIFF(p, oldp); \
		if (jstr_likely(dst != oldp))             \
			memmove(dst, oldp, n_);           \
		dst += n_;                                \
		oldp += n_ + find_len;                    \
		p += find_len;                            \
	} while (0)

#define PJSTR_RPLCALLINPLACE(dst, oldp, p, rplc, rplc_len, find_len)  \
	do {                                                          \
		const size_t n_ = JSTR_PTR_DIFF(p, oldp);             \
		if (jstr_likely(find_len != rplc_len)                 \
		    && jstr_likely(dst != oldp))                      \
			memmove(dst, oldp, n_);                       \
		dst = (char *)jstr_mempcpy(dst + n_, rplc, rplc_len); \
		oldp += n_ + find_len;                                \
		p += find_len;                                        \
	} while (0)

JSTR_FUNC_VOID
JSTR_ATTR_INLINE
static void
pjstr_rmallinplace(char **const dst,
                   const char **const oldp,
                   const char **const p,
                   size_t find_len)
JSTR_NOEXCEPT
{
	const size_t n = JSTR_PTR_DIFF(*p, *oldp);
	if (jstr_likely(*dst != *oldp))
		memmove(*dst, *oldp, n);
	*dst += n;
	*oldp += n + find_len;
	*p += find_len;
}

JSTR_FUNC_VOID
JSTR_ATTR_INLINE
static void
pjstr_rplcallinplace(char **const dst,
                     const char **const oldp,
                     const char **const p,
                     const char *R const rplc,
                     size_t rplc_len,
                     size_t find_len)
JSTR_NOEXCEPT
{
	const size_t n = JSTR_PTR_DIFF(*p, *oldp);
	if (jstr_likely(find_len != rplc_len)
	    && jstr_likely(*dst != *oldp))
		memmove(*dst, *oldp, n);
	*dst = (char *)jstr_mempcpy(*dst + n, rplc, rplc_len);
	*oldp += n + find_len;
	*p += find_len;
}

/*
  Insert SRC into DST[AT].
  Return value:
  ptr to '\0' in S.
  Assume that S have enough space for SRC.
*/
JSTR_ATTR_ACCESS((__read_only__, 4, 5))
JSTR_ATTR_INLINE
JSTR_FUNC_VOID
static void
jstr_insert_unsafe(char *R s,
                   size_t sz,
                   size_t at,
                   const char *R src,
                   size_t src_len)
JSTR_NOEXCEPT
{
	memmove(s + at + src_len,
	        s + at,
	        sz - at + 1);
	memcpy(s + at, src, src_len);
}

/*
  Insert SRC into DST[AT].
  Return JSTR_RET_ERR on malloc error;
  otherwise JSTR_RET_SUCC.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_insert_len(char *R *R s,
                size_t *R sz,
                size_t *R cap,
                size_t at,
                const char *R src,
                size_t src_len)
JSTR_NOEXCEPT
{
	if (jstr_chk(jstr_reserve(s, sz, cap, *sz + src_len)))
		return JSTR_RET_ERR;
	jstr_insert_unsafe(*s, *sz, at, src, src_len);
	*sz += src_len;
	return JSTR_RET_SUCC;
}

JSTR_FUNC
JSTR_ATTR_INLINE
static char *
pjstr_rplcat_len_higher(char *R *R s,
                        size_t *R sz,
                        size_t *R cap,
                        size_t at,
                        const char *R rplc,
                        size_t rplc_len,
                        size_t find_len)
JSTR_NOEXCEPT
{
	if (jstr_chk(jstr_reserve(s, sz, cap, *sz + rplc_len - find_len)))
		return NULL;
	jstr_strmove_len(*s + at + rplc_len,
	                 *s + at + find_len,
	                 *sz - (at + find_len));
	memcpy(*s + at, rplc, rplc_len);
	*sz += rplc_len - find_len;
	return *s + at + rplc_len;
}

/*
   Replace RPLC in S with FIND.
   Return value:
   ptr to RPLC in S + RPLCLEN;
   NULL on error.
*/
JSTR_FUNC
static char *
jstr_rplcat_len(char *R *R s,
                size_t *R sz,
                size_t *R cap,
                size_t at,
                const char *R rplc,
                size_t rplc_len,
                size_t find_len)
JSTR_NOEXCEPT
{
	if (jstr_chk(jstr_reserve(s, sz, cap, *sz + rplc_len - find_len)))
		return NULL;
	if (rplc_len != find_len)
		jstr_strmove_len(*s + at + rplc_len,
		                 *s + at + find_len,
		                 *sz - (at + find_len));
	memcpy(*s + at, rplc, rplc_len);
	*sz += rplc_len - find_len;
	return *s + at + rplc_len;
}

/*
  Insert SRC after C in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_ATTR_INLINE
JSTR_FUNC
static jstr_ret_ty
jstr_insertafterchr_len(char *R *R s,
                        size_t *R sz,
                        size_t *R cap,
                        int c,
                        const char *R src,
                        size_t src_len)
JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p != NULL)
		return jstr_insert_len(s, sz, cap, JSTR_PTR_DIFF(p, *s + 1), src, src_len);
	return JSTR_RET_SUCC;
}

/*
  Insert SRC after all C in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_insertafterallchr_len(char *R *R s,
                           size_t *R sz,
                           size_t *R cap,
                           int c,
                           const char *R src,
                           size_t src_len)
JSTR_NOEXCEPT
{
	size_t off = 0;
	const char *p;
	while ((p = (char *)memchr(*s + off, c, *sz - off))) {
		off = JSTR_PTR_DIFF(p, *s);
		if (jstr_chk(jstr_insert_len(s, sz, cap, off, src, src_len)))
			return JSTR_RET_ERR;
		off += src_len + 1;
	}
	return JSTR_RET_SUCC;
}

/*
  Insert SRC after end of NE in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_insertafter_len(char *R *R s,
                     size_t *R sz,
                     size_t *R cap,
                     const char *R find,
                     size_t find_len,
                     const char *R src,
                     size_t src_len)
JSTR_NOEXCEPT
{
	if (find_len == 1)
		return jstr_insertafterchr_len(s, sz, cap, *find, src, src_len);
	if (jstr_unlikely(find_len == 0))
		return JSTR_RET_SUCC;
	const char *const p = (char *)jstr_memmem(*s, *sz, find, find_len);
	if (p != NULL)
		return jstr_insert_len(s, sz, cap, JSTR_PTR_DIFF(p, *s + find_len), src, src_len);
	return JSTR_RET_SUCC;
}

/*
  Insert SRC after all end of NE in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_insertafterall_len(char *R *R s,
                        size_t *R sz,
                        size_t *R cap,
                        const char *R find,
                        size_t find_len,
                        const char *R src,
                        size_t src_len)
JSTR_NOEXCEPT
{
	if (find_len == 1)
		return jstr_insertafterallchr_len(s, sz, cap, *find, src, src_len);
	if (jstr_unlikely(find_len == 0))
		return JSTR_RET_SUCC;
	size_t off = 0;
	const char *p;
	while ((p = (char *)jstr_memmem(*s + off, *sz - off, find, find_len))) {
		off = JSTR_PTR_DIFF(p, *s);
		if (jstr_chk(jstr_insert_len(s, sz, cap, JSTR_PTR_DIFF(p, *s + find_len), src, src_len)))
			return JSTR_RET_ERR;
		off += find_len + src_len;
	}
	return JSTR_RET_SUCC;
}

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_ATTR_ACCESS((__read_write__, 1, 3))
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_rmchr_len_p(char *R s,
                 size_t sz,
                 int c)
JSTR_NOEXCEPT
{
	const char *const start = s;
	s = (char *)memchr(s, c, sz);
	return s ? jstr_stpmove_len(s, s + 1, JSTR_PTR_DIFF(sz, JSTR_PTR_DIFF(s, start))) : s + sz;
}

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_rmchr_p(char *R s,
             int c)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	s = strchrnul(s, c);
	return *s ? jstr_stpmove_len(s, s + 1, strlen(s)) : s;
#else
	return jstr_rmchr_len_p(s, strlen(s), c);
#endif /* HAVE_STRCHRNUL */
}

/*
  Replace first group of REJECT in S with RPLC.
*/
JSTR_FUNC_VOID
static void
jstr_rplcspn(char *R s,
             const char *R reject,
             char rplc)
JSTR_NOEXCEPT
{
	if (jstr_likely(*s)
	    && (*(s += strcspn(s, reject))))
		memset(s, rplc, strspn(s, reject));
}

/*
  Replace first group of REJECT in S with RPLC.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rplcspn_p(char *R s,
               const char *R reject,
               char rplc)
JSTR_NOEXCEPT
{
	if (jstr_likely(*s)
	    && (*(s += strcspn(s, reject)))) {
		s = jstr_stpset_len(s, rplc, strspn(s, reject));
		return s + strlen(s);
	}
	return s;
}

/*
  Replace first group of REJECT in S with RPLC.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rplcspn_len_p(char *R s,
                   const char *R reject,
                   size_t sz,
                   char rplc)
JSTR_NOEXCEPT
{
	if (jstr_likely(*s)) {
		const char *end = s + sz;
		if (*(s += strcspn(s, reject))) {
			const char *const p = s + strspn(s, reject);
			end += (p - s);
			memset(s, rplc, JSTR_PTR_DIFF(p, s));
			return (char *)end;
		}
	}
	return s;
}

/*
  Replace all REJECT in S with RPLC.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rplcallspn_p(char *R s,
                  const char *R reject,
                  char rplc)
JSTR_NOEXCEPT
{
	for (; *s && (*(s += strcspn(s, reject))); s = jstr_stpset_len(s, rplc, strspn(s, reject)))
		;
	return s;
}

/*
  Remove first group of REJECT in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmspn_p(char *R s,
             const char *R reject)
JSTR_NOEXCEPT
{
	if (jstr_likely(*s)
	    && (*(s += strcspn(s, reject)))) {
		char *const p = s + strspn(s, reject);
		return jstr_stpmove_len(s, p, strlen(p));
	}
	return s;
}

/*
  Remove first group of REJECT in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_ATTR_ACCESS((__read_write__, 1, 3))
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmspn_len_p(char *R s,
                 const char *R reject,
                 size_t sz)
JSTR_NOEXCEPT
{
	if (jstr_likely(*s)) {
		const char *const end = s + sz;
		if (*(s += strcspn(s, reject))) {
			char *const p = s + strspn(s, reject);
			return jstr_stpmove_len(p, s, JSTR_PTR_DIFF(end, p));
		}
	}
	return s;
}

/*
  Remove all REJECT in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmallspn_p(char *R s,
                const char *R reject)
JSTR_NOEXCEPT
{
	const char *oldp = s;
	const char *p = s;
	size_t find_len;
	for (; *p && (*(p += strcspn(p, reject)));) {
		find_len = strspn(p, reject);
		PJSTR_RMALLINPLACE(s, oldp, p, find_len);
	}
	if (s != oldp)
		return jstr_stpmove_len(s, oldp, JSTR_PTR_DIFF(p, oldp));
	return (char *)p;
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_ATTR_ACCESS((__read_write__, 1, 3))
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmnchr_len_p(char *R s,
                  size_t sz,
                  int c,
                  size_t n)
JSTR_NOEXCEPT
{
	char *dst = s;
	const char *oldp = s;
	const char *p = s;
	const char *const end = dst + sz;
	for (; n-- && (p = (char *)memchr(p, c, JSTR_PTR_DIFF(end, p)));) {
		PJSTR_RMALLINPLACE(dst, oldp, p, 1);
	}
	return (dst != oldp) ? jstr_stpmove_len(dst, oldp, JSTR_PTR_DIFF(end, oldp)) : s + sz;
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmallchr_len_p(char *R s,
                    size_t sz,
                    int c)
JSTR_NOEXCEPT
{
	return jstr_rmnchr_len_p(s, sz, c, (size_t)-1);
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmnchr_p(char *R s,
              int c,
              size_t n)
JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCHRNUL
	char *dst = s;
	const char *oldp = dst;
	const char *p = dst;
	for (; n-- && *(p = strchrnul((char *)p, c));)
		PJSTR_RMALLINPLACE(dst, oldp, p, 1);
	if (jstr_unlikely(dst == s))
		return s + n;
	return (dst != oldp) ? jstr_stpmove_len(dst, oldp, JSTR_PTR_DIFF(p, oldp)) : (s + n);
#else
	return jstr_rmnchr_len_p(s, strlen(s), c, n);
#endif /* HAVE_STRCHRNUL */
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rmallchr_p(char *R s,
                int c)
JSTR_NOEXCEPT
{
	return jstr_rmnchr_p(s, c, (size_t)-1);
}

/*
  Remove chars in REJECT in S.
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
	for (; *(p += strcspn(p, rjct));)
		PJSTR_RMALLINPLACE(dst, oldp, p, 1);
	return (char *)((dst != oldp) ? jstr_stpmove_len(dst, oldp, JSTR_PTR_DIFF(p, oldp)) : p);
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_rmat_len_p(char *R s,
                size_t sz,
                size_t at,
                size_t find_len)
JSTR_NOEXCEPT
{
	memmove(s + at, s + at + find_len, JSTR_PTR_DIFF(s + sz, s + at));
	return s + sz - find_len;
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_rm_len_p(char *R s,
              size_t sz,
              const char *R find,
              size_t find_len)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(find_len == 0))
		return s + sz;
	char *const p = (char *)jstr_memmem(s, sz, find, find_len);
	if (jstr_unlikely(p == NULL))
		return s + sz;
	memmove(p, p + find_len, JSTR_PTR_DIFF(s + sz, p));
	return s + sz - find_len;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_ATTR_INLINE
JSTR_FUNC_VOID
static void
jstr_rplcchr_len(char *R s,
                 size_t sz,
                 char find,
                 char rplc)
JSTR_NOEXCEPT
{
	s = (char *)memchr(s, find, sz);
	if (s != NULL)
		*s = rplc;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_ATTR_INLINE
JSTR_FUNC_VOID
static void
jstr_rplcchr(char *R s,
             char find,
             char rplc)
JSTR_NOEXCEPT
{
	s = strchr(s, find);
	if (s != NULL)
		*s = rplc;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_rplcallchr_len(char *R s,
                    size_t sz,
                    char find,
                    char rplc)
JSTR_NOEXCEPT
{
	const char *R end = s + sz;
	for (; (s = (char *)memchr(s, find, JSTR_PTR_DIFF(end, s))); *s++ = rplc)
		;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_rplcallchr(char *R s,
                char find,
                char rplc)
JSTR_NOEXCEPT
{
	for (; (s = strchr(s, find)); *s++ = rplc)
		;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_rplcnchr_len(char *R s,
                  size_t sz,
                  char find,
                  char rplc,
                  size_t n)
JSTR_NOEXCEPT
{
	const char *R end = s + sz;
	for (; n-- && (s = (char *)memchr(s, find, JSTR_PTR_DIFF(end, s))); *s++ = rplc)
		;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_FUNC_VOID
static void
jstr_rplcnchr(char *R s,
              char find,
              char rplc,
              size_t n)
JSTR_NOEXCEPT
{
	for (; n-- && (s = strchr(s, find)); *s++ = rplc)
		;
}

/*
  Replace last SEARCH in S with REPLACE.
  Return ptr to '\0' in S.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static char *
jstr_rmlast_len_p(char *R s,
                  size_t sz,
                  const char *R find,
                  size_t find_len)
JSTR_NOEXCEPT
{
	const char *const p = jstr_strrstr_len(s, sz, find, find_len);
	return p ? jstr_rmat_len_p(s, sz, JSTR_PTR_DIFF(p, s), find_len) : s + sz;
}

/*
  Replace last SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_rplclast_len(char *R *R s,
                  size_t *R sz,
                  size_t *R cap,
                  const char *R find,
                  size_t find_len,
                  const char *R rplc,
                  size_t rplc_len)
JSTR_NOEXCEPT
{
	const char *const p = jstr_strrstr_len(*s, *sz, find, find_len);
	return (p && jstr_rplcat_len(s, sz, cap, JSTR_PTR_DIFF(p, *s), rplc, rplc_len, find_len)) ? JSTR_RET_SUCC : JSTR_RET_ERR;
}

JSTR_FUNC_RET_NONNULL
static char *
jstr_rmn_len_p(char *R s,
               size_t sz,
               const char *R find,
               size_t find_len,
               size_t n)
JSTR_NOEXCEPT
{
	JSTR_ASSERT_DEBUG(jstr_strnlen(find, find_len) == find_len, "");
	if (find_len == 1)
		return jstr_rmnchr_len_p(s, sz, *find, n);
	if (jstr_unlikely(find_len == 0))
		return s + sz;
	char *dst = s;
	const char *oldp = s;
	const char *p = s;
	const char *const end = dst + sz;
	for (; n-- && (p = (char *)jstr_memmem(p, JSTR_PTR_DIFF(end, p), find, find_len));)
		PJSTR_RMALLINPLACE(dst, oldp, p, find_len);
	return (dst != end) ? jstr_stpmove_len(dst, oldp, JSTR_PTR_DIFF(end, oldp)) : s + sz;
}

/*
  Remove all SEARC in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_rmall_len_p(char *R s,
                 size_t sz,
                 const char *R find,
                 size_t find_len)
JSTR_NOEXCEPT
{
	return jstr_rmn_len_p(s, sz, find, find_len, (size_t)(size_t)-1);
}

JSTR_FUNC
static jstr_ret_ty
jstr_rplcn_len_from(char *R *R s,
                    size_t *R sz,
                    size_t *R cap,
                    size_t start_idx,
                    const char *R find,
                    size_t find_len,
                    const char *R rplc,
                    size_t rplc_len,
                    size_t n)
JSTR_NOEXCEPT
{
	JSTR_ASSERT_DEBUG(start_idx == 0 || start_idx < *sz, "");
	JSTR_ASSERT_DEBUG(strlen(*s) == *sz, "");
	JSTR_ASSERT_DEBUG(jstr_strnlen(find, find_len) == find_len, "");
	JSTR_ASSERT_DEBUG(jstr_strnlen(rplc, rplc_len) == rplc_len, "");
	char *dst = *s + start_idx;
	if (jstr_unlikely(rplc_len == 0)) {
		*sz = JSTR_PTR_DIFF(jstr_rmn_len_p(dst, *sz - start_idx, find, find_len, n), *s);
		return JSTR_RET_SUCC;
	}
	if (find_len == 1) {
		if (rplc_len == 1) {
			jstr_rplcnchr_len(dst, *sz - start_idx, *find, *rplc, n);
			return JSTR_RET_SUCC;
		}
	} else if (jstr_unlikely(find_len == 0)) {
		return JSTR_RET_SUCC;
	}
	const char *p = dst;
	const char *oldp = p;
	while (n-- && (p = (char *)jstr_memmem(p, JSTR_PTR_DIFF(*s + *sz, p), find, find_len))) {
		if (rplc_len <= find_len) {
			PJSTR_RPLCALLINPLACE(dst, oldp, p, rplc, rplc_len, find_len);
		} else {
			p = pjstr_rplcat_len_higher(s, sz, cap, JSTR_PTR_DIFF(p, *s), rplc, rplc_len, find_len);
			if (jstr_unlikely(p == NULL))
				return JSTR_RET_ERR;
		}
	}
	if (jstr_unlikely(dst == *s))
		return JSTR_RET_SUCC;
	if (rplc_len < find_len)
		*sz = JSTR_PTR_DIFF(jstr_stpmove_len(dst, oldp, JSTR_PTR_DIFF(*s + *sz, oldp)), *s);
	return JSTR_RET_SUCC;
}

/*
  Replace N SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_rplcn_len(char *R *R s,
               size_t *R sz,
               size_t *R cap,
               const char *R find,
               size_t find_len,
               const char *R rplc,
               size_t rplc_len,
               size_t n)
JSTR_NOEXCEPT
{
	return jstr_rplcn_len_from(s, sz, cap, 0, find, find_len, rplc, rplc_len, n);
}

/*
  Replace all SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_rplcall_len_from(char *R *R s,
                      size_t *R sz,
                      size_t *R cap,
                      size_t start_idx,
                      const char *R find,
                      size_t find_len,
                      const char *R rplc,
                      size_t rplc_len)
JSTR_NOEXCEPT
{
	return jstr_rplcn_len_from(s, sz, cap, start_idx, find, find_len, rplc, rplc_len, (size_t)-1);
}

/*
  Replace N SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_rplcall_len(char *R *R s,
                 size_t *R sz,
                 size_t *R cap,
                 const char *R find,
                 size_t find_len,
                 const char *R rplc,
                 size_t rplc_len)
JSTR_NOEXCEPT
{
	return jstr_rplcn_len_from(s, sz, cap, 0, find, find_len, rplc, rplc_len, (size_t)-1);
}

/*
  Replace first SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_rplc_len(char *R *R s,
              size_t *R sz,
              size_t *R cap,
              const char *R find,
              size_t find_len,
              const char *R rplc,
              size_t rplc_len)
JSTR_NOEXCEPT
{
	return jstr_rplcn_len_from(s, sz, cap, 0, find, find_len, rplc, rplc_len, 1);
}

/*
  Replace first SEARCH in S with REPLACE.
  Return JSTR_RET_ERR on malloc error;
  otherwise, 1.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_rplc_len_from(char *R *R s,
                   size_t *R sz,
                   size_t *R cap,
                   size_t start_idx,
                   const char *R find,
                   size_t find_len,
                   const char *R rplc,
                   size_t rplc_len)
JSTR_NOEXCEPT
{
	return jstr_rplcn_len_from(s, sz, cap, start_idx, find, find_len, rplc, rplc_len, 1);
}

JSTR_FUNC_VOID
static void
jstr_revcpy_len(char *R dst,
                char *R src,
                size_t src_len)
JSTR_NOEXCEPT
{
	src += src_len - 1;
	for (; src_len--; *dst++ = *src--)
		;
	*dst = '\0';
}

JSTR_FUNC
JSTR_ATTR_INLINE
static char *
jstr_revcpy_p(char *R dst,
              char *R src)
{
	const size_t len = strlen(src);
	jstr_revcpy_len(dst, src, len);
	return dst + len;
}

/*
  Reverse S.
*/
JSTR_FUNC_VOID
static void
jstr_rev_len(char *R s,
             size_t sz)
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

/*
  Reverse S.
  Return value:
  ptr to '\0' in S.
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_rev_p(char *R s)
JSTR_NOEXCEPT
{
	const size_t _len = strlen(s);
	jstr_rev_len(s, _len);
	return s + _len;
}

/*
  Trim leading and trailing jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_trimend_len_p(char *R s,
                   size_t sz)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return s;
	char *end = jstr_skipspace_rev(s, s + sz - 1);
	*++end = '\0';
	return end;
}

/*
  Trim leading and trailing jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_trimend_p(char *R s)
JSTR_NOEXCEPT
{
	return jstr_trimend_len_p(s, strlen(s));
}

/*
  Trim leading and trailing jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_trimstart_len_p(char *R s,
                     size_t sz)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return s;
	const char *const start = jstr_skipspace(s);
	if (s != start)
		return jstr_stpmove_len(s, start, JSTR_PTR_DIFF(s + sz, start));
	return s + sz;
}

/*
  Trim leading jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_trimstart_p(char *R s)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return s;
	const char *const start = jstr_skipspace(s);
	if (s != start)
		return jstr_stpmove_len(s, start, strlen(start));
	return s + strlen(start);
}

/*
  Trim leading jstr_isspace() chars in S.
*/
JSTR_FUNC_VOID
JSTR_ATTR_INLINE
static void
jstr_trimstart(char *R s)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return;
	const char *const start = jstr_skipspace(s);
	if (s != start)
		jstr_strmove_len(s, start, strlen(start));
}

/*
  Trim leading and trailing jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_trim_len_p(char *R s,
                size_t sz)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(*s == '\0'))
		return s;
	const char *const end = jstr_skipspace_rev(s, s + sz - 1) + 1;
	const char *const start = jstr_skipspace(s);
	if (start != s)
		return jstr_stpmove_len(s, start, JSTR_PTR_DIFF(end, start));
	return s + sz;
}

/*
  Trim leading and trailing jstr_isspace() chars in S.
  Return value:
  ptr to '\0' in S;
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_trim_p(char *R s)
JSTR_NOEXCEPT
{
	return jstr_trim_len_p(s, strlen(s));
}

/*
  Place SRC into DST[AT].
  Assume that S have enough space for SRC.
  Return value:
*/
JSTR_FUNC_VOID
JSTR_ATTR_INLINE
static void
jstr_place_len_unsafe(char *R s,
                      size_t at,
                      const char *R src,
                      size_t src_len)
JSTR_NOEXCEPT
{
	memcpy(s + at, src, src_len);
}

/*
  Place SRC into DST[AT].
  Return JSTR_RET_ERR on malloc error;
  otherwise JSTR_RET_SUCC.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_place_len(char *R *R s,
               size_t *R sz,
               size_t *R cap,
               size_t at,
               const char *R src,
               size_t src_len)
JSTR_NOEXCEPT
{
	if (at + src_len > *sz) {
		if (jstr_chk(jstr_reservealways(s, sz, cap, at + src_len)))
			return JSTR_RET_ERR;
		*sz = at + src_len;
		*(*s + *sz) = '\0';
	}
	jstr_place_len_unsafe(*s, at, src, src_len);
	return JSTR_RET_SUCC;
}

/*
  Place SRC after C in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise JSTR_RET_SUCC.
*/
JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_placeafterchr_len(char *R *R s,
                       size_t *R sz,
                       size_t *R cap,
                       int c,
                       const char *R src,
                       size_t src_len)
JSTR_NOEXCEPT
{
	const char *const p = (char *)memchr(*s, c, *sz);
	if (p != NULL)
		return jstr_place_len(s, sz, cap, JSTR_PTR_DIFF(p, *s + 1), src, src_len);
	return JSTR_RET_SUCC;
}

/*
  Place SRC after end of NE in DST.
  Return JSTR_RET_ERR on malloc error;
  otherwise JSTR_RET_SUCC.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_placeafter_len(char *R *R s,
                    size_t *R sz,
                    size_t *R cap,
                    const char *R find,
                    size_t find_len,
                    const char *R src,
                    size_t src_len)
JSTR_NOEXCEPT
{
	if (find_len == 1)
		return jstr_placeafterchr_len(s, sz, cap, *find, src, src_len);
	if (jstr_unlikely(find_len == 0))
		return JSTR_RET_SUCC;
	const char *const p = (char *)jstr_memmem(*s, *sz, find, find_len);
	if (p != NULL)
		return jstr_place_len(s, sz, cap, JSTR_PTR_DIFF(p, *s + find_len), src, src_len);
	return JSTR_RET_SUCC;
}

/*
   Convert snake_case to camelCase.
   Return ptr to '\0' in S.
   Leading underscores are preserved.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_toCamelCaseP(char *R s)
JSTR_NOEXCEPT
{
	for (; *s == '_'; ++s)
		;
	for (; *s && *s != '_'; ++s)
		;
	if (jstr_unlikely(*s == '\0'))
		return s;
	const char *src = s;
	goto start;
	for (; *src; ++src)
		if (jstr_likely(*src != '_'))
			*s++ = *src;
		else {
start:
			*s++ = jstr_toupper(*++src);
			if (jstr_unlikely(*src == '\0'))
				break;
		}
	*s = '\0';
	return s;
}

/*
   Convert snake_case to camelCase.
   Return ptr to '\0' in DST.
   Leading underscores are preserved.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_toCamelCaseCpyP(char *R dst,
                     const char *R src)
JSTR_NOEXCEPT
{
	for (; *src == '_'; ++src, *dst++ = '_')
		;
	while (*src)
		if (*src != '_') {
			*dst++ = *src++;
		} else {
			if (jstr_unlikely(*++src == '\0'))
				break;
			*dst++ = jstr_toupper(*src++);
		}
	*dst = '\0';
	return dst;
}

/*
   Convert camelCase to snake_case.
   Return ptr to '\0' in S.
   Leading underscores are preserved.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_to_snake_case_p(char *R s)
JSTR_NOEXCEPT
{
	for (; *s == '_'; ++s)
		;
	*s = jstr_tolower(*s);
	for (; *s && !jstr_isupper(*s); ++s)
		;
	if (jstr_unlikely(*s == '\0'))
		return s;
	const char *end = s + strlen(s);
	goto start;
	for (; *s; ++s)
		if (jstr_isupper(*s)) {
start:
			jstr_strmove_len(s + 1, s, JSTR_PTR_DIFF(end++, s));
			*s++ = '_';
			*s = jstr_tolower(*s);
		}
	*s = '\0';
	return s;
}

/*
   Convert camelCase to snake_case.
   Return ptr to '\0' in DST.
   Leading underscores are preserved.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_to_snake_case_cpy_p(char *R dst,
                         const char *R src)
JSTR_NOEXCEPT
{
	for (; *src == '_'; ++src, *dst++ = '_')
		;
	*dst = jstr_tolower(*src);
	while (*src)
		if (!jstr_isupper(*src)) {
			*dst++ = *src++;
		} else {
			*dst = '_';
			*(dst + 1) = jstr_tolower(*src++);
			dst += 2;
		}
	*dst = '\0';
	return dst;
}

/*
   Non-destructive strtok.
   END must be NUL terminated.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok_ne_len(const char **R const save_ptr,
                   const char *R const end,
                   const char *R ne,
                   size_t ne_len)
JSTR_NOEXCEPT
{
	const char *const s = *save_ptr;
	if (jstr_unlikely(*s == '\0'))
		return NULL;
	*save_ptr = jstr_strstrnul_len(s, JSTR_PTR_DIFF(end, s), ne, ne_len);
	if (jstr_likely(**save_ptr != '\0'))
		*save_ptr += ne_len;
	return (char *)s;
}

/*
   Non-destructive strtok.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok_ne(const char **R const save_ptr,
               const char *R ne)
JSTR_NOEXCEPT
{
	const char *const s = *save_ptr;
	if (jstr_unlikely(*s == '\0'))
		return NULL;
	*save_ptr = jstr_strstrnul(s, ne);
	if (jstr_likely(**save_ptr != '\0'))
		*save_ptr += strlen(ne);
	return (char *)s;
}

/*
   Non-destructive strtok.
   Instead of nul-termination, use the save_ptr to know the length of the string.
*/
JSTR_FUNC_PURE
static char *
jstr_strtok(const char **R save_ptr,
            const char *R delim)
JSTR_NOEXCEPT
{
	const char *s = *save_ptr;
	if (jstr_unlikely(*s == '\0'))
		return NULL;
	s += strspn(s, delim);
	if (jstr_unlikely(*s == '\0')) {
		*save_ptr = s;
		return NULL;
	}
	*save_ptr = s + strcspn(s, delim);
	return (char *)s;
}

JSTR_FUNC
JSTR_ATTR_INLINE
static char *
jstr_cpy_p(char *R dst,
           const jstr_ty *R src)
JSTR_NOEXCEPT
{
	return jstr_stpcpy_len(dst, src->data, src->size);
}

JSTR_FUNC
JSTR_ATTR_INLINE
static jstr_ret_ty
jstr_dup(jstr_ty *R dst,
         const jstr_ty *R src)
JSTR_NOEXCEPT
{
	dst->data = (char *)malloc(src->capacity);
	if (jstr_nullchk(dst->data))
		return JSTR_RET_ERR;
	dst->size = JSTR_PTR_DIFF(jstr_cpy_p(dst->data, dst), dst->data);
	dst->size = src->size;
	dst->capacity = src->capacity;
	return JSTR_RET_SUCC;
}

/* Return ptr to '\0' in DST. */
JSTR_FUNC
static char *
jstr_repeat_len_unsafe_p(char *s,
                         size_t sz,
                         size_t n)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(n < 2))
		return s + sz;
	--n;
	if (jstr_likely(sz > 1))
		while (n--)
			s = (char *)jstr_mempmove(s + sz, s, sz);
	else if (sz == 1)
		s = (char *)memset(s, *s, n) + n;
	*s = '\0';
	return s;
}

/*
   Return value:
   JSTR_RET_ERR on error;
   1 otherwise.
*/
JSTR_FUNC
static jstr_ret_ty
jstr_repeat_len(char *R *R s,
                size_t *R sz,
                size_t *R cap,
                size_t n)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(n <= 1))
		return JSTR_RET_SUCC;
	if (jstr_chk(jstr_reserve(s, sz, cap, *sz * n)))
		return JSTR_RET_ERR;
	*sz = JSTR_PTR_DIFF(jstr_repeat_len_unsafe_p(*s, *sz, n), *s);
	return JSTR_RET_SUCC;
}

/* Return ptr to '\0' in DST. */
JSTR_FUNC
static char *
jstr_repeatcpy_len_p(char *R dst,
                     const char *R src,
                     size_t src_len,
                     size_t n)
JSTR_NOEXCEPT
{
	if (jstr_likely(src_len > 1))
		while (n--)
			dst = (char *)jstr_mempcpy(dst, src, src_len);
	else if (src_len == 1)
		dst = (char *)memset(dst, *src, n) + n;
	*dst = '\0';
	return dst;
}

/*
   Add thousand separator to NPTR.
   Return value:
   ptr to '\0' in NPTR.
   For example: 1234 becomes 1,234.
*/
JSTR_FUNC_RET_NONNULL
static char *
jstr_thousep_len_p(char *R nptr,
                   size_t sz,
                   char separator)
JSTR_NOEXCEPT
{
	char *end = nptr + sz;
	if (*nptr == '-') {
		++nptr;
		--sz;
	} else if (jstr_unlikely(sz == 0))
		return nptr;
	if (sz < 4)
		return end;
	size_t dif = (sz - 1) / 3;
	end += dif;
	const char *const start = nptr;
	nptr += (sz - 1);
	int n;
	for (n = 0; nptr >= start;) {
		*(nptr + dif) = *nptr;
		--nptr;
		if (++n == 3) {
			*(nptr + dif) = (char)separator;
			if (jstr_unlikely(--dif == 0))
				break;
			n = 0;
		}
	}
	*end = '\0';
	return (char *)end;
}

/*
   Add thousand separator to NPTR.
   Return value:
   ptr to '\0' in NPTR.
   For example: 1234 becomes 1,234.
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_thousep_p(char *R nptr,
               char separator)
JSTR_NOEXCEPT
{
	return jstr_thousep_len_p(nptr, strlen(nptr), separator);
}

/*
   Copy SRC to DST, adding thousand separator.
   Return value:
   ptr to '\0' in DST.
*/
JSTR_ATTR_ACCESS((__read_only__, 2, 3))
JSTR_FUNC_RET_NONNULL
static char *
jstr_thousepcpy_len_p(char *R dst,
                      const char *R src,
                      size_t src_len,
                      char separator)
JSTR_NOEXCEPT
{
	if (*src == '-') {
		*dst++ = '-';
		++src;
		--src_len;
	}
	if (src_len < 4) {
		while ((*dst++ = *src++))
			;
		return dst - 1;
	}
	int i = src_len % 3;
	for (int j = i; j--; *dst++ = *src++)
		;
	if (i) {
		*dst++ = separator;
		i = 0;
	}
	for (; *src; ++i) {
		if (i == 3) {
			*dst = separator;
			*(dst + 1) = *src++;
			dst += 2;
			i = 0;
		} else {
			*dst++ = *src++;
		}
	}
	*dst = '\0';
	return dst;
}

/*
   Copy SRC to DST, adding thousand separator.
   Return value:
   ptr to '\0' in DST.
*/
JSTR_FUNC_RET_NONNULL
JSTR_ATTR_INLINE
static char *
jstr_thousepcpy_p(char *R dst,
                  const char *R src,
                  char separator)
JSTR_NOEXCEPT
{
	return jstr_thousepcpy_len_p(dst, src, strlen(src), separator);
}

PJSTR_END_DECLS

#undef R

#endif /* JSTR_H_REPLACE_DEF */
