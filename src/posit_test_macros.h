#ifndef POSIT_TEST_H
#define POSIT_TEST_H
    #include <mpfr.h>
    #include <gmp.h>
    typedef struct {
        mpz_t i1;
        mpfr_t f1;
    } PositTest_t;
    static inline void PositTest_init(PositTest_t* pt) {
        mpz_init(pt->i1);
        mpfr_init2(pt->f1, 256);
    }
    static inline void PositTest_clear(PositTest_t* pt) {
        mpz_clear(pt->i1);
        mpfr_clear(pt->f1);
    }
#endif

#define POSIT_NOUNDEF
#include "posit_macros_impl.h"
#undef POSIT_NOUNDEF

static inline void POSIT_MKNAME(mkOne)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_NBITS - 2);
}
static inline void POSIT_MKNAME(mkOneNeg)(mpz_t p) {
    POSIT_MKNAME(mkOne)(p);
    mpz_setbit(p, POSIT_SIGBIT);
}
static inline void POSIT_MKNAME(mkMax)(mpz_t p) {
    mpz_set_ui(p, 0);
    mpz_setbit(p, POSIT_SIGBIT);
    mpz_sub_ui(p, p, 1);
}
static inline bool POSIT_MKNAME(isMax)(mpz_t p) {
    if (mpz_tstbit(p, POSIT_SIGBIT)) { return false; }
    return mpz_popcount(p) == POSIT_NBITS - 1;
}
static inline void POSIT_MKNAME(mkMaxNeg)(mpz_t p) {
    mpz_set_ui(p, 1);
    mpz_setbit(p, POSIT_SIGBIT);
}
static inline void POSIT_MKNAME(mkMin)(mpz_t p) {
    mpz_set_ui(p, 1);
}
static inline void POSIT_MKNAME(mkMinNeg)(mpz_t p) {
    POSIT_MKNAME(mkOnes)(p);
}

static inline void POSIT_MKNAME(testOne)(
    PositTest_t* ctx,
    const char* name,
    mpz_t expect,
    mpz_t res,
    mpfr_t m,
    bool shouldBeExact)
{
    if (name) {
        mpfr_printf("\n%s (%s)\n%Rf -> ", name, shouldBeExact ? "exact" : "inexact", m);
    }
    gmp_printf("%Zx == ", expect);
    bool isExact = POSIT_MKNAME(mpzFromMpfr)(res, m);
    gmp_printf("%Zx (ex: %d)\n", res, mpfr_get_exp(m));

    POSIT_MKNAME(mpzToMpfr)(ctx->f1, res);
    mpfr_printf("toMpfr(%Zx) -> %.40Rf\n", res, ctx->f1);

    assert(!mpz_cmp(expect, res));
    //assert(isExact == shouldBeExact);

    if (isExact) {
        assert(!mpfr_cmp(ctx->f1, m));
    }
    mpfr_set(m, ctx->f1, MPFR_RNDN);
    mpfr_printf("%Rf: ", m);
    gmp_printf("%Zx == ", expect);
    isExact = POSIT_MKNAME(mpzFromMpfr)(res, m);
    gmp_printf("%Zx\n", res);
    assert(!mpz_cmp(expect, res));
    if (!isExact) {
        POSIT_MKNAME(mkNaR)(res);
        if (mpz_cmp(res, expect)) {
            mpfr_printf("Inexact serialization of [%.40Rf]\n", m);
            //assert(!mpz_cmp(res, expect) && "Inexact serialization of posit other than nan");
        }
    }
}

// direction:
//   -1 = almost
//   0 = exact
//   1 = past
//   2 = way past
static inline void POSIT_MKNAME(testLimit)(
    PositTest_t* ctx,
    mpz_t ex, mpz_t res,
    mpfr_t m,
    bool min, bool neg, int direction
) {
    mpfr_set_si(m, 1, MPFR_RNDN);
    mpfr_mul_2si(m, m, POSIT_LOG2_MAXVAL, MPFR_RNDN);
    mpfr_add_si(m, m, (min) ? -direction : direction, MPFR_RNDN);
    if (min) { mpfr_div_2si(m, m, (2 + (direction == 2))*POSIT_LOG2_MAXVAL, MPFR_RNDN); }
    if (neg) { mpfr_neg(m, m, MPFR_RNDN); }

    const char* dir = (
        (direction == -1) ? "Almost" : (
            (direction == 0) ?  "Exact" : (
                (direction == 1) ?  "Past" : (
                    (direction == 2) ?  "Way past" : "wtf"))));
    const char* n = (neg) ? "negative" : "positive";
    const char* minmax = (min) ? "minpos" : "maxpos";

    mpfr_printf("\n%s %s %s \n%.40Rf -> ", dir, n, minmax, m);
    POSIT_MKNAME(testOne)(ctx, NULL, ex, res, m, (direction == 0));

    if (direction < 2) {
        POSIT_MKNAME(testLimit)(ctx, ex, res, m, min, neg, direction+1);
    }
}

