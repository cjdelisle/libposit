#include "util.h"

#include <assert.h>
#include <stdio.h>

#define MKNAME(x) GLUE4(posit,POSIT_NBITS,_,x)
#define POSIT_T GLUE3(posit,POSIT_NBITS,_t)

#define GLUE4(w,x,y,z) GLUE(GLUE(w,x),GLUE(y,z))
#define GLUE3(x,y,z) GLUE(x,GLUE(y,z))
#define GLUE(x,y) _GLUE(x,y)
#define _GLUE(x,y) x##y

#include "mathhdr.h"

#define VERBOSE 0

#define TEST1(__name__, __impl__) \
static inline void MKNAME(GLUE(__name__, Test))(TestContext_t* ctx) { \
    uint64_t maxVal = ctx->initialVal + ctx->range; \
    printf("Running posit%d_" #__name__ "Test with %llx -> %llx\n", \
        POSIT_NBITS, (long long)ctx->initialVal, (long long)maxVal); \
    for (uint64_t a = ctx->initialVal; a < maxVal; ++a) { \
        POSIT_T pa = MKNAME(reinterpret)(a); \
        MKNAME(toMpfr)(ctx->ma, pa); \
        if (VERBOSE) { mpfr_printf("%s %llx (%.20Rf)\n", #__name__, (long long)a, ctx->ma); } \
        POSIT_T pc; \
        __impl__; \
        POSIT_T pref = MKNAME(fromMpfr)(ctx->mref); \
        if (MKNAME(cmp)(pref, pc)) { \
            char buf[4][MKNAME(debug_SIZE)]; \
            MKNAME(toMpfr)(ctx->mc, pc); \
            mpfr_printf("FAIL: %s(%s) produced %s instead of %s " \
                "(%.20Rf -> %.20Rf instead of %.20Rf)\n", #__name__, \
                MKNAME(debug)(buf[0], pa), \
                MKNAME(debug)(buf[1], pc), MKNAME(debug)(buf[2], pref), \
                ctx->ma, ctx->mc, ctx->mref); \
            assert(0); \
        } \
    } \
}

#define TEST2(__name__, __impl__) \
static inline void MKNAME(GLUE(__name__, Test))(TestContext_t* ctx) { \
    uint64_t maxVal = ctx->initialVal + ctx->range; \
    uint64_t maxValB = ctx->initialValB + ctx->rangeB; \
    printf("Running posit%d_" #__name__ "Test with %llx -> %llx and %llx -> %llx\n", \
        POSIT_NBITS, (long long)ctx->initialVal, (long long)maxVal, \
        (long long)ctx->initialValB, (long long)maxValB); \
    for (uint64_t a = ctx->initialVal; a < maxVal; ++a) { \
        POSIT_T pa = MKNAME(reinterpret)(a); \
        for (uint64_t b = ctx->initialValB; b < maxValB; ++b) { \
            if (VERBOSE) { printf("%llx and %llx\n", (long long)a, (long long)b); } \
            POSIT_T pb = MKNAME(reinterpret)(b); \
            MKNAME(toMpfr)(ctx->ma, pa); \
            MKNAME(toMpfr)(ctx->mb, pb); \
            POSIT_T pc; \
            __impl__; \
            POSIT_T pref = MKNAME(fromMpfr)(ctx->mref); \
            if (MKNAME(cmp)(pref, pc)) { \
                char buf[4][MKNAME(debug_SIZE)]; \
                printf("FAIL: %s(%s, %s) produced %s instead of %s\n", #__name__, \
                    MKNAME(debug)(buf[0], pa), MKNAME(debug)(buf[1], pb), \
                    MKNAME(debug)(buf[2], pc), MKNAME(debug)(buf[3], pref)); \
                assert(0); \
            } \
        } \
    } \
}

/*
static inline void MKNAME(assert_equals)(POSIT_T x, POSIT_T y,
    const char* op, POSIT_T arg1, POSIT_T arg2)
{
    if (x.v == y.v) { return; }
    char buf[4][MKNAME(debug_SIZE)];
    printf("FAIL: %s != %s when computing %s(%s,%s)\n",
        MKNAME(debug)(buf[0], x), MKNAME(debug)(buf[1], y),
        op, MKNAME(debug)(buf[2], x), MKNAME(debug)(buf[3], y));
    assert(0);
}*/

TEST2(add, {
    pc = MKNAME(add_all)(pa, pb);
    if (mpfr_nan_p(ctx->ma) || mpfr_nan_p(ctx->mb)) {
        // nan + anything -> nan
        // anything + nan -> nan
        mpfr_set_nan(ctx->mref);
    } else if (!mpfr_cmpabs(ctx->ma, ctx->mb) && mpfr_cmp(ctx->ma, ctx->mb)) {
        // anything + -self -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else if (mpfr_zero_p(ctx->ma) && mpfr_zero_p(ctx->mb)) {
        // 0 + 0 -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else {
        //mpfr_printf("%.20Rf + %.60Rf\n", ctx->ma, ctx->mb);
        if (POSIT_NBITS < 32) {
            util_CANT_ROUND(mpfr_add(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ));
        } else {
            mpfr_add(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ);
        }
        assert(mpfr_regular_p(ctx->mref));
    }
})

TEST2(sub, {
    pc = MKNAME(sub_all)(pa, pb);
    if (mpfr_nan_p(ctx->ma) || mpfr_nan_p(ctx->mb)) {
        // nan - anything -> nan
        // anything - nan -> nan
        mpfr_set_nan(ctx->mref);
    } else if (!mpfr_cmp(ctx->ma, ctx->mb)) {
        // anything - self -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else if (mpfr_zero_p(ctx->ma) && mpfr_zero_p(ctx->mb)) {
        // 0 - 0 -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else {
        if (POSIT_NBITS < 32) {
            util_CANT_ROUND(mpfr_sub(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ));
        } else {
            mpfr_sub(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ);
        }
        assert(mpfr_regular_p(ctx->mref));
    }
})

TEST2(mul, {
    pc = MKNAME(mul_all)(pa, pb);
    if (mpfr_nan_p(ctx->ma) || mpfr_nan_p(ctx->mb)) {
        // nan * anything -> nan
        // anything * nan -> nan
        mpfr_set_nan(ctx->mref);
    } else {
        util_CANT_ROUND(mpfr_mul(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ));
    }
})

TEST2(div, {
    pc = MKNAME(div_all)(pa, pb);
    if (mpfr_nan_p(ctx->ma) || mpfr_nan_p(ctx->mb)) {
        // nan / anything -> nan
        // anything / nan -> nan
        mpfr_set_nan(ctx->mref);
    } else if (mpfr_zero_p(ctx->mb)) {
        // anything / 0 -> nan
        mpfr_set_nan(ctx->mref);
    } else if (mpfr_zero_p(ctx->ma)) {
        // 0 / anything -> zero
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else {
        // numbers such as 2/3 will round, no matter how much precision we have
        mpfr_div(ctx->mref, ctx->ma, ctx->mb, MPFR_RNDZ);
        // any special cases should be handled, everything will be a regular number
        assert(mpfr_regular_p(ctx->mref));
    }
})



TEST1(sqrt, {
    pc = MKNAME(sqrt_all)(pa);
    if (mpfr_nan_p(ctx->ma)) {
        // sqrt(nan) -> nan
        mpfr_set_nan(ctx->mref);
    } else if (mpfr_zero_p(ctx->ma)) {
        // sqrt(0) -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else if (mpfr_cmp_si(ctx->ma, 0) <= 0) {
        // sqrt(anything negative) -> nan
        mpfr_set_nan(ctx->mref);
    } else {
        mpfr_sqrt(ctx->mref, ctx->ma, MPFR_RNDZ);
        // all special cases should be handled above
        assert(mpfr_regular_p(ctx->mref));
    }
})

TEST1(log, {
    pc = MKNAME(log_all)(pa);
    if (mpfr_nan_p(ctx->ma)) {
        // log(nan) -> nan
        mpfr_set_nan(ctx->mref);
    } else if (!mpfr_cmp_si(ctx->ma, 1)) {
        // log(0) -> 0
        mpfr_set_zero(ctx->mref, MPFR_RNDZ);
    } else if (mpfr_cmp_si(ctx->ma, 0) <= 0) {
        // log(anything negative) -> nan
        mpfr_set_nan(ctx->mref);
    } else {
        mpfr_log(ctx->mref, ctx->ma, MPFR_RNDZ);
        // all special cases should be handled above
        assert(mpfr_regular_p(ctx->mref));
    }
})

TEST1(exp, {
    pc = MKNAME(exp_all)(pa);
    if (mpfr_nan_p(ctx->ma)) {
        // exp(nan) -> nan
        mpfr_set_nan(ctx->mref);
    } else {
        mpfr_exp(ctx->mref, ctx->ma, MPFR_RNDZ);
        if (mpfr_zero_p(ctx->mref)) {
            mpfr_set_ui(ctx->mref, 1, MPFR_RNDN);
            mpfr_div_2si(ctx->mref, ctx->mref, 2 * MKNAME(LOG2_MAXVAL), MPFR_RNDN);
        }
        assert(mpfr_regular_p(ctx->mref));
    }
})


static inline void MKNAME(tests)(TestContext_t* ctx) {
    uint64_t maxVal = ctx->initialVal + ctx->range;
    uint64_t maxValB = ctx->initialValB + ctx->rangeB;
    printf("\nRunning posit%d_tests with %llx -> %llx and %llx -> %llx\n",
        POSIT_NBITS, (long long)ctx->initialVal, (long long)maxVal,
        (long long)ctx->initialValB, (long long)maxValB);

    MKNAME(addTest)(ctx);
    MKNAME(subTest)(ctx);
    MKNAME(mulTest)(ctx);
    MKNAME(divTest)(ctx);

    MKNAME(sqrtTest)(ctx);
    MKNAME(logTest)(ctx);
    MKNAME(expTest)(ctx);
}
