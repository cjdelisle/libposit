// SPDX-License-Identifier: MIT OR BSD-3-Clause
LITERAL(/)*
SPDX-License-Identifier: MIT OR BSD-3-Clause
This file is auto-generated from src/templates/gen_posit.h
Please only edit files inside of src/templates/
*LITERAL(/)
#define posit_FUNC(__name__, __args__, __rett__, __argdef__) posit_EXPORT __rett__ __name__ __argdef__;
#define posit_VFUNC(__name__, __args__, __argdef__) posit_EXPORT void __name__ __argdef__;
#define posit_IFUNC(...) static inline __VA_ARGS__
#define posit_ENUM(...) enum { __VA_ARGS__ };
#define posit_CONST(...) static const __VA_ARGS__;

LITERAL(#pragma) once
LITERAL(#ifdef) __cplusplus
extern "C" {
LITERAL(#endif)

LITERAL(#include) <stdint.h>

LITERAL(#define) posit_EXPORT __attribute__ ((visibility ("default")))

LITERAL(#ifdef) posit_WITH_MPFR
LITERAL(#include) <gmp.h>
LITERAL(#include) <mpfr.h>
LITERAL(#endif)

typedef struct posit8_s { uint8_t v; } posit8_t;
_Static_assert(sizeof(posit8_t) == 1, "");

typedef struct posit8x2_s { posit8_t x; posit8_t y; } posit8x2_t;
_Static_assert(sizeof(posit8x2_t) == 2, "");

typedef struct posit16_s { uint16_t v; } posit16_t;
_Static_assert(sizeof(posit16_t) == 2, "");

typedef struct posit16x2_s { posit16_t x; posit16_t y; } posit16x2_t;
_Static_assert(sizeof(posit16x2_t) == 4, "");

typedef struct posit32_s { uint32_t v; } posit32_t;
_Static_assert(sizeof(posit32_t) == 4, "");

typedef struct posit32x2_s { posit32_t x; posit32_t y; } posit32x2_t;
_Static_assert(sizeof(posit32x2_t) == 8, "");

typedef struct posit64_s { uint64_t v; } posit64_t;
_Static_assert(sizeof(posit64_t) == 8, "");

typedef struct posit64x2_s { posit64_t x; posit64_t y; } posit64x2_t;
_Static_assert(sizeof(posit64x2_t) == 16, "");

typedef struct posit128_s { uint64_t lo; uint64_t hi; } posit128_t;
_Static_assert(sizeof(posit128_t) == 16, "");


posit_IFUNC(uint8_t   posit8_bits(posit8_t p)   { return p.v; })
posit_IFUNC(uint16_t  posit16_bits(posit16_t p) { return p.v; })
posit_IFUNC(uint32_t  posit32_bits(posit32_t p) { return p.v; })
posit_IFUNC(uint64_t  posit64_bits(posit64_t p) { return p.v; })

posit_IFUNC(posit8_t   posit8_reinterpret(uint8_t n)   { posit8_t  x; x.v = n; return x; })
posit_IFUNC(posit16_t  posit16_reinterpret(uint16_t n) { posit16_t x; x.v = n; return x; })
posit_IFUNC(posit32_t  posit32_reinterpret(uint32_t n) { posit32_t x; x.v = n; return x; })
posit_IFUNC(posit64_t  posit64_reinterpret(uint64_t n) { posit64_t x; x.v = n; return x; })

LITERAL(/)/ report posit format for posit8_t. str must be at least 8 characters in size:    8.0x40p + /0 is 8 chars
posit_ENUM(posit8_debug_SIZE = 8)

LITERAL(/)/ report posit format for posit16_t. str must be at least 11 characters in size:  16.1x4000p + /0 is 11 chars
posit_ENUM(posit16_debug_SIZE = 11)

LITERAL(/)/ report posit format for posit32_t. str must be at least 15 characters in size:  32.2x40000000p + /0 is 15 chars
posit_ENUM(posit32_debug_SIZE = 16)

LITERAL(/)/ report posit format for posit64_t. str must be at least 23 characters in size:  64.3x1234567812345678p + /0 is 23 chars
posit_ENUM(posit64_debug_SIZE = 23)

LITERAL(/)/ report posit format for posit128_t. str must be at least 40 characters in size:  128.4x12345678123456781234567812345678p + /0 is 40 chars
posit_ENUM(posit128_debug_SIZE = 40)

LITERAL(#if) defined(__cplusplus) || defined(_MSC_VER)
posit_IFUNC(posit128_t posit128_reinterpret(uint64_t* n) {
    posit128_t out;
    out.lo = n[0];
    out.hi = n[1];
    return out;
})
LITERAL(#else)
// static array parameters are illegal in C++ but they provide valuable verification in C
posit_IFUNC(posit128_t posit128_reinterpret(uint64_t n[static 2]) {
    return (posit128_t) { .lo = n[0], .hi = n[1] };
})
LITERAL(#endif)

#include "templates/main.h"

LITERAL(#if) __STDC_VERSION__ >= 201112L && !defined(_MSC_VER) && !defined(posit_NO_GENERICS)

LITERAL(#define) posit__FROM(nbits, x) (_Generic((x), LITERAL(\)
	long double: 		posit__GLUE4(posit,nbits,_from,ld), LITERAL(\)
	double: 			posit__GLUE4(posit,nbits,_from,d), LITERAL(\)
	float: 				posit__GLUE4(posit,nbits,_from,f), LITERAL(\)
	int64_t: 			posit__GLUE4(posit,nbits,_from,sl), LITERAL(\)
    uint64_t: 			posit__GLUE4(posit,nbits,_from,ul), LITERAL(\)
    int32_t: 			posit__GLUE4(posit,nbits,_from,si), LITERAL(\)
	uint32_t:           posit__GLUE4(posit,nbits,_from,ui), LITERAL(\)
	posit8_t: 			posit__GLUE4(posit,nbits,_from,p8), LITERAL(\)
	posit16_t: 			posit__GLUE4(posit,nbits,_from,p16), LITERAL(\)
	posit32_t: 			posit__GLUE4(posit,nbits,_from,p32), LITERAL(\)
	posit64_t: 			posit__GLUE4(posit,nbits,_from,p64) LITERAL(\)
)(x))
LITERAL(#define) posit8(x)       posit__FROM(8, (x))
LITERAL(#define) posit16(x)      posit__FROM(16, (x))
LITERAL(#define) posit32(x)      posit__FROM(32, (x))
LITERAL(#define) posit64(x)      posit__FROM(64, (x))

LITERAL(#define) posit__OP2(nbits, op, y) _Generic((y), LITERAL(\)
	long double: 	posit__GLUE5(posit,nbits,_,op,ld), LITERAL(\)
	double: 		posit__GLUE5(posit,nbits,_,op,d), LITERAL(\)
	float: 			posit__GLUE5(posit,nbits,_,op,f), LITERAL(\)
	int64_t: 		posit__GLUE5(posit,nbits,_,op,sl), LITERAL(\)
	uint64_t: 		posit__GLUE5(posit,nbits,_,op,ul), LITERAL(\)
	int32_t: 		posit__GLUE5(posit,nbits,_,op,si), LITERAL(\)
	uint32_t:   	posit__GLUE5(posit,nbits,_,op,ui), LITERAL(\)
	posit8_t: 		posit__GLUE5(posit,nbits,_,op,p8), LITERAL(\)
	posit16_t: 		posit__GLUE5(posit,nbits,_,op,p16), LITERAL(\)
	posit32_t: 		posit__GLUE5(posit,nbits,_,op,p32), LITERAL(\)
	posit64_t: 		posit__GLUE5(posit,nbits,_,op,p64) LITERAL(\)
)
LITERAL(#define) posit__OP2X(nbits, op, x) _Generic((x), LITERAL(\)
	long double: 	posit__GLUE5(posit,nbits,_,ld,op), LITERAL(\)
	double: 		posit__GLUE5(posit,nbits,_,d,op), LITERAL(\)
	float: 			posit__GLUE5(posit,nbits,_,f,op), LITERAL(\)
	int64_t: 		posit__GLUE5(posit,nbits,_,sl,op), LITERAL(\)
	uint64_t: 		posit__GLUE5(posit,nbits,_,ul,op), LITERAL(\)
	int32_t: 		posit__GLUE5(posit,nbits,_,si,op), LITERAL(\)
	uint32_t:	    posit__GLUE5(posit,nbits,_,ui,op), LITERAL(\)
	posit8_t: 		posit__GLUE5(posit,nbits,_,p8,op), LITERAL(\)
	posit16_t: 		posit__GLUE5(posit,nbits,_,p16,op), LITERAL(\)
	posit32_t: 		posit__GLUE5(posit,nbits,_,p32,op), LITERAL(\)
	posit64_t: 		posit__GLUE5(posit,nbits,_,p64,op) LITERAL(\)
)

LITERAL(#define) posit__GENERIC_OP(p,x,op) (_Generic((p), LITERAL(\)
	posit8_t:           posit__OP2(8, op, (x)), LITERAL(\)
	posit16_t:          posit__OP2(16, op, (x)), LITERAL(\)
	posit32_t:          posit__OP2(32, op, (x)), LITERAL(\)
	posit64_t:          posit__OP2(64, op, (x)), LITERAL(\)
	default: _Generic((x), LITERAL(\)
		posit8_t:       posit__OP2X(8, op, (p)), LITERAL(\)
		posit16_t:      posit__OP2X(16, op, (p)), LITERAL(\)
		posit32_t:      posit__OP2X(32, op, (p)), LITERAL(\)
		posit64_t:      posit__OP2X(64, op, (p)) LITERAL(\)
	) LITERAL(\)
)((p),(x)))
LITERAL(#define) posit_add(p, x) posit__GENERIC_OP(p,x,add)
LITERAL(#define) posit_sub(p, x) posit__GENERIC_OP(p,x,sub)
LITERAL(#define) posit_mul(p, x) posit__GENERIC_OP(p,x,mul)
LITERAL(#define) posit_div(p, x) posit__GENERIC_OP(p,x,div)
LITERAL(#define) posit_cmp(p, x) posit__GENERIC_OP(p,x,cmp)
LITERAL(#define) posit_cmpabs(p, x) posit__GENERIC_OP(p,x,cmpabs)

LITERAL(#define) posit__GENETIC1(p, op) _Generic((p), LITERAL(\)
	posit8_t: posit__GLUE(posit8_,op), LITERAL(\)
	posit16_t: posit__GLUE(posit16_,op), LITERAL(\)
	posit32_t: posit__GLUE(posit32_,op), LITERAL(\)
	posit64_t: posit__GLUE(posit64_,op) LITERAL(\)
)
LITERAL(#define) posit_sqrt(p)       posit__GENETIC1(p, sqrt)(p)
LITERAL(#define) posit_log(p)        posit__GENETIC1(p, log)(p)
LITERAL(#define) posit_exp(p)        posit__GENETIC1(p, exp)(p)
LITERAL(#define) posit_told(p)       posit__GENETIC1(p, told)(p)
LITERAL(#define) posit_tod(p)        posit__GENETIC1(p, tod)(p)
LITERAL(#define) posit_tof(p)        posit__GENETIC1(p, tof)(p)
LITERAL(#define) posit_tosl(p)       posit__GENETIC1(p, tosl)(p)
LITERAL(#define) posit_tosi(p)       posit__GENETIC1(p, tosi)(p)
LITERAL(#define) posit_toul(p)       posit__GENETIC1(p, toul)(p)
LITERAL(#define) posit_toui(p)       posit__GENETIC1(p, toui)(p)

LITERAL(#define) posit_bits(p) (_Generic((p), LITERAL(\)
	posit4_t: posit4_bits, LITERAL(\)
	posit8_t: posit8_bits, LITERAL(\)
	posit16_t: posit16_bits, LITERAL(\)
	posit32_t: posit32_bits, LITERAL(\)
	posit64_t: posit64_bits LITERAL(\)
)(p))

LITERAL(#endif)

LITERAL(#ifdef) __cplusplus
}
LITERAL(#endif)
