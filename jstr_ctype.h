#ifndef JSTR_CTYPE_DEF_H
#define JSTR_CTYPE_DEF_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */
#ifdef __cplusplus
}
#endif /* __cpluslus */

#include "jstr_ctype_table.h"
#include "jstr_macros.h"
#include <string.h>

#include "string-fza.h"
#include "string-fzb.h"
#include "string-fzc.h"
#include "string-fzi.h"
#include "string-misc.h"
#include "string-opthr.h"
#include "string-optype.h"
#include "string-shift.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isupper(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISUPPER;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_islower(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISLOWER;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isalnum(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISALNUM;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isdigit(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISDIGIT;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isxdigit(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISXDIGIT;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isblank(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISBLANK;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isgraph(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISGRAPH;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_ispunct(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISPUNCT;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_NOTHROW
JSTR_WARN_UNUSED
static int
jstr_isspace(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISSPACE;
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isalpha(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_ctype[(unsigned char)_c] & JSTR_ISALPHA;
}

/*
   Will NOT handle EOF correctly.
   toupper_ascii(EOF) != EOF;
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_toupper_ascii(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_toupper_ascii[(unsigned char)_c];
}

/*
   Will NOT handle EOF correctly.
   tolower_ascii(EOF) != EOF;
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_tolower_ascii(const int _c) JSTR_NOEXCEPT
{
	return pjstr_table_tolower_ascii[(unsigned char)_c];
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isalnum_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_s == '\0'))
		return 1;
	while (jstr_isalnum(*_s++))
		;
	return (*_s - 1) == '\0';
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isalpha_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_s == '\0'))
		return 1;
	while (jstr_isalpha(*_s++))
		;
	return (*_s - 1) == '\0';
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_islower_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_s == '\0'))
		return 1;
	while (jstr_islower(*_s++))
		;
	return (*_s - 1) == '\0';
}

JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isupper_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_s == '\0'))
		return 1;
	while (jstr_isupper(*_s++))
		;
	return *(_s - 1) == '\0';
}

JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_NOTHROW
JSTR_WARN_UNUSED
static int
jstr_isspace_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	return *(_s + strspn(_s, "\t\n\v\f\r ")) == '\0';
}

JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_NOTHROW
JSTR_WARN_UNUSED
static int
jstr_isblank_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	return *(_s + strspn(_s, " \t")) == '\0';
}

JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_isdigit_str(const char *JSTR_RST _s) JSTR_NOEXCEPT
{
	return *(_s + strspn(_s, "0123456789")) == '\0';
}

JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_NOTHROW
static void
jstr_tolower_str(char *JSTR_RST _s) JSTR_NOEXCEPT
{
#if JSTR_HAVE_ATTR_MAY_ALIAS
	pjstr_op_ty *_sw = (pjstr_op_ty *)_s;
remainder:
	if (!_s[0])
		return;
	_s[0] = jstr_toupper_ascii(_s[0]);
	if (!_s[1])
		return;
	_s[1] = jstr_toupper_ascii(_s[1]);
	if (!_s[2])
		return;
	_s[2] = jstr_toupper_ascii(_s[2]);
	if (!_s[3])
		return;
	_s[3] = jstr_toupper_ascii(_s[3]);
	if (!_s[4])
		return;
	_s[4] = jstr_toupper_ascii(_s[4]);
	if (!_s[5])
		return;
	_s[5] = jstr_toupper_ascii(_s[5]);
	if (!_s[6])
		return;
	_s[6] = jstr_toupper_ascii(_s[6]);
	if (!_s[7])
		return;
	*_sw = PJSTR_PTR_ALIGN_DOWN(_sw, sizeof(*_sw));
	while (!pjstr_has_zero(*_sw)) {
		*(char *)_sw = jstr_tolower_ascii(*_sw);
		((char *)_sw)[1] = jstr_tolower_ascii(((char *)_sw)[1]);
		((char *)_sw)[2] = jstr_tolower_ascii(((char *)_sw)[2]);
		((char *)_sw)[3] = jstr_tolower_ascii(((char *)_sw)[3]);
		((char *)_sw)[4] = jstr_tolower_ascii(((char *)_sw)[4]);
		((char *)_sw)[5] = jstr_tolower_ascii(((char *)_sw)[5]);
		((char *)_sw)[6] = jstr_tolower_ascii(((char *)_sw)[6]);
		((char *)_sw)[7] = jstr_tolower_ascii(((char *)_sw)[7]);
		++_sw;
	}
	goto remainder;
#else
	for (;;) {
		if (!_s[0])
			break;
		_s[0] = jstr_tolower_ascii(!_s[0]);
		if (!_s[1])
			break;
		_s[1] = jstr_tolower_ascii(!_s[1]);
		if (!_s[2])
			break;
		_s[2] = jstr_tolower_ascii(!_s[2]);
		if (!_s[3])
			break;
		_s[3] = jstr_tolower_ascii(!_s[3]);
		_s += 4;
	}
#endif
}

JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_NOTHROW
static void
jstr_toupper_str(char *JSTR_RST _s) JSTR_NOEXCEPT
{
#if JSTR_HAVE_ATTR_MAY_ALIAS
	pjstr_op_ty *_sw = (pjstr_op_ty *)_s;
remainder:
	if (!_s[0])
		return;
	_s[0] = jstr_toupper_ascii(_s[0]);
	if (!_s[1])
		return;
	_s[1] = jstr_toupper_ascii(_s[1]);
	if (!_s[2])
		return;
	_s[2] = jstr_toupper_ascii(_s[2]);
	if (!_s[3])
		return;
	_s[3] = jstr_toupper_ascii(_s[3]);
	if (!_s[4])
		return;
	_s[4] = jstr_toupper_ascii(_s[4]);
	if (!_s[5])
		return;
	_s[5] = jstr_toupper_ascii(_s[5]);
	if (!_s[6])
		return;
	_s[6] = jstr_toupper_ascii(_s[6]);
	if (!_s[7])
		return;
	_s[7] = jstr_toupper_ascii(_s[7]);
	*_sw = PJSTR_PTR_ALIGN_UP(*_sw, sizeof(*_sw));
	while (!pjstr_has_zero(*_sw)) {
		*(char *)_sw = jstr_toupper_ascii(*_sw);
		((char *)_sw)[1] = jstr_toupper_ascii(((char *)_sw)[1]);
		((char *)_sw)[2] = jstr_toupper_ascii(((char *)_sw)[2]);
		((char *)_sw)[3] = jstr_toupper_ascii(((char *)_sw)[3]);
		((char *)_sw)[4] = jstr_toupper_ascii(((char *)_sw)[4]);
		((char *)_sw)[5] = jstr_toupper_ascii(((char *)_sw)[5]);
		((char *)_sw)[6] = jstr_toupper_ascii(((char *)_sw)[6]);
		((char *)_sw)[7] = jstr_toupper_ascii(((char *)_sw)[7]);
		++_sw;
	}
	_s = (char *)_sw;
	goto remainder;
#else
	for (;;) {
		if (jstr_unlikely(_s[0]) == '\0')
			break;
		_s[0] = jstr_toupper_ascii(_s[0]);
		if (jstr_unlikely(_s[1]) == '\0')
			break;
		_s[1] = jstr_toupper_ascii(_s[1]);
		if (jstr_unlikely(_s[2]) == '\0')
			break;
		_s[2] = jstr_toupper_ascii(_s[2]);
		if (jstr_unlikely(_s[3]) == '\0')
			break;
		_s[3] = jstr_toupper_ascii(_s[3]);
		_s += 4;
	}
#endif
}

#ifdef __cplusplus
} /* extern C */
#endif /* __cpluslus */

#endif /* JSTR_CTYPE_DEF_H */
