#pragma once

#define POSIT_NBITS 8
#define POSIT_ES 0
#include "slowposit/posit_macros.h"

#define POSIT_NBITS 16
#define POSIT_ES 1
#include "slowposit/posit_macros.h"

#define POSIT_NBITS 32
#define POSIT_ES 2
#include "slowposit/posit_macros.h"

#define POSIT_NBITS 64
#define POSIT_ES 3
#include "slowposit/posit_macros.h"

#define POSIT_NBITS 128
#define POSIT_ES 4
#include "slowposit/posit_macros.h"






//POSIT_T POSIT_MKNAME(add)

// posit_add
// posit_add_exact
// posit_sub
// posit_sub_exact
// posit_mul
// posit_mul_exact
//
/*
math

padd(2,1) ROUNDS add 2 posits, rounds to nearest
  * padd8, padd16, padd32, padd64(4,2)
paddC(2,2) add 2 posits, return the truncated sum and the difference between the truncated sum and the true sum
  * paddC8, paddC16, paddC32, paddC64(4,4)
psub(2,1) ROUNDS same as add with second operand negated
  * psub8, psub16, psub32, psub64(4,2)
psubC(2,2) subtract a posit from another posit, return the truncated difference and the difference between the truncated difference and the true difference
  * psubC8, psubC16, psubC32, psubC64(4,4)

pmul(2,1) ROUNDS multiply 2 posits and returns the result as 1 posit of the same size, rounded to nearest
  * pmul8, pmul16, pmul32, pmul64(4,2)
pmulC(2,2) multiplies 2 posits and returns a posit of one size bigger, does not need to round
  * pmulC8, pmulC16, pmulC32, pmulC64(4,4)

pdiv(2,1) ROUNDS divides 2 posits and returns the quotient as 1 posit of the same size, rounded to nearest
  * pdiv8, pdiv16, pdiv32, pdiv64(4,2)
pdivC(2,2) divides 2 posits and returns the quotient as 1 posit of one size bigger, no rounding is needed
  * pdivC8, pdivC16, pdivC32, pdivC64(4,4)

psqrt(1,1) ROUNDS finds the square root of a posit, outputs a posit of the same size
  * psqrt8, psqrt16, psqrt32, psqrt64(2,2)


conversions

pfromint(1,1) convert a signed integer of nbits to a posit
  * pfromint8, pfromint16, pfromint32, pfromint64(2,2)

pfromuint(1,1) convert an unsigned integer of nbits to a posit
  * pfromuint8, pfromuint16, pfromuint32, pfromuint64(2,2)

pint(1,1) ROUNDS convert the posit to a signed int of size nbits, rounding to nearest and saturating at 0x7fffffff, 0x80000000 and 0
  * pint8, pint16, pint32, pint64(2,2)

puint(1,1) ROUNDS convert the posit to an unsigned int of size nbits, rounding to nearest and saturating at 0xffffffff and 0
  * puint8, puint16, puint32, puint64(2,2)

pup(1,2) up-casts the posit to the next size
  * pup8, pup16, pup32

pdown(2,1) ROUNDS takes 2 words of posits and returns 1 word containing posits of the next smaller size (rounded)
  * pdown16, pdown32, pdown64


bit hacking

pexp(1,1) get posit's exponent as a signed integer the same as the posit width for 8, 16 and 32 and of half the posit width for 64 and 128
  * pexp8, pexp16, pexp32, pexp64(2,1), pexp128(4,2)

pmant(1,1) output a representation of the posit >= 0.5 and < 1 (exponent zero)
  * pmant8, pmant16, pmant32, pmant64(2,2)

pexpadd(2,1) ROUNDS "shift" posit A by B bits (adjust exponent), may round if B is positive
  * pshift8, pshift16, pshift32, pshift64(4,2)
  *
*/
