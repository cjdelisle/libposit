// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"
#include "test/util/allimpls.h"

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define DATA_MAX 4
#define DATA_OFFSET 4

typedef struct Data_s {
    union {
        posit8_t p8[DATA_MAX*32];

        posit16_t p16[DATA_MAX*16];
        uint16_t shorts[DATA_MAX*16];

        posit32_t p32[DATA_MAX*8];
        uint32_t ui[DATA_MAX*8];
        int32_t si[DATA_MAX*8];
        float f[DATA_MAX*8];

        posit64_t p64[DATA_MAX*4];
        uint64_t ul[DATA_MAX*4];
        int64_t sl[DATA_MAX*4];
        double d[DATA_MAX*4];

        long double ld[DATA_MAX*2];
    };
    mpz_t mpz;
    mpfr_t mpfr;
    struct timeval tv;
    const char* bestimpl;
    uint64_t besttime;
} Data_t;

#pragma GCC diagnostic ignored "-Wunused-function"

#define GLUE4(w,x,y,z) GLUE(GLUE(w,x),GLUE(y,z))
#define GLUE3(x,y,z) GLUE(x,GLUE(y,z))
#define GLUE(x,y) _GLUE(x,y)
#define _GLUE(x,y) x##y

#define FOREACH(__op__,__name__) \
    __op__(8,__name__) __op__(16,__name__) __op__(32,__name__) __op__(64,__name__)

#define FROM(__width__,__name__,__type__) \
    static inline void GLUE4(measure_posit,__width__,_from,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(__type__ x)) { impl(d->__name__[i]); }

#define TOFROM(__width__,__name__,__type__) \
    static inline void GLUE4(measure_posit,__width__,_to,__name__)(Data_t* d, int i, \
        __type__ (*impl)(GLUE3(posit,__width__,_t) x)) { impl(d->GLUE(p,__width__)[i]); } \
    FROM(__width__,__name__,__type__)

#define TOFROM_EACH(__name__,__type__) \
    TOFROM(8,__name__,__type__) TOFROM(16,__name__,__type__) \
    TOFROM(32,__name__,__type__) TOFROM(64,__name__,__type__)

TOFROM_EACH(ul,uint64_t)
TOFROM_EACH(sl,int64_t)
TOFROM_EACH(ui,uint32_t)
TOFROM_EACH(si,int32_t)
TOFROM_EACH(f,float)
TOFROM_EACH(d,double)
TOFROM_EACH(ld,long double)

FROM(8,p16,posit16_t)
FROM(8,p32,posit32_t)
FROM(8,p64,posit64_t)

FROM(16,p8,posit8_t)
FROM(16,p32,posit32_t)
FROM(16,p64,posit64_t)

FROM(32,p16,posit16_t)
FROM(32,p8,posit8_t)
FROM(32,p64,posit64_t)

FROM(64,p16,posit16_t)
FROM(64,p32,posit32_t)
FROM(64,p8,posit8_t)

#define MATH2(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(GLUE3(posit,__width__,_t) x, GLUE3(posit,__width__,_t) y)) { \
            impl(d->GLUE(p,__width__)[i], d->GLUE(p,__width__)[i+DATA_OFFSET]); }
FOREACH(MATH2, add)
FOREACH(MATH2, sub)
FOREACH(MATH2, mul)
FOREACH(MATH2, div)

#define MATH2_EXACT(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,x2_t) (*impl)(GLUE3(posit,__width__,_t) x, GLUE3(posit,__width__,_t) y)) { \
            impl(d->GLUE(p,__width__)[i], d->GLUE(p,__width__)[i+DATA_OFFSET]); }
FOREACH(MATH2_EXACT, add_exact)
FOREACH(MATH2_EXACT, sub_exact)

#define MATH2_PROMOTE(__width__,__width2__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width2__,_t) (*impl)(GLUE3(posit,__width__,_t) x, GLUE3(posit,__width__,_t) y)) { \
            impl(d->GLUE(p,__width__)[i], d->GLUE(p,__width__)[i+DATA_OFFSET]); }
