#ifndef POSIT_UNDEF
#include <stdbool.h>
#include "slowposit/posit_types.h"

#if !defined(POSIT_NBITS) || !defined(POSIT_ES)
    // Give the IDE something to chew on
    #if __INTELLISENSE__
        #define POSIT_NBITS 16
        #define POSIT_ES 1
    #else
        #error "please define POSIT_NBITS and POSIT_ES before including this header"
    #endif
#elif POSIT_NBITS % 8
    #error "POSIT_NBITS must be a multiple of 8"
#endif


#ifdef POSIT_MULTI_ES
    #define POSIT_MKNAME(name) POSIT_GLUE6(Posit,POSIT_NBITS,e,POSIT_ES,_,name)
#else
    #define POSIT_MKNAME(name) POSIT_GLUE4(Posit,POSIT_NBITS,_,name)
    #define POSIT_T POSIT_GLUE3(Posit,POSIT_NBITS, _t)
#endif

// some enum values to export
#define POSIT_LOG2_MAXVAL    ( POSIT_NBITS * (1<<POSIT_ES) - (1<<(POSIT_ES+1)) )
#define POSIT_USEED          (((uint64_t)1)<<(1<<POSIT_ES))

// add,sub,mul,div
#define POSIT_BASE_OP(__rett__, __op__) \
    __rett__ POSIT_GLUE3(POSIT_MKNAME(__op__),p,POSIT_NBITS)(POSIT_T x, POSIT_T y); \
    static inline __rett__ POSIT_GLUE3(POSIT_MKNAME(p), POSIT_NBITS, __op__)(POSIT_T x, POSIT_T y) { \
        return POSIT_GLUE3(POSIT_MKNAME(__op__), p, POSIT_NBITS)(x, y); \
    } \
    static inline __rett__ POSIT_MKNAME(__op__)(POSIT_T x, POSIT_T y) { \
        return POSIT_GLUE3(POSIT_MKNAME(__op__), p, POSIT_NBITS)(x, y); \
    }

#define POSIT_GLUE6(a,b,c,d,e,f) POSIT_GLUE3(POSIT_GLUE4(a,b,c,d),e,f)
#define POSIT_GLUE5(a,b,c,d,e) POSIT_GLUE(POSIT_GLUE4(a,b,c,d),e)
#define POSIT_GLUE4(a,b,c,d) POSIT_GLUE(POSIT_GLUE(a,b),POSIT_GLUE(c,d))
#define POSIT_GLUE3(a,b,c) POSIT_GLUE(POSIT_GLUE(a,b),c)
#define POSIT_GLUE(a,b) POSIT_GLUE_(a,b)
#define POSIT_GLUE_(a,b) a ## b

// Enable some assertions
#define POSIT_PARANOIA 1

#ifndef POSIT_DEFINES_ONLY
    enum {
        POSIT_MKNAME(LOG2_MAXVAL) = POSIT_LOG2_MAXVAL,
        POSIT_MKNAME(USEED) =       POSIT_USEED
    };

    #ifdef POSIT_WITH_MPFR
        bool POSIT_MKNAME(mpzFromMpfr)(mpz_t out, mpfr_t in);
        bool POSIT_MKNAME(mpzToMpfr)(mpfr_t out, mpz_t in);
        #ifdef POSIT_T
            POSIT_T POSIT_MKNAME(fromMpfr)(mpfr_t in);
            void POSIT_MKNAME(toMpfr)(mpfr_t out, POSIT_T in);
        #endif
    #endif

    #ifndef POSIT_MULTI_ES
        POSIT_BASE_OP(POSIT_T, add);
        POSIT_BASE_OP(POSIT_T, sub);
        POSIT_BASE_OP(POSIT_T, mul);
        POSIT_BASE_OP(POSIT_T, div);
    #endif
#endif
#endif

#ifndef POSIT_NOUNDEF
    #undef POSIT_MKNAME
    #undef POSIT_LOG2_MAXVAL
    #undef POSIT_WORD_SZ
    #undef POSIT_GLUE
    #undef POSIT_GLUE2
    #undef POSIT_GLUE3
    #undef POSIT_GLUE4
    #undef POSIT_GLUE5
    #undef POSIT_PARANOIA
#endif
