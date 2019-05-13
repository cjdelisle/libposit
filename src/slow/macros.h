#ifndef POSIT_UNDEF

#define posit_WITH_MPFR
#include "posit.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define POSIT_CANT_ROUND(expr) do {\
    mpfr_clear_flags(); \
    assert(!(expr)); \
    assert(!mpfr_flags_save() && #expr "should not round"); \
} while (0)

#define POSIT_MKNAME(name) POSIT_GLUE6(posit,POSIT_NBITS,_,name,_,POSIT_IMPL)
#define POSIT_T POSIT_GLUE3(posit,POSIT_NBITS,_t)
#define POSIT_X2_T POSIT_GLUE3(posit,POSIT_NBITS,x2_t)

#define POSIT_GLUE6(a,b,c,d,e,f) POSIT_GLUE3(POSIT_GLUE4(a,b,c,d),e,f)
#define POSIT_GLUE4(a,b,c,d) POSIT_GLUE(POSIT_GLUE(a,b),POSIT_GLUE(c,d))
#define POSIT_GLUE3(a,b,c) POSIT_GLUE(POSIT_GLUE(a,b),c)
#define POSIT_GLUE(a,b) POSIT_GLUE_(a,b)
#define POSIT_GLUE_(a,b) a ## b

typedef struct POSIT_MKNAME(Context_s) {
    mpfr_t fx;
    mpfr_t fy;
    mpz_t i;
} POSIT_MKNAME(Context_t);

static void POSIT_MKNAME(init)(POSIT_MKNAME(Context_t)* ctx, int bits) {
    mpfr_init2(ctx->fx, bits);
    mpfr_init2(ctx->fy, bits);
    mpz_init(ctx->i);
}
static void POSIT_MKNAME(fini)(POSIT_MKNAME(Context_t)* ctx) {
    mpfr_clear(ctx->fx);
    mpfr_clear(ctx->fy);
    mpz_clear(ctx->i);
}

POSIT_T POSIT_MKNAME(fromMpz)(const mpz_t in) {
    uint8_t buf[192];
    _Static_assert(sizeof(POSIT_T) < 96, "");
    assert((mpz_sizeinbase(in, 16) + 2) < 192);
    mpz_get_str((char*)buf, 16, in);
    union {
        POSIT_T p;
        uint8_t b[POSIT_NBITS / 8];
    } out;
    const uint8_t numForAscii[] = {
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9,99,99,99,99,99,99,
        99,10,11,12,13,14,15,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,10,11,12,13,14,15,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    };
    int i = 0;
    int j = strlen(buf) - 2;
    while (j >= 0) {
        assert(i < (int)sizeof(POSIT_T));
        out.b[i] = (numForAscii[buf[j]] << 4) | numForAscii[buf[j+1]];
        j -= 2;
        i++;
    }
    if (j == -1) {
        assert(i < (int)sizeof(POSIT_T));
        out.b[i] = numForAscii[buf[0]];
        i++;
    }
    for (; i < (int)sizeof(POSIT_T); i++) { out.b[i] = 0; }
    return out.p;
}

void POSIT_MKNAME(toMpz)(mpz_t out, POSIT_T in) {
    char buf[192];
    _Static_assert(sizeof(POSIT_T) < 96, "");
    union {
        POSIT_T p;
        uint8_t b[POSIT_NBITS / 8];
    } x = { .p = in };
    const char* asciiNum = "0123456789abcdef";
    int j = 0;
    for (int i = (sizeof x) - 1; i >= 0; i--, j += 2) {
        buf[j] = asciiNum[x.b[i] >> 4];
        buf[j+1] = asciiNum[x.b[i] & 0x0f];
    }
    buf[j] = '\0';
    assert(!mpz_set_str(out, buf, 16));
}

POSIT_T POSIT_MKNAME(_fromMpfr)(mpz_t scratch, mpfr_t in) {
    POSIT_MKNAME(mpzFromMpfr)(scratch, in);
    POSIT_T out = POSIT_MKNAME(fromMpz)(scratch);
    return out;
}
POSIT_T POSIT_MKNAME(fromMpfr)(mpfr_t in) {
    mpz_t mpz;
    mpz_init(mpz);
    POSIT_T out = POSIT_MKNAME(_fromMpfr)(mpz, in);
    mpz_clear(mpz);
    return out;
}

void POSIT_MKNAME(_toMpfr)(mpz_t scratch, mpfr_t out, POSIT_T in) {
    POSIT_MKNAME(toMpz)(scratch, in);
    POSIT_MKNAME(mpzToMpfr)(out, scratch);
}
void POSIT_MKNAME(toMpfr)(mpfr_t out, POSIT_T in) {
    mpz_t mpz;
    mpz_init(mpz);
    POSIT_MKNAME(_toMpfr)(mpz, out, in);
    mpz_clear(mpz);
}

#define POSIT_MPFR_1FUNC(__rett__, __name__, ...) \
    __rett__ POSIT_MKNAME(__name__)(POSIT_T x) { \
        POSIT_MKNAME(Context_t) ctx; \
        POSIT_MKNAME(init)(&ctx, 2*POSIT_NBITS); \
        POSIT_MKNAME(_toMpfr)(ctx.i, ctx.fx, x); \
        __rett__ out; \
        __VA_ARGS__; \
        POSIT_MKNAME(fini)(&ctx); \
        return out; \
    }

#define POSIT_MPFR_FUNC(__rett__, __name__, ...) \
    __rett__ POSIT_MKNAME(__name__)(POSIT_T x, POSIT_T y) { \
        POSIT_MKNAME(Context_t) ctx; \
        POSIT_MKNAME(init)(&ctx, 2*POSIT_NBITS); \
        POSIT_MKNAME(_toMpfr)(ctx.i, ctx.fx, x); \
        POSIT_MKNAME(_toMpfr)(ctx.i, ctx.fy, y); \
        __rett__ out; \
        __VA_ARGS__; \
        POSIT_MKNAME(fini)(&ctx); \
        return out; \
    }

#ifndef POSIT_LIMITED