#define MATH2_PROMOTE_EACH(__name__) \
    MATH2_PROMOTE(8,16,__name__) MATH2_PROMOTE(16,32,__name__) \
    MATH2_PROMOTE(32,64,__name__) MATH2_PROMOTE(64,128,__name__)
MATH2_PROMOTE_EACH(mul_promote)
MATH2_PROMOTE_EACH(div_promote)

#define INT2(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        int (*impl)(GLUE3(posit,__width__,_t) x, GLUE3(posit,__width__,_t) y)) { \
            impl(d->GLUE(p,__width__)[i], d->GLUE(p,__width__)[i+DATA_OFFSET]); }
FOREACH(INT2, cmp)
FOREACH(INT2, cmpabs)
FOREACH(INT2, equals)

#define MATH1(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(GLUE3(posit,__width__,_t) x)) { \
            impl(d->GLUE(p,__width__)[i]); }
FOREACH(MATH1, exp)
FOREACH(MATH1, log)
FOREACH(MATH1, abs)
FOREACH(MATH1, sqrt)
FOREACH(MATH1, fract)

#define INT1(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        int32_t (*impl)(GLUE3(posit,__width__,_t) x)) { impl(d->GLUE(p,__width__)[i]); }
FOREACH(INT1, iexp)

#define TOMPZ(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        void (*impl)(mpz_t mpz, GLUE3(posit,__width__,_t) x)) { \
            impl(d->mpz, d->GLUE(p,__width__)[i]); }
FOREACH(TOMPZ, toMpz)

#define FROMMPZ(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(mpz_t mpz)) { impl(d->mpz); }
FOREACH(FROMMPZ, fromMpz)

#define TOMPFR(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        void (*impl)(mpfr_t mpfr, GLUE3(posit,__width__,_t) x)) { \
            impl(d->mpfr, d->GLUE(p,__width__)[i]); }
FOREACH(TOMPFR, toMpfr)

#define FROMMPFR(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(mpfr_t mpfr)) { impl(d->mpfr); }
FOREACH(FROMMPFR, fromMpfr)

#define MPZ_TO_MPFR(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        int (*impl)(mpfr_t mpfr, mpz_t mpz)) { impl(d->mpfr, d->mpz); }
FOREACH(MPZ_TO_MPFR, mpzToMpfr)

#define MPFR_TO_MPZ(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        int (*impl)(mpz_t mpz, mpfr_t mpfr)) { impl(d->mpz, d->mpfr); }
FOREACH(MPFR_TO_MPZ, mpzFromMpfr)

#define DEBUG(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        char* (*impl)(char* buf, GLUE3(posit,__width__,_t) x)) { \
            char buf[GLUE3(posit,__width__,_debug_SIZE)]; \
            impl(buf, d->GLUE(p,__width__)[i]); }
FOREACH(DEBUG, debug)

#define SHIFT(__width__,__name__) \
    static inline void GLUE4(measure_posit,__width__,_,__name__)(Data_t* d, int i, \
        GLUE3(posit,__width__,_t) (*impl)(GLUE3(posit,__width__,_t) x, int32_t y)) { \
            impl(d->GLUE(p,__width__)[i], d->shorts[i]); }
FOREACH(SHIFT, shift)

static inline void time_start(Data_t* d) {
    gettimeofday(&d->tv,(struct timezone *) 0);
}
const char* SPACES = "                                         ";
static inline void time_end(Data_t* d, const char* impl) {
    struct timeval tv;
    gettimeofday(&tv,(struct timezone *) 0);
    uint64_t usec = (tv.tv_sec - d->tv.tv_sec) * 1000000ull + tv.tv_usec - d->tv.tv_usec;
    printf("%s - %lld micros\n", &SPACES[strlen(impl)], (long long) usec);
    if (usec < d->besttime) {
        d->bestimpl = impl;
        d->besttime = usec;
    }
}

#define CYCLES 1000

#define TESTCYCLE(__name__) \
    for (int cycle = 0; cycle < CYCLES; cycle++) { \
        for (int i = 0; i < DATA_MAX; i++) { \
            GLUE(measure_,__name__)(&data, i, _imp->call); \
        } \
    } \

