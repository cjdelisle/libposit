// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"
#include "test/util/allimpls.h"

#include <assert.h>
#include <stdio.h>

/*
ROUNDING TEST Posit8 range 00 - ff
middlepoint between 91 and 92 is -3.8125000000000000000000000000000000000000, 91
less -3.8125000000000000000000000000000000000001
*/

static void testp32to8() {
    //posit32_t p32 = posit32_reinterpret(0xb0bfe591);
    //  -3.8125000000000000000000000000000000000000
    //  -3.8124999999999999999999999999999999999999
    double dy = -3.8125;
    double dz = -3.812500000000001;
    assert(dy > dz);
    mpfr_t my, mz;
    mpfr_init(my);
    mpfr_init(mz);
    mpfr_set_d(my, dy, MPFR_RNDN);
    mpfr_set_d(mz, dz, MPFR_RNDN);
    mpfr_printf("%.40Rf %.40Rf\n\n", my, mz);
    posit8_t p8y = posit8_fromMpfr(my);
    posit8_t p8z = posit8_fromMpfr(mz);
    printf("\np8y = %02x\np8z = %02x\n", posit8_bits(p8y), posit8_bits(p8z));

    printf("%.40f\n%.40f\n", dy, dz);
    p8y = posit8_fromd_all(dy);
    p8z = posit8_fromd_all(dz);
    printf("\np8y = %02x\np8z = %02x\n", posit8_bits(p8y), posit8_bits(p8z));
}

int main() {
    testp32to8();

    mpfr_t mpfr;
    mpfr_init2(mpfr, 1024);
    mpfr_const_pi(mpfr, MPFR_RNDN);
    mpfr_mul_si(mpfr, mpfr, 3, MPFR_RNDN);
    mpfr_mul(mpfr, mpfr, mpfr, MPFR_RNDN);

    posit32_t xxp32 = posit32_fromsi(1);
    posit32_t xx_p32 = posit32_reinterpret(posit32_bits(xxp32) + 1);
    mpfr_printf("%.40Rf %.40f %.40f\n", mpfr, posit32_tod(xxp32), posit32_tod(xx_p32));

    mpfr_printf("%.40f\n", ( (3.14159265358979323846264338327950288f * 3.0f) * (3.14159265358979323846264338327950288f * 3.0f) ));

    xxp32 = posit32_fromd(3.14159265358979323846264338327950288);
    xx_p32 = posit32_fromui(3);
    posit32_t mul = posit32_mul(xx_p32, xxp32);
    mul = posit32_mul(mul, mul);
    posit32_toMpfr(mpfr, mul);
    mpfr_printf("%.40Rf\n", mpfr);


    posit8_t p8; p8.v = 0;
    assert(posit8_tosi_all(p8) == 0);
    for (p8.v = 0; p8.v < 0x80; p8.v++) {
        //printf("%02x\n", p8.v);
        assert(posit8_tosi_all(p8) >= 0);
    }
    for (p8.v = 0x80; p8.v != 0; p8.v++) {
        //printf("%02x\n", p8.v);
        assert(posit8_tosi_all(p8) <= 0);
    }

    posit32_t p32; p32.v = 0x80063666;
    //mpfr_t mpfr;
    //mpfr_init2(mpfr, 128);
    posit32_toMpfr(mpfr, p32);
    mpfr_printf("%.40Rf\n", mpfr);

    p32.v = 0xb0bfe591;
    posit32_toMpfr(mpfr, p32);
    mpfr_printf("%.40Rf\n", mpfr);
    double d = posit32_tod_all(p32);
    p8 = posit8_fromp32_all(p32);
    posit8_t _p8 = posit8_fromd_all(d);
    mpfr_printf("%.40Rf %.40f %08x %02x %02x\n", mpfr, d, p32.v, p8.v, _p8.v);
    assert(p8.v == _p8.v);
}
