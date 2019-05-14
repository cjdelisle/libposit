// SPDX-License-Identifier: MIT OR BSD-3-Clause
#include "positutil.h"

#include <stdbool.h>
#include <mpfr.h>
#include <gmp.h>
#include <assert.h>

#define POSIT_MKNAME(name) POSIT_GLUE6(posit,POSIT_NBITS,_,name,_,POSIT_IMPL)

#define POSIT_GLUE6(a,b,c,d,e,f) POSIT_GLUE3(POSIT_GLUE4(a,b,c,d),e,f)
#define POSIT_GLUE4(a,b,c,d) POSIT_GLUE(POSIT_GLUE(a,b),POSIT_GLUE(c,d))
#define POSIT_GLUE3(a,b,c) POSIT_GLUE(POSIT_GLUE(a,b),c)
#define POSIT_GLUE(a,b) POSIT_GLUE_(a,b)
#define POSIT_GLUE_(a,b) a ## b


#define POSIT_WORD_SZ       ((int)(8 * sizeof(char*))) // pointer size in bits
#define POSIT_SIGBIT        (POSIT_NBITS-1)
#define POSIT_REGBIT0       (POSIT_NBITS-2)
#define POSIT_DEBUGF(a, ...) //mpfr_printf(a, __VA_ARGS__)

static inline bool POSIT_MKNAME(mpzIsNaR)(mpz_t p) {
    return (mpz_popcount(p) == 1) && (mpz_tstbit(p, POSIT_SIGBIT) == 1);
}
static inline void POSIT_MKNAME(mpzMkNaR)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_SIGBIT);
}
static inline void POSIT_MKNAME(mpzMkMax)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_SIGBIT);
    mpz_sub_ui(p, p, 1);
}
static inline void POSIT_MKNAME(mpzMkNegMax)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_SIGBIT);
    mpz_add_ui(p, p, 1);
}
static inline void POSIT_MKNAME(mpzMkOnes)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_NBITS);
    mpz_sub_ui(p, p, 1);
}

// 2s complement negation
static inline void POSIT_MKNAME(mpzNegate)(mpz_t p) {
    mpz_t scratch;
    mpz_init(scratch);
    POSIT_MKNAME(mpzMkOnes)(scratch);
    mpz_xor(p, p, scratch);
    mpz_add_ui(p, p, 1);
    mpz_and(p, p, scratch);
    mpz_clear(scratch);
}

int POSIT_MKNAME(mpzFromMpfr)(mpz_t out, mpfr_t in) {
    if (!mpfr_number_p(in)) {
        POSIT_MKNAME(mpzMkNaR)(out);
        return false;
    }

    int _sign = mpfr_sgn(in);
    if (!_sign) {
        mpz_set_ui(out, 0);
        return true;
    }
    bool negative = (_sign < 0);

    mpfr_clear_flags();

    mpfr_exp_t exponent = mpfr_get_exp(in);

    if (exponent > POSIT_GLUE4(posit,POSIT_NBITS,_,LOG2_MAXVAL) + 1) {
        POSIT_DEBUGF("oversize exponent, bailing out exp: %d %d\n", exponent,
            POSIT_GLUE4(posit,POSIT_NBITS,_,LOG2_MAXVAL));
        if (mpfr_signbit(in)) {
            POSIT_MKNAME(mpzMkNegMax)(out);
        } else {
            POSIT_MKNAME(mpzMkMax)(out);
        }
        POSIT_DEBUGF("x> %Zx\n", out);
        return false;
    }

    bool lessThan1 = (exponent < 1);
    uint32_t scale = (lessThan1 ? -exponent : (exponent-1));
    uint32_t regimeBits = (scale>>POSIT_ES);
    uint32_t subexponent = scale % (1<<POSIT_ES);
    if (lessThan1) {
        subexponent = (1<<POSIT_ES) - 1 - subexponent;
    }

POSIT_DEBUGF("\n>> origEx = %d\n", exponent);

    // We're going to first flag the sign bit so that
    // the top of our 2*POSIT_NBITS-1 precision is fixed.
    mpfr_t f1;
    mpfr_init2(f1, 2*POSIT_NBITS-1);
    positutil_CANT_ROUND(mpfr_set_si(f1, 1, MPFR_RNDZ));

    bool inexact = false;

POSIT_DEBUGF("\n>> %Rb\n", f1);
    for (uint32_t i = 0; i < regimeBits+1; i++) {
        if (i >= POSIT_NBITS-1) {
            inexact = true;
            break;
        }
        positutil_CANT_ROUND(mpfr_mul_2si(f1, f1, 1, MPFR_RNDZ));
        positutil_CANT_ROUND(mpfr_add_si(f1, f1, !lessThan1, MPFR_RNDZ));
        //POSIT_DEBUGF("\n>> %Rb\n", f1);
    }

    // Guard bit
    positutil_CANT_ROUND(mpfr_mul_2si(f1, f1, 1, MPFR_RNDZ));
    positutil_CANT_ROUND(mpfr_add_si(f1, f1, lessThan1, MPFR_RNDZ));

    // Add the subexponent
    positutil_CANT_ROUND(mpfr_mul_2si(f1, f1, POSIT_ES, MPFR_RNDZ));
    positutil_CANT_ROUND(mpfr_add_ui(f1, f1, subexponent, MPFR_RNDZ));

    POSIT_DEBUGF("\n>> %Rb %.40Rf inexact = %d  subex = %d reg = %d\n", f1, in, inexact, subexponent, regimeBits);

    // Subtract the hidden bit
    positutil_CANT_ROUND(mpfr_sub_si(f1, f1, 1, MPFR_RNDZ));

    POSIT_DEBUGF("\nk> %Rb %.40Rf inexact = %d  subex = %d reg = %d\n", f1, in, inexact, subexponent, regimeBits);

    // Shift so that the hidden bit lines up with the top of the input
    positutil_CANT_ROUND(mpfr_mul_2si(f1, f1, exponent-1, MPFR_RNDZ));

    // Round 1 (fight!)
    if (negative) {
        mpfr_sub(f1, f1, in, MPFR_RNDZ);
    } else {
        mpfr_add(f1, f1, in, MPFR_RNDZ);
    }
    inexact |= (mpfr_inexflag_p() != 0);
    mpfr_clear_inexflag();
    assert(!positutil_CHK_FLAGS());

    POSIT_DEBUGF(">> %Rb %Rb inexact = %d\n", f1, in, inexact);

    // Put the sign bit just below the decimal point
    mpfr_exp_t trash = 0;
    positutil_CANT_ROUND(mpfr_frexp(&trash, f1, f1, MPFR_RNDZ));

    // Shift up by 2*POSIT_NBITS so that the 1's place is 1 bit below the bottom of the input
    // (recall the input was limited to 2*POSIT_NBITS-1 bits of precision)
    positutil_CANT_ROUND(mpfr_mul_2si(f1, f1, POSIT_NBITS*2, MPFR_RNDZ));

    // Bump the precision by 1 bit
    positutil_CANT_ROUND(mpfr_prec_round(f1, POSIT_NBITS*2, MPFR_RNDZ));

    // Add an "inexact bit" which will be a tie-breaker, just as would have been the bits
    // that are now rounded off
    positutil_CANT_ROUND(mpfr_add_si(f1, f1, inexact, MPFR_RNDZ));

    POSIT_DEBUGF("x> %Rb %Rb f = %d\n", f1, in, positutil_CHK_FLAGS());

    // Round 2 (fight!)
    // This time we round-to-nearest
    inexact |= (mpfr_prec_round(f1, POSIT_NBITS, MPFR_RNDN) != 0);
    POSIT_DEBUGF(">> %Rb %Rb f = %d\n", f1, in, positutil_CHK_FLAGS());
    assert(!positutil_CHK_FLAGS());

    POSIT_DEBUGF(">> %Rb %Rb inexact = %d\n", f1, in, inexact);

    // We have POSIT_NBITS bits so we just need to output them to the mpz
    positutil_CANT_ROUND(mpfr_div_2si(f1, f1, POSIT_NBITS, MPFR_RNDZ));
    positutil_CANT_ROUND(mpfr_get_z(out, f1, MPFR_RNDZ));

    // Drop the sign bit
    mpz_clrbit(out, POSIT_NBITS-1);

    POSIT_DEBUGF(">> %Zx\n", out);
    if (!mpz_cmp_si(out, 0)) {
        mpz_add_ui(out, out, 1);
    }

    // And if it's negative, we negate the whole thing, setting the sign bit again
    if (negative) {
        POSIT_MKNAME(mpzNegate)(out);
    }

    mpfr_clear(f1);
    return !inexact;
}

