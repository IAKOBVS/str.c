/* Zero byte detection; basics.  RISCV version.
   Copyright (C) 2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#ifndef PJSTR_RISCV_STRING_FZA_H
#define PJSTR_RISCV_STRING_FZA_H 1
#include "jstr-macros.h"
#if defined __riscv_zbb || defined __riscv_xtheadbb
/* With bitmap extension we can use orc.b to find all zero bytes.  */
#	include "jstr-string-misc.h"
#	include "jstr-string-optype.h"
/* The functions return a byte mask.  */
/* This function returns 0xff for each byte that is zero in X.  */
static JSTR_INLINE jstr_word_ty
jstr_word_find_zero_all(jstr_word_ty x)
{
	jstr_word_ty r;
#	ifdef __riscv_xtheadbb
	asm("th.tstnbz %0, %1"
	    : "=r"(r)
	    : "r"(x));
	return r;
#	else
	asm("orc.b %0, %1"
	    : "=r"(r)
	    : "r"(x));
	return ~r;
#	endif
}
/* This function returns 0xff for each byte that is equal between X1 and
   X2.  */
static JSTR_INLINE jstr_word_ty
jstr_word_find_eq_all(jstr_word_ty x1, jstr_word_ty x2)
{
	return jstr_word_find_zero_all(x1 ^ x2);
}
/* Identify zero bytes in X1 or equality between X1 and X2.  */
static JSTR_INLINE jstr_word_ty
jstr_word_find_zero_eq_all(jstr_word_ty x1, jstr_word_ty x2)
{
	return jstr_word_find_zero_all(x1) | jstr_word_find_eq_all(x1, x2);
}
/* Identify zero bytes in X1 or inequality between X1 and X2.  */
static JSTR_INLINE jstr_word_ty
jstr_word_find_zero_ne_all(jstr_word_ty x1, jstr_word_ty x2)
{
	return jstr_word_find_zero_all(x1) | ~jstr_word_find_eq_all(x1, x2);
}
/* Define the "inexact" versions in terms of the exact versions.  */
#	define jstr_word_find_zero_low	   jstr_word_find_zero_all
#	define jstr_word_find_eq_low	   jstr_word_find_eq_all
#	define jstr_word_find_zero_eq_low jstr_word_find_zero_eq_all
#else
#	include "jstr-string-fza.h"
#endif
#endif /* _RISCV_STRING_FZA_H  */