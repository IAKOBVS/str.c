/* string-fzc.h -- zero byte detection with indexes.  HPPA version.
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

#ifndef JSTR_STRING_FZC_H
#define JSTR_STRING_FZC_H 1

#include "jstr-macros.h"

#include "_string-optype.h"

_Static_assert (sizeof (jstr_word_ty) == 4, "64-bit not supported");

/* Given a word X that is known to contain a zero byte, return the
   index of the first such within the long in memory order.  */
static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_first_zero (jstr_word_ty x)
{
  unsigned int ret;

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  asm ("extrw,u,<> %1,23,8,%%r0\n\t"
       "ldi 2,%0\n\t"
       "extrw,u,<> %1,15,8,%%r0\n\t"
       "ldi 1,%0\n\t"
       "extrw,u,<> %1,7,8,%%r0\n\t"
       "ldi 0,%0"
       : "=r"(ret) : "r"(x), "0"(3));

  return ret;
}

/* Similarly, but perform the search for byte equality between X1 and X2.  */
static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_first_eq (jstr_word_ty x1, jstr_word_ty x2)
{
  return jstr_word_index_first_zero (x1 ^ x2);
}

/* Similarly, but perform the search for zero within X1 or
   equality between X1 and X2.  */
static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_first_zero_eq (jstr_word_ty x1, jstr_word_ty x2)
{
  unsigned int ret;

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  asm ("extrw,u,= %1,23,8,%%r0\n\t"
       "extrw,u,<> %2,23,8,%%r0\n\t"
       "ldi 2,%0\n\t"
       "extrw,u,= %1,15,8,%%r0\n\t"
       "extrw,u,<> %2,15,8,%%r0\n\t"
       "ldi 1,%0\n\t"
       "extrw,u,= %1,7,8,%%r0\n\t"
       "extrw,u,<> %2,7,8,%%r0\n\t"
       "ldi 0,%0"
       : "=r"(ret) : "r"(x1), "r"(x1 ^ x2), "0"(3));

  return ret;
}

/* Similarly, but perform the search for zero within X1 or
   inequality between X1 and X2. */
static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_first_zero_ne (jstr_word_ty x1, jstr_word_ty x2)
{
  unsigned int ret;

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  asm ("extrw,u,<> %2,23,8,%%r0\n\t"
       "extrw,u,<> %1,23,8,%%r0\n\t"
       "ldi 2,%0\n\t"
       "extrw,u,<> %2,15,8,%%r0\n\t"
       "extrw,u,<> %1,15,8,%%r0\n\t"
       "ldi 1,%0\n\t"
       "extrw,u,<> %2,7,8,%%r0\n\t"
       "extrw,u,<> %1,7,8,%%r0\n\t"
       "ldi 0,%0"
       : "=r"(ret) : "r"(x1), "r"(x1 ^ x2), "0"(3));

  return ret;
}

/* Similarly, but search for the last zero within X.  */
static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_last_zero (jstr_word_ty x)
{
  unsigned int ret;

  /* Since we have no ctz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  asm ("extrw,u,<> %1,15,8,%%r0\n\t"
       "ldi 1,%0\n\t"
       "extrw,u,<> %1,23,8,%%r0\n\t"
       "ldi 2,%0\n\t"
       "extrw,u,<> %1,31,8,%%r0\n\t"
       "ldi 3,%0"
       : "=r"(ret) : "r"(x), "0"(0));

  return ret;
}

static JSTR_ATTR_MAYBE_UNUSED JSTR_ATTR_INLINE unsigned int
jstr_word_index_last_eq (jstr_word_ty x1, jstr_word_ty x2)
{
  return jstr_word_index_last_zero (x1 ^ x2);
}

#endif /* _STRING_FZC_H */
 */