int POSIT_MKNAME(mpzToMpfr)(mpfr_t out, mpz_t in) {
    // NaR
    if (POSIT_MKNAME(mpzIsNaR)(in)) {
        // we return zeros with the nan flag set
        mpfr_set_ui(out, 0, MPFR_RNDN);
        mpfr_set_nan(out);
        return false;
    }

    // 0
    if (!mpz_cmp_ui(in, 0)) {
        mpfr_set_ui(out, 0, MPFR_RNDN);
        return true;
    }

    mpfr_clear_flags();

    mpz_t i1, i2;
    mpz_init(i1);
    mpz_init(i2);

    mpz_set(i1, in);

    bool neg = mpz_tstbit(in, POSIT_SIGBIT);
    if (neg) { POSIT_MKNAME(mpzNegate)(i1); }

    bool lessThan1 = !mpz_tstbit(i1, POSIT_REGBIT0);

    int guardBitIdx = 0;
    int regimeBits = 0;
    for (guardBitIdx = POSIT_REGBIT0 - 1; guardBitIdx >= 0; guardBitIdx--) {
        if (mpz_tstbit(i1, guardBitIdx) == lessThan1) { break; }
        regimeBits++;
    }

    if (guardBitIdx >= 0) {
        // mask off the regime
        mpz_set_ui(i2, 0);
        mpz_setbit(i2, guardBitIdx);
        mpz_sub_ui(i2, i2, 1);
        mpz_and(i1, i1, i2);

        // but we're going to flag the guard bit itself so that
        // we can find the top of the subexponent
        mpz_setbit(i1, guardBitIdx);
    } else {
        mpz_set_ui(i1, 0);
    }

    mpfr_set_ui(out, 0, MPFR_RNDN);
    while (mpz_cmp_ui(i1, 0)) {
        unsigned long x = mpz_get_ui(i1);
        mpz_sub_ui(i1, i1, x);
        mpz_div_2exp(i1, i1, POSIT_WORD_SZ);

        mpfr_add_ui(out, out, x, MPFR_RNDN);
        mpfr_div_2si(out, out, POSIT_WORD_SZ, MPFR_RNDN);
    }

    // Move the guard bit up to just below the decimal point...
    mpfr_exp_t ex = 0;
    mpfr_frexp(&ex, out, out, MPFR_RNDN);

    // Grab off the subexponent and the guard bit
    mpfr_mul_2si(out, out, POSIT_ES + 1, MPFR_RNDN);

    bool exact = !mpfr_inexflag_p();
    unsigned long subexp = mpfr_get_ui(out, MPFR_RNDZ);
    mpfr_clear_flags(); // inexact, by definition
    mpfr_sub_ui(out, out, subexp, MPFR_RNDN);
    assert(!(subexp >> (1+POSIT_ES)));
    // clear the guard bit
    subexp %= (1<<POSIT_ES);

    // Set the hidden bit
    mpfr_add_ui(out, out, 1, MPFR_RNDN);
    mpfr_div_2si(out, out, 1, MPFR_RNDN);

    // calculate the exponent
    int exponent = 0;
    if (lessThan1) {
        exponent = 1 - ((regimeBits+1)<<POSIT_ES) + (subexp % (1<<POSIT_ES));
    } else {
        exponent = 1 + (regimeBits<<POSIT_ES) + (subexp % (1<<POSIT_ES));
    }

    POSIT_DEBUGF("%Rb, exp= %d  subexp = %d regimeBits = %d \n", out, exponent, subexp, regimeBits);
    assert(!mpfr_set_exp(out, exponent));
    POSIT_DEBUGF("%Rb, exp= %d\n", out, exponent);

    if (neg) { mpfr_neg(out, out, MPFR_RNDN); }

    exact &= !mpfr_inexflag_p();

    mpfr_clear_flags();
    mpz_clear(i1);
    mpz_clear(i2);

    return exact;
}