#define MEASURE1(__name__) \
    else if (f->generic == (uintptr_t) __name__) { \
        GLUE(__name__,_impl_t)* _imp = (GLUE(__name__,_impl_t)*) imp; \
        /* warm up the caches */ \
        TESTCYCLE(__name__); \
        time_start(&data);\
        TESTCYCLE(__name__); \
        time_end(&data, imp->name); \
    }
#define MEASURE_ALL(__name__) \
    MEASURE1(GLUE4(posit,8,_,__name__)) \
    MEASURE1(GLUE4(posit,16,_,__name__)) \
    MEASURE1(GLUE4(posit,32,_,__name__)) \
    MEASURE1(GLUE4(posit,64,_,__name__))
#define MEASURE_TOFROM(__name__) \
    MEASURE_ALL(GLUE(to,__name__)) \
    MEASURE_ALL(GLUE(from,__name__))

int main() {
    struct timeval tv;
    gettimeofday(&tv,(struct timezone *) 0);
    srand(tv.tv_usec);
    Data_t data;
    for (int i = 0; i < (int)sizeof data / 2; i++) { data.shorts[i] = rand(); }

    mpz_init_set_ui(data.mpz, (unsigned long) data.ul);
    mpfr_init2(data.mpfr, 128);
    mpfr_set_ld(data.mpfr, data.ld[0], MPFR_RNDZ);

    allimpls_register();

    allimpls_func_t* f = g_posit_functions;
    while (f) {
        printf("// %s\n", f->name);
        allimpls_impl_t* imp = f->impls;
        data.besttime = UINT64_MAX;
        data.bestimpl = "slow";
        while (imp) {
            printf("//     %s ", imp->name);
            if (0) { }
            MEASURE_ALL(exp)
            MEASURE_ALL(log)
            MEASURE_ALL(abs)
            MEASURE_ALL(sqrt)
            MEASURE_ALL(fract)

            MEASURE_ALL(iexp)

            MEASURE_ALL(add)
            MEASURE_ALL(sub)
            MEASURE_ALL(mul)
            MEASURE_ALL(div)

            MEASURE_ALL(add_exact)
            MEASURE_ALL(sub_exact)

            MEASURE_ALL(mul_promote)
            MEASURE_ALL(div_promote)

            MEASURE_ALL(cmp)
            MEASURE_ALL(cmpabs)
            MEASURE_ALL(equals)

            MEASURE_ALL(toMpz)
            MEASURE_ALL(fromMpz)
            MEASURE_ALL(toMpfr)
            MEASURE_ALL(fromMpfr)
            MEASURE_ALL(mpzToMpfr)
            MEASURE_ALL(mpzFromMpfr)

            MEASURE_ALL(debug)
            MEASURE_ALL(shift)

            MEASURE_TOFROM(ul)
            MEASURE_TOFROM(sl)
            MEASURE_TOFROM(ui)
            MEASURE_TOFROM(si)
            MEASURE_TOFROM(f)
            MEASURE_TOFROM(d)
            MEASURE_TOFROM(ld)

            MEASURE1(posit8_fromp16)
            MEASURE1(posit8_fromp32)
            MEASURE1(posit8_fromp64)

            MEASURE1(posit16_fromp8)
            MEASURE1(posit16_fromp32)
            MEASURE1(posit16_fromp64)

            MEASURE1(posit32_fromp16)
            MEASURE1(posit32_fromp8)
            MEASURE1(posit32_fromp64)

            MEASURE1(posit64_fromp16)
            MEASURE1(posit64_fromp32)
            MEASURE1(posit64_fromp8)
            else {
                printf(" - no measurement\n");
            }
            imp = imp->next;
        }
        if (strcmp(data.bestimpl, "slow")) {
            printf("#define %s_IMPL %s\n", f->name, data.bestimpl);
        }
        f = f->next;
    }

    mpz_clear(data.mpz);
    mpfr_clear(data.mpfr);
}
