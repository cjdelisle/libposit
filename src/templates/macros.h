// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define POSIT_MKNAME(name) GLUE4(posit,POSIT_NBITS,_,name)
#define POSIT_T GLUE3(posit,POSIT_NBITS, _t)
#define POSIT_X2_T GLUE3(posit,POSIT_NBITS, x2_t)

posit_FUNC(POSIT_MKNAME(iexp), (arg0), int32_t, (POSIT_T arg0))
posit_FUNC(POSIT_MKNAME(fract), (arg0), POSIT_T, (POSIT_T arg0))

posit_ENUM(POSIT_MKNAME(NBITS) = POSIT_NBITS)
posit_ENUM(POSIT_MKNAME(ES) = POSIT_ES)
posit_ENUM(POSIT_MKNAME(LOG2_MAXVAL) = ( POSIT_MKNAME(NBITS) * (1<<POSIT_MKNAME(ES)) - (1<<(POSIT_MKNAME(ES)+1)) ))
posit_ENUM(POSIT_MKNAME(USEED) = ( 1 << (1<<POSIT_MKNAME(ES)) ))

LITERAL(#ifdef) posit_WITH_MPFR
posit_FUNC(POSIT_MKNAME(fromMpfr), (arg0), POSIT_T, (mpfr_t arg0))
posit_VFUNC(POSIT_MKNAME(toMpfr), (arg0, arg1), (mpfr_t arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(mpzToMpfr), (arg0, arg1), int, (mpfr_t arg0, mpz_t arg1))
posit_FUNC(POSIT_MKNAME(mpzFromMpfr), (arg0, arg1), int, (mpz_t arg0, mpfr_t arg1))
posit_FUNC(POSIT_MKNAME(fromMpz), (arg0), POSIT_T, (mpz_t arg0))
posit_VFUNC(POSIT_MKNAME(toMpz), (arg0, arg1), (mpz_t arg0, POSIT_T arg1))
LITERAL(#endif)

#ifndef POSIT_LIMITED

posit_CONST(POSIT_T POSIT_MKNAME(ZERO) = { 0 })
posit_CONST(POSIT_T POSIT_MKNAME(NAR) = { 1ull<<(POSIT_NBITS- 1) })
posit_CONST(POSIT_T POSIT_MKNAME(MAX) = { (1ull<<(POSIT_NBITS- 1)) - 1 })
posit_CONST(POSIT_T POSIT_MKNAME(NEGMAX) = { (1ull<<(POSIT_NBITS- 1)) + 1 })
posit_CONST(POSIT_T POSIT_MKNAME(MIN) = { 1 })
posit_CONST(POSIT_T POSIT_MKNAME(NEGMIN) = { -1ll })

posit_IFUNC(POSIT_X2_T GLUE3(posit,POSIT_NBITS,x2)(POSIT_T x, POSIT_T y) {
    POSIT_X2_T out;
    out.x = x;
    out.y = y;
    return out;
})

posit_FUNC(POSIT_MKNAME(add), (arg0, arg1), POSIT_T, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(sub), (arg0, arg1), POSIT_T, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(add_exact), (arg0, arg1), POSIT_X2_T, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(sub_exact), (arg0, arg1), POSIT_X2_T, (POSIT_T arg0, POSIT_T arg1))

posit_FUNC(POSIT_MKNAME(mul), (arg0, arg1), POSIT_T, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(div), (arg0, arg1), POSIT_T, (POSIT_T arg0, POSIT_T arg1))
#ifdef POSIT_NEXT_BITS
posit_FUNC(POSIT_MKNAME(mul_promote), (arg0, arg1), GLUE3(posit,POSIT_NEXT_BITS, _t), (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(div_promote), (arg0, arg1), GLUE3(posit,POSIT_NEXT_BITS, _t), (POSIT_T arg0, POSIT_T arg1))
#endif

posit_FUNC(POSIT_MKNAME(equals), (arg0, arg1), int, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(cmp), (arg0, arg1), int, (POSIT_T arg0, POSIT_T arg1))
posit_FUNC(POSIT_MKNAME(cmpabs), (arg0, arg1), int, (POSIT_T arg0, POSIT_T arg1))

posit_FUNC(POSIT_MKNAME(sqrt), (arg0), POSIT_T, (POSIT_T arg0))
posit_FUNC(POSIT_MKNAME(abs), (arg0), POSIT_T, (POSIT_T arg0))
posit_FUNC(POSIT_MKNAME(log), (arg0), POSIT_T, (POSIT_T arg0))
posit_FUNC(POSIT_MKNAME(exp), (arg0), POSIT_T, (POSIT_T arg0))

posit_FUNC(POSIT_MKNAME(shift), (arg0, arg1), POSIT_T, (POSIT_T arg0, int32_t arg1))
posit_FUNC(POSIT_MKNAME(debug), (arg0, arg1), char*, (char* arg0, POSIT_T arg1))

posit_IFUNC(POSIT_T POSIT_MKNAME(GLUE(fromp, POSIT_NBITS))(POSIT_T x) { return x; })
posit_IFUNC(posit8_t POSIT_MKNAME(top8)(POSIT_T x) { return GLUE(posit8_fromp,POSIT_NBITS)(x); })
posit_IFUNC(posit16_t POSIT_MKNAME(top16)(POSIT_T x) { return GLUE(posit16_fromp,POSIT_NBITS)(x); })
posit_IFUNC(posit32_t POSIT_MKNAME(top32)(POSIT_T x) { return GLUE(posit32_fromp,POSIT_NBITS)(x); })
posit_IFUNC(posit64_t POSIT_MKNAME(top64)(POSIT_T x) { return GLUE(posit64_fromp,POSIT_NBITS)(x); })

#define OPTYPE(__rett__,__op__,__type__,__name__) \
posit_IFUNC(__rett__ POSIT_MKNAME(GLUE(__op__, __name__))(POSIT_T x, __type__ y) { \
    return POSIT_MKNAME(__op__)(x, POSIT_MKNAME(GLUE(from, __name__))(y)); \
})LITERAL(\n)\
posit_IFUNC(__rett__ POSIT_MKNAME(GLUE(__name__,__op__))(__type__ x, POSIT_T y) { \
    return POSIT_MKNAME(__op__)(POSIT_MKNAME(GLUE(from, __name__))(x), y); \
})LITERAL(\n)

#ifdef POSIT_NEXT_BITS
#define IFPROMOTE(__rett__,__op__,__type__,__name__) OPTYPE(__rett__,__op__,__type__,__name__)
#else
#define IFPROMOTE(__rett__,__op__,__type__,__name__)
#endif

#define CONVERTERS(__name__, __type__)\
posit_FUNC(POSIT_MKNAME(GLUE(from, __name__)), (arg0), POSIT_T, (__type__ arg0))LITERAL(\n)\
posit_FUNC(POSIT_MKNAME(GLUE(to, __name__)), (arg0), __type__, (POSIT_T arg0))LITERAL(\n)

#define TYPEFUNCS(__name__, __type__)\
OPTYPE(POSIT_T,add,__type__,__name__)\
OPTYPE(POSIT_T,sub,__type__,__name__)\
OPTYPE(POSIT_T,mul,__type__,__name__)\
OPTYPE(POSIT_T,div,__type__,__name__)\
OPTYPE(POSIT_X2_T,add_exact,__type__,__name__)\
OPTYPE(POSIT_X2_T,sub_exact,__type__,__name__)\
OPTYPE(int,cmp,__type__,__name__)\
OPTYPE(int,cmpabs,__type__,__name__)\
IFPROMOTE(GLUE3(posit,POSIT_NEXT_BITS, _t),mul_promote,__type__,__name__)\
IFPROMOTE(GLUE3(posit,POSIT_NEXT_BITS, _t),div_promote,__type__,__name__)

#define TYPE(__name__, __type__)\
CONVERTERS(__name__, __type__)\
TYPEFUNCS(__name__, __type__)

TYPE(ld,long double)
TYPE(d,double)
TYPE(f,float)
TYPE(sl,int64_t)
TYPE(ul,uint64_t)
TYPE(si,int32_t)
TYPE(ui,uint32_t)
TYPEFUNCS(p8,posit8_t)
TYPEFUNCS(p16,posit16_t)
TYPEFUNCS(p32,posit32_t)
TYPEFUNCS(p64,posit64_t)

#endif

#undef POSIT_NBITS
#undef POSIT_ES
#undef POSIT_NEXT_BITS
#undef POSIT_LAST_BITS
#undef POSIT_X2_T
#undef POSIT_MKNAME
#undef POSIT_WORD_SZ
#undef POSIT_LIMITED