void POSIT_MKNAME(add_exact_mpfr)(mpz_t sum, mpz_t remainder, mpfr_t x, mpfr_t y) {
    if (!mpfr_number_p(x) || !mpfr_number_p(y)) {
        POSIT_MKNAME(mpzMkNaR)(sum);
        mpz_set_si(remainder, 0);
        return;
    }
    if (mpfr_cmpabs(x, y) < 0) { mpfr_swap(x, y); }
    // from here on in, x is greater than y
    // s = x, r = y
    mpfr_t realSum;
    mpfr_init2(realSum, POSIT_NBITS*2);
    mpfr_clear_flags();
    assert(!mpfr_add(realSum, x, y, MPFR_RNDZ));
    //if (mpfr_flags_
    int inexact = mpfr_inexflag_p();
    mpfr_clear_inexflag();
    assert(!positutil_CHK_FLAGS());
    if (inexact) {
        // If this was rounded, it means there should be no intersecting bits in the
        // bit patterns, we could bail out immediately returning x,y but lets first
        // verify that converting realSum back to a posit yields the same thing as
        // x.

        // 99% of the time this should be inexact, but it's not reliable since the
        // number might have rounded down to something which is in fact perfectly
        // representable.
        POSIT_MKNAME(mpzFromMpfr)(sum, realSum);
        //assert(!mpz_cmp(sum, x));
        // this should obviously never round
        assert(!POSIT_MKNAME(mpzFromMpfr)(remainder, y));
        mpfr_clear(realSum);
        return;
    }

    if (POSIT_MKNAME(mpzFromMpfr)(sum, realSum)) {
        // exact conversion, yay, return sum,0
        mpz_set_si(remainder, 0);
        mpfr_clear(realSum);
        return;
    }

    // we're going to use remainder as a scratch to place x in order to check if it's
    // the same as the sum, in which case no bits intersected.
    assert(POSIT_MKNAME(mpzFromMpfr)(remainder, x));
    if (!mpz_cmp(sum, remainder)) {
        // no bits intersect, the only thing we can do is return x,y
        assert(POSIT_MKNAME(mpzFromMpfr)(remainder, y));
        mpfr_clear(realSum);
        return;
    }

    // Ok, there are bits which intersect between x and y
    // We're going to take the difference between realSum and the representable sum (sum)
    // and that will give us the remainder.

    // Load rf with the representable sum, then subtract it from the real sum which
    // should not have to round
    mpfr_t rf;
    mpfr_init2(rf, POSIT_NBITS);
    POSIT_MKNAME(mpzToMpfr)(rf, sum);
    positutil_CANT_ROUND(mpfr_sub(rf, realSum, rf, MPFR_RNDZ));
    assert(POSIT_MKNAME(mpzFromMpfr)(remainder, rf));

    mpfr_clear(rf);
    mpfr_clear(realSum);
    return;
}


#ifndef POSIT_NOUNDEF
#undef POSIT_MKNAME
#undef POSIT_DEBUGF
#undef POSIT_WORD_SZ
#undef POSIT_SIGBIT
#undef POSIT_REGBIT0
#undef POSIT_GLUE_
#endif
