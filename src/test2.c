#include "SoftPosit/source/include/softposit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// We need to use names like Posit16e0_t rather than Posit16_t because
// we will have Posit16e0_t and also Posit16e1_t
#define POSIT_MULTI_ES

// We will be using mpzToMpfr
#define POSIT_WITH_MPFR

#define POSIT_NBITS 16
#define POSIT_ES 0
#include "posit_macros_impl.h"
#undef POSIT_NBITS
#undef POSIT_ES
#define POSIT_NEXT_TO_MPFR Posit16e0_mpzToMpfr
#define POSIT_NBITS 8
#define POSIT_ES 0
#include "posit_test_macros.h"
#undef POSIT_NEXT_TO_MPFR
#undef POSIT_NBITS
#undef POSIT_ES


#define POSIT_NBITS 32
#define POSIT_ES 1
#include "posit_macros_impl.h"
#undef POSIT_NBITS
#undef POSIT_ES
#define POSIT_NEXT_TO_MPFR Posit32e1_mpzToMpfr
#define POSIT_NBITS 16
#define POSIT_ES 1
#include "posit_test_macros.h"
#undef POSIT_NEXT_TO_MPFR
#undef POSIT_NBITS
#undef POSIT_ES


#define POSIT_NBITS 64
#define POSIT_ES 2
#include "posit_macros_impl.h"
#undef POSIT_NBITS
#undef POSIT_ES
#define POSIT_NEXT_TO_MPFR Posit64e2_mpzToMpfr
#define POSIT_NBITS 32
#define POSIT_ES 2
#include "posit_test_macros.h"
#undef POSIT_NEXT_TO_MPFR
#undef POSIT_NBITS
#undef POSIT_ES


#define POSIT_NBITS 128
#define POSIT_ES 3
#include "posit_macros_impl.h"
#undef POSIT_NBITS
#undef POSIT_ES
#define POSIT_NEXT_TO_MPFR Posit128e3_mpzToMpfr
#define POSIT_NBITS 64
#define POSIT_ES 3
#include "posit_test_macros.h"
#undef POSIT_NEXT_TO_MPFR
#undef POSIT_NBITS
#undef POSIT_ES


#define POSIT_NBITS 256
#define POSIT_ES 4
#include "posit_macros_impl.h"
#undef POSIT_NBITS
#undef POSIT_ES
#define POSIT_NEXT_TO_MPFR Posit256e4_mpzToMpfr
#define POSIT_NBITS 128
#define POSIT_ES 4
#include "posit_test_macros.h"
#undef POSIT_NEXT_TO_MPFR
#undef POSIT_NBITS
#undef POSIT_ES

#define _EXP__64(x)  (0.0 + (((uint64_t)1)<<(x)))
#define _EXP_128(x)  ((x) >=  64 ? (_EXP__64((x)- 64) * 1.8446744073709552e+19) : _EXP__64(x))
#define _EXP_256(x)  ((x) >= 128 ? (_EXP_128((x)-128) * 3.4028236692093846e+38) : _EXP_128(x))
#define _EXP_512(x)  ((x) >= 256 ? (_EXP_256((x)-256) * 1.1579208923731620e+77) : _EXP_256(x))
#define _EXP_1024(x) ((x) >= 512 ? (_EXP_512((x)-512) * 1.3407807929942597e+154) : _EXP_512(x))
#define POSIT_EXP(x) _EXP_1024(x)


#define TEST(n,p,s) do { \
    (n)(p); \
    char* str = mpz_get_str(NULL, 16, (p)); \
    if (strcmp(str, (s))) { \
        printf("Assertion failed [%s] != [%s]", str, (s)); \
        assert(0); \
    } \
    free(str); \
} while (0)

#define PRECISION 256

typedef void (* TestToMpfr)(PositTest_t* ctx, mpfr_t out, mpz_t in);
typedef bool (* TestFromMpfr)(PositTest_t* ctx, mpz_t out, mpfr_t in);
typedef void (* TestGetMid)(PositTest_t* ctx, mpfr_t out, mpz_t in);
typedef struct {
    mpz_t rangeEnd;
    mpz_t i0;
    mpz_t i1;

    mpfr_t f0;
    mpfr_t f1;

    mpfr_t reallySmallValue;

    mpfr_t middle;
    mpz_t roundchk;

    PositTest_t pt;
} RoundingTestCtx_t;
static void roundingTestInit(RoundingTestCtx_t* fc) {
    PositTest_init(&fc->pt);
    mpz_init(fc->rangeEnd);
    mpz_init(fc->i0);
    mpz_init(fc->i1);
    mpz_init(fc->roundchk);

    mpfr_init2(fc->f0, PRECISION);
    mpfr_init2(fc->f1, PRECISION);
    mpfr_init2(fc->middle, PRECISION);
    mpfr_init2(fc->reallySmallValue, PRECISION);

    mpfr_set_ui_2exp(fc->reallySmallValue, 1, -65535, MPFR_RNDU);
    //mpfr_printf("Really small value = %Re\n", fc->reallySmallValue);
}
static void roundingTestClear(RoundingTestCtx_t* fc) {
    PositTest_clear(&fc->pt);
    mpz_clear(fc->rangeEnd);
    mpz_clear(fc->i0);
    mpz_clear(fc->i1);
    mpz_clear(fc->roundchk);

    mpfr_clear(fc->f0);
    mpfr_clear(fc->f1);
    mpfr_clear(fc->middle);
}