static inline void POSIT_MKNAME(selfTest)(PositTest_t* ctx) {
    mpfr_t m;
    mpfr_init2(m, 256);

    mpz_t expect, res;
    mpz_init(expect);
    mpz_init(res);

    mpfr_printf("\n\n=== Testing Posit%d ===\n\n", POSIT_NBITS);

    mpfr_set_nan(m);
    POSIT_MKNAME(mkNaR)(expect);
    POSIT_MKNAME(testOne)(ctx, "NaR Test", expect, res, m, false);

    mpfr_set_inf(m, 1);
    POSIT_MKNAME(mkNaR)(expect);
    POSIT_MKNAME(testOne)(ctx, "Inf Test", expect, res, m, false);

    mpfr_set_inf(m, -1);
    POSIT_MKNAME(mkNaR)(expect);
    POSIT_MKNAME(testOne)(ctx, "Negative Inf Test", expect, res, m, false);

    // 0
    mpfr_set_si(m, 0, MPFR_RNDN);
    mpz_set_ui(expect, 0);
    POSIT_MKNAME(testOne)(ctx, "Zero Test", expect, res, m, true);

    // 1
    mpfr_set_si(m, 1, MPFR_RNDN);
    POSIT_MKNAME(mkOne)(expect);
    POSIT_MKNAME(testOne)(ctx, "One Test", expect, res, m, true);

    // -1
    mpfr_set_si(m, -1, MPFR_RNDN);
    POSIT_MKNAME(mkOneNeg)(expect);
    POSIT_MKNAME(testOne)(ctx, "Negative One Test", expect, res, m, true);

    // POS MAX
    POSIT_MKNAME(mkMax)(expect);
    POSIT_MKNAME(testLimit)(ctx, expect, res, m, false, false, -1);

    POSIT_MKNAME(mkMaxNeg)(expect);
    POSIT_MKNAME(testLimit)(ctx, expect, res, m, false, true, -1);

    POSIT_MKNAME(mkMin)(expect);
    POSIT_MKNAME(testLimit)(ctx, expect, res, m, true, false, -1);

    POSIT_MKNAME(mkMinNeg)(expect);
    POSIT_MKNAME(testLimit)(ctx, expect, res, m, true, true, -1);

    mpfr_clear(m);
    mpz_clear(expect);
    mpz_clear(res);
}

static void POSIT_MKNAME(testToMpfr)(PositTest_t* ctx, mpfr_t out, mpz_t in) {
    POSIT_MKNAME(mpzToMpfr)(out, in);
    int exact = POSIT_MKNAME(mpzFromMpfr)(ctx->i1, out);
    if (POSIT_MKNAME(isNaR)(in)) {
        assert(!exact);
        exact = true;
    }
    if (!exact || mpz_cmp(ctx->i1, in)) {
        mpfr_printf("Roundtrip %Zx toMpfr -> %.40Rf fromMpfr -> %Zx\n", in, out, ctx->i1);
        assert(0);
    }
    #if POSIT_NBITS < 64
        POSIT_GLUE(POSIT_GLUE(posit, POSIT_NBITS), _t) softPosit;
        softPosit.v = mpz_get_ui(in);
        double d = POSIT_GLUE(POSIT_GLUE(convertP, POSIT_NBITS), ToDouble)(softPosit);
        assert(!mpfr_cmp_d(out, d));
    #endif
}

// output the value which is halfway between in and in.nextval() where nextval
// always goes toward infinity. Result is a posit of double the nbits but same es.
static void POSIT_MKNAME(testGetMid)(PositTest_t* ctx, mpfr_t out, mpz_t in) {
    mpz_mul_2exp(ctx->i1, in, 1);
    mpz_add_ui(ctx->i1, ctx->i1, 1);
    mpz_mul_2exp(ctx->i1, ctx->i1, POSIT_NBITS-1);
    POSIT_NEXT_TO_MPFR(out, ctx->i1);
}

static bool POSIT_MKNAME(testFromMpfr)(PositTest_t* ctx, mpz_t out, mpfr_t in) {
    int exact = POSIT_MKNAME(mpzFromMpfr)(out, in);

    #if POSIT_NBITS < 64
        double asD = mpfr_get_d(in, MPFR_RNDN);
        mpfr_set_d(ctx->f1, asD, MPFR_RNDN);
        if (!mpfr_cmp(ctx->f1, in)) {
            POSIT_GLUE(POSIT_GLUE(posit, POSIT_NBITS), _t) softPosit =
                POSIT_GLUE(convertDoubleToP, POSIT_NBITS)(asD);
            assert(!mpz_cmp_ui(out, softPosit.v));
        }
    #endif

    if (exact) {
        POSIT_MKNAME(mpzToMpfr)(ctx->f1, out);
        assert(!mpfr_cmp(ctx->f1, in));
    }

    return exact;
}

#define POSIT_UNDEF
#include "slowposit/posit_macros.h"
#undef POSIT_UNDEF