POSIT_MPFR_FUNC(POSIT_T, add, {
    mpfr_add(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_FUNC(POSIT_T, sub, {
    mpfr_sub(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_FUNC(POSIT_T, mul, {
    mpfr_mul(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_FUNC(POSIT_T, div, {
    mpfr_div(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_FUNC(int, cmp, {
    if (mpfr_nan_p(ctx.fx)) { return -1; }
    if (mpfr_nan_p(ctx.fy)) { return 1; }
    out = mpfr_cmp(ctx.fx, ctx.fy);
})
POSIT_MPFR_FUNC(int, cmpabs, {
    if (mpfr_nan_p(ctx.fx)) { return -1; }
    if (mpfr_nan_p(ctx.fy)) { return 1; }
    out = mpfr_cmpabs(ctx.fx, ctx.fy);
})
POSIT_MPFR_FUNC(bool, equals, {
    if (mpfr_nan_p(ctx.fx)) { return 0; }
    if (mpfr_nan_p(ctx.fy)) { return 0; }
    out = mpfr_cmp(ctx.fx, ctx.fy) == 0;
})

POSIT_MPFR_1FUNC(POSIT_T, sqrt, {
    mpfr_sqrt(ctx.fx, ctx.fx, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_1FUNC(POSIT_T, log, {
    mpfr_log(ctx.fx, ctx.fx, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_1FUNC(POSIT_T, abs, {
    mpfr_abs(ctx.fx, ctx.fx, MPFR_RNDN);
    out = POSIT_MKNAME(fromMpfr)(ctx.fx);
})

POSIT_MPFR_1FUNC(POSIT_T, exp, {
    mpfr_exp(ctx.fx, ctx.fx, MPFR_RNDN);
    if (mpfr_zero_p(ctx.fx)) {
        // be careful, mpfr flushes to zero!
        out = POSIT_GLUE3(posit,POSIT_NBITS,_MIN);
    } else {
        out = POSIT_MKNAME(fromMpfr)(ctx.fx);
    }
})

#ifdef POSIT_NEXT_BITS
#define POSIT_NEXT_T POSIT_GLUE3(posit,POSIT_NEXT_BITS,_t)
POSIT_MPFR_FUNC(POSIT_NEXT_T, mul_promote, {
    mpfr_mul(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_GLUE3(posit,POSIT_NEXT_BITS,_fromMpfr)(ctx.fx);
})
POSIT_MPFR_FUNC(POSIT_NEXT_T, div_promote, {
    mpfr_div(ctx.fx, ctx.fx, ctx.fy, MPFR_RNDN);
    out = POSIT_GLUE3(posit,POSIT_NEXT_BITS,_fromMpfr)(ctx.fx);
})
#undef POSIT_NEXT_T
#endif

/*
char* POSIT_MKNAME(str)(char* buf, POSIT_T x) {
    POSIT_MKNAME(Context_t) ctx;
    POSIT_MKNAME(init)(&ctx);
    POSIT_MKNAME(_toMpfr)(ctx.i, ctx.fx, x);
    mpfr_snprintf(buf, POSIT_GLUE3(posit,POSIT_NBITS,_str_SIZE), "%.Rf", ctx.fx);
    POSIT_MKNAME(fini)(&ctx);
    return buf;
}*/

POSIT_MPFR_1FUNC(long double, told, { out = mpfr_get_ld(ctx.fx, MPFR_RNDN); })
POSIT_MPFR_1FUNC(double, tod, { out = mpfr_get_d(ctx.fx, MPFR_RNDN); })
POSIT_MPFR_1FUNC(float, tof, { out = mpfr_get_flt(ctx.fx, MPFR_RNDN); })

///
#define POSIT_MPFR_FUNC1(__name__, __argt__, __width__, ...) \
    POSIT_T POSIT_MKNAME(__name__)(__argt__ x) { \
        mpfr_t mpfr; mpfr_init2(mpfr, __width__); \
        __VA_ARGS__; \
        POSIT_T out = POSIT_MKNAME(fromMpfr)(mpfr); \
        mpfr_clear(mpfr); \
        return out; \
    }

// set long double seems to round no matter what we do...
POSIT_MPFR_FUNC1(fromld, long double, 128, { mpfr_set_ld(mpfr, x, MPFR_RNDZ); })

POSIT_MPFR_FUNC1(fromd, double, 64, { mpfr_set_d(mpfr, x, MPFR_RNDZ); })
POSIT_MPFR_FUNC1(fromf, float, 32, { mpfr_set_flt(mpfr, x, MPFR_RNDZ); })

#define CONVERTER(__width__) \
    POSIT_MPFR_FUNC1(POSIT_GLUE(fromp, __width__), POSIT_GLUE3(posit, __width__, _t), __width__, { \
        POSIT_GLUE3(posit, __width__, _toMpfr)(mpfr, x); })
#if POSIT_NBITS != 8
CONVERTER(8)
#endif
#if POSIT_NBITS != 16
CONVERTER(16)
#endif
#if POSIT_NBITS != 32
CONVERTER(32)
#endif
#if POSIT_NBITS != 64
CONVERTER(64)
#endif

#if __SIZEOF_LONG__ == 4
    #define U32FORULONG(x) (x)
    #define S32FORSLONG(x) (x)
    __Static_assert(sizeof(uintmax_t) == 8, "uintmax_t size is not 8 bytes");
    POSIT_MPFR_1FUNC(int64_t, tosl, { out = mpfr_get_sj(ctx.fx, MPFR_RNDN); })
    POSIT_MPFR_1FUNC(uint64_t, toul, { out = mpfr_get_uj(ctx.fx, MPFR_RNDN); })
    POSIT_MPFR_FUNC1(fromsl, int64_t, 64, { POSIT_CANT_ROUND(mpfr_set_sj(mpfr, x, MPFR_RNDZ)); })
    POSIT_MPFR_FUNC1(fromul, uint64_t, 64, { POSIT_CANT_ROUND(mpfr_set_uj(mpfr, x, MPFR_RNDZ)); })
#else
    _Static_assert(sizeof(long) == 8, "sizeof(long) must be 4 or 8");
    #define U32FORULONG(x) ( ((x) > UINT32_MAX) ? UINT32_MAX : (x) )
    #define S32FORSLONG(x) ( ((x) > INT32_MAX) ? INT32_MAX : ((x) < INT32_MIN ? INT32_MIN : (x)) )
    POSIT_MPFR_1FUNC(int64_t, tosl, { out = mpfr_get_si(ctx.fx, MPFR_RNDN); })
    POSIT_MPFR_1FUNC(uint64_t, toul, { out = mpfr_get_ui(ctx.fx, MPFR_RNDN); })
    POSIT_MPFR_FUNC1(fromsl, int64_t, 64, { POSIT_CANT_ROUND(mpfr_set_si(mpfr, x, MPFR_RNDZ)); })
    POSIT_MPFR_FUNC1(fromul, uint64_t, 64, { POSIT_CANT_ROUND(mpfr_set_ui(mpfr, x, MPFR_RNDZ)); })
#endif

POSIT_MPFR_1FUNC(int32_t, tosi, {
    long x = mpfr_get_si(ctx.fx, MPFR_RNDN);
    out = S32FORSLONG(x);
})
POSIT_MPFR_1FUNC(uint32_t, toui, {
    unsigned long x = mpfr_get_ui(ctx.fx, MPFR_RNDN);
    out = U32FORULONG(x);
})
POSIT_MPFR_FUNC1(fromsi, int32_t, 32, { POSIT_CANT_ROUND(mpfr_set_si(mpfr, x, MPFR_RNDZ)); })
POSIT_MPFR_FUNC1(fromui, uint32_t, 32, { POSIT_CANT_ROUND(mpfr_set_ui(mpfr, x, MPFR_RNDZ)); })

POSIT_T POSIT_MKNAME(shift)(POSIT_T x, int32_t y) {
    POSIT_MKNAME(Context_t) ctx;
    POSIT_MKNAME(init)(&ctx, 2*POSIT_NBITS);
    POSIT_MKNAME(_toMpfr)(ctx.i, ctx.fx, x);

    POSIT_CANT_ROUND(mpfr_mul_2si(ctx.fy, ctx.fx, y, MPFR_RNDZ));
    POSIT_T out = POSIT_MKNAME(_fromMpfr)(ctx.i, ctx.fy);
    POSIT_MKNAME(fini)(&ctx);
    return out;
}

#define NAR POSIT_GLUE3(posit,POSIT_NBITS,_NAR)
#define ZERO POSIT_GLUE3(posit,POSIT_NBITS,_ZERO)

POSIT_X2_T POSIT_MKNAME(add_exact)(POSIT_T x, POSIT_T y) {
    // if either is nar, return (nar,0)
    POSIT_X2_T out;
    if (POSIT_MKNAME(equals)(x, NAR) || POSIT_MKNAME(equals)(y, NAR)) {
        out.x = NAR;
        out.y = ZERO;
        return out;
    }

    // y should always be less than or equal to x, in magnitude
    if (POSIT_MKNAME(cmpabs)(x, y) < 0) {
        POSIT_T _x = x;
        x = y;
        y = _x;
    }

    // if x or y is zero then return [y or x, 0]
    bool xz = POSIT_MKNAME(equals)(x, ZERO);
    if (xz || POSIT_MKNAME(equals)(y, ZERO)) {
        out.x = (xz) ? y : x;
        out.y = ZERO;
        return out;
    }

    POSIT_T s = POSIT_MKNAME(add)(x, y);
    POSIT_T r = POSIT_MKNAME(sub)(y, POSIT_MKNAME(sub)(s, x));
    out.x = s;
    out.y = r;
    return out;
}

POSIT_X2_T POSIT_MKNAME(sub_exact)(POSIT_T x, POSIT_T y) {
    POSIT_T _y = POSIT_MKNAME(sub)(ZERO, y);
    return POSIT_MKNAME(add_exact)(x, _y);
}

#endif // POSIT_LIMITED

POSIT_MPFR_1FUNC(int32_t, iexp, {
    if (!mpfr_number_p(ctx.fx)) {
        out = 0;
    } else {
        mpfr_exp_t e = 0;
        assert(!mpfr_frexp(&e, ctx.fx, ctx.fx, MPFR_RNDN));
        out = e;
    }
})

POSIT_MPFR_1FUNC(POSIT_T, fract, {
    mpfr_exp_t e = 0;
    mpfr_frexp(&e, ctx.fy, ctx.fx, MPFR_RNDN);
    out = POSIT_MKNAME(_fromMpfr)(ctx.i, ctx.fy);
})

#endif // POSIT_UNDEF

#ifndef POSIT_NOUNDEF
    #undef POSIT_CANT_ROUND
    #undef POSIT_UNDEF
    #undef POSIT_MKNAME
    #undef POSIT_GLUE
    #undef POSIT_GLUE2
    #undef POSIT_GLUE3
    #undef POSIT_GLUE4
#endif