static bool isNaR(mpz_t x, int width) {
    return mpz_popcount(x) == 1 && mpz_tstbit(x, width-1) == 1;
}

static void roundingTest(
    RoundingTestCtx_t* fc,
    const char* startHex,
    const char* endHex,
    TestToMpfr toMpfr,
    TestFromMpfr fromMpfr,
    TestGetMid getMid,
    int width, int es
) {
    printf("\n\nROUNDING TEST Posit%d range %s - %s\n", width, startHex, endHex);
    mpz_set_str(fc->i0, startHex, 16);
    mpz_set_str(fc->rangeEnd, endHex, 16);
    toMpfr(&fc->pt, fc->f0, fc->i0);
    while (mpz_cmp(fc->i0, fc->rangeEnd)) {
        //toMpfr(fctx, fc->f0, fc->i0);
        bool exact = fromMpfr(&fc->pt, fc->i1, fc->f0);

        mpz_add_ui(fc->i1, fc->i0, 1);

        // next posit
        toMpfr(&fc->pt, fc->f1, fc->i1);

        // halfway between current posit and next posit
        getMid(&fc->pt, fc->middle, fc->i0);

        fromMpfr(&fc->pt, fc->roundchk, fc->middle);

        if (!mpz_cmp_ui(fc->i0, 0)) {
            // never round to zero
            if (mpz_cmp(fc->roundchk, fc->i1)) {
                mpfr_printf("Rounded to 0: %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
        } else if (isNaR(fc->i1, width)) {
            // never round to NaR
            if (mpz_cmp(fc->roundchk, fc->i0)) {
                mpfr_printf("Rounded up to NaR: %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
        } else if (isNaR(fc->i0, width)) {
            // never round to NaR
            if (mpz_cmp(fc->roundchk, fc->i1)) {
                mpfr_printf("Rounded down to NaR: %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
        } else if (mpz_tstbit(fc->i0, 0)) {
            // odd, middle should round up
            if (mpz_cmp(fc->roundchk, fc->i1)) {
                mpfr_printf("Should have rounded up %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
            mpfr_sub(fc->middle, fc->middle, fc->reallySmallValue, MPFR_RNDD);
            fromMpfr(&fc->pt, fc->roundchk, fc->middle);
            if (mpz_cmp(fc->roundchk, fc->i0)) {
                mpfr_printf("Should have rounded down %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
        } else {
            if (mpz_cmp(fc->roundchk, fc->i0)) {
                mpfr_printf("Should have rounded down %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
            mpfr_add(fc->middle, fc->middle, fc->reallySmallValue, MPFR_RNDU);
            fromMpfr(&fc->pt, fc->roundchk, fc->middle);
            if (mpz_cmp(fc->roundchk, fc->i1)) {
                mpfr_printf("Should have rounded up %Zx - %Zx, middle point %.40Rf rounds to %Zx\n",
                    fc->i0, fc->i1, fc->middle, fc->roundchk);
                assert(0);
            }
        }

        mpz_swap(fc->i0, fc->i1);
        mpfr_swap(fc->f0, fc->f1);
    }
}
#define ROUNDING_TEST(ctx, width, es, startHex, endHex) \
    roundingTest((ctx), (startHex), (endHex), \
        Posit ## width ## e ## es ## _testToMpfr, \
        Posit ## width ## e ## es ## _testFromMpfr, \
        Posit ## width ## e ## es ## _testGetMid, \
        width, es)

#define ROUNDING_TEST_2()

int main() {
    mpz_t p; mpz_init(p);
    RoundingTestCtx_t fc;
    roundingTestInit(&fc);

    TEST(Posit8e0_mkNaR,p,   "80");
    TEST(Posit8e0_mkOne,p,   "40");
    TEST(Posit8e0_mkOneNeg,p,"c0");
    TEST(Posit8e0_mkMax,p,   "7f");
    TEST(Posit8e0_mkMaxNeg,p,"81");
    TEST(Posit8e0_mkMin,p,    "1");
    TEST(Posit8e0_mkMinNeg,p,"ff");
    assert(Posit8e0_LOG2_MAXVAL ==                6); // math.log2( ((2**2**0)**(8-2)) )
    assert(POSIT_EXP(Posit8e0_LOG2_MAXVAL) ==     64.0);
    assert(1/POSIT_EXP(Posit8e0_LOG2_MAXVAL) ==   0.015625);
    Posit8e0_selfTest(&fc.pt);
    // exhaustive test except skip 00 and 80
    ROUNDING_TEST(&fc, 8, 0, "00", "ff");

    TEST(Posit16e1_mkNaR,p,   "8000");
    TEST(Posit16e1_mkOne,p,   "4000");
    TEST(Posit16e1_mkOneNeg,p,"c000");
    TEST(Posit16e1_mkMax,p,   "7fff");
    TEST(Posit16e1_mkMaxNeg,p,"8001");
    TEST(Posit16e1_mkMin,p,      "1");
    TEST(Posit16e1_mkMinNeg,p,"ffff");
    assert(Posit16e1_LOG2_MAXVAL ==                28); // math.log2( ((2**2**1)**(16-2)) )
    assert(POSIT_EXP(Posit16e1_LOG2_MAXVAL) ==     268435456.0);
    assert(1/POSIT_EXP(Posit16e1_LOG2_MAXVAL) ==   3.725290298461914e-9);
    Posit16e1_selfTest(&fc.pt);
    // exhaustive test for posit16 because it's pretty quick
    ROUNDING_TEST(&fc, 16, 1, "0000", "ffff");

    TEST(Posit32e2_mkNaR,p,   "80000000");
    TEST(Posit32e2_mkOne,p,   "40000000");
    TEST(Posit32e2_mkOneNeg,p,"c0000000");
    TEST(Posit32e2_mkMax,p,   "7fffffff");
    TEST(Posit32e2_mkMaxNeg,p,"80000001");
    TEST(Posit32e2_mkMin,p,          "1");
    TEST(Posit32e2_mkMinNeg,p,"ffffffff");
    assert(Posit32e2_LOG2_MAXVAL ==                120); // math.log2( ((2**2**2)**(32-2)) )
    assert(POSIT_EXP(Posit32e2_LOG2_MAXVAL) ==     1.329227995784916e+36);
    assert(1/POSIT_EXP(Posit32e2_LOG2_MAXVAL) ==   7.52316384526264e-37);
    Posit32e2_selfTest(&fc.pt);
    // only test 64 edge cases
    ROUNDING_TEST(&fc, 32, 2, "00000000", "0000000f");
    ROUNDING_TEST(&fc, 32, 2, "7ffffff0", "8000000f");
    ROUNDING_TEST(&fc, 32, 2, "fffffff0", "ffffffff");

    TEST(Posit64e3_mkNaR,p,   "8000000000000000");
    TEST(Posit64e3_mkOne,p,   "4000000000000000");
    TEST(Posit64e3_mkOneNeg,p,"c000000000000000");
    TEST(Posit64e3_mkMax,p,   "7fffffffffffffff");
    TEST(Posit64e3_mkMaxNeg,p,"8000000000000001");
    TEST(Posit64e3_mkMin,p,                  "1");
    TEST(Posit64e3_mkMinNeg,p,"ffffffffffffffff");
    assert(Posit64e3_LOG2_MAXVAL ==                496); // math.log2( ((2**2**3)**(64-2)) )
    Posit64e3_selfTest(&fc.pt);
    // only test 64 edge cases
    ROUNDING_TEST(&fc, 64, 3, "0000000000000000", "000000000000000f");
    ROUNDING_TEST(&fc, 64, 3, "7ffffffffffffff0", "800000000000000f");
    ROUNDING_TEST(&fc, 64, 3, "fffffffffffffff0", "ffffffffffffffff");


    TEST(Posit128e4_mkNaR,p,   "80000000000000000000000000000000");
    TEST(Posit128e4_mkOne,p,   "40000000000000000000000000000000");
    TEST(Posit128e4_mkOneNeg,p,"c0000000000000000000000000000000");
    TEST(Posit128e4_mkMax,p,   "7fffffffffffffffffffffffffffffff");
    TEST(Posit128e4_mkMaxNeg,p,"80000000000000000000000000000001");
    TEST(Posit128e4_mkMin,p,                                  "1");
    TEST(Posit128e4_mkMinNeg,p,"ffffffffffffffffffffffffffffffff");
    assert(Posit128e4_LOG2_MAXVAL ==               2016); // math.log2( ((2**2**4)**(128-2)) )
    Posit128e4_selfTest(&fc.pt);
    // 128 bit only testing edge cases
    ROUNDING_TEST(&fc, 128, 4, "00000000000000000000000000000000", "0000000000000000000000000000000f");
    ROUNDING_TEST(&fc, 128, 4, "7ffffffffffffffffffffffffffffff0", "8000000000000000000000000000000f");
    ROUNDING_TEST(&fc, 128, 4, "fffffffffffffffffffffffffffffff0", "ffffffffffffffffffffffffffffffff");

    roundingTestClear(&fc);
    mpz_clear(p);
}
