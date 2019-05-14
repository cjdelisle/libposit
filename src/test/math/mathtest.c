// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"
#include "useall.h"

#define POSIT_NBITS 8
#include "mathtest.h"
#undef POSIT_NBITS

#define POSIT_NBITS 16
#include "mathtest.h"
#undef POSIT_NBITS

#define POSIT_NBITS 32
#include "mathtest.h"
#undef POSIT_NBITS

#define POSIT_NBITS 64
#include "mathtest.h"
#undef POSIT_NBITS

int main() {
    TestContext_t tc = {
        .initialVal = 0,
        .initialValB = 0,
        .range = 0xff,
        .rangeB = 0xff
    };
    mpfr_init2(tc.ma, 128);
    mpfr_init2(tc.mb, 128);
    mpfr_init2(tc.mc, 128);
    mpfr_init2(tc.mref, 128);

    posit8_tests(&tc);

    tc.rangeB = tc.range = 0x20;


    tc.initialVal = 0x7ff0;
    tc.initialValB = 0x7ff0;
    posit16_tests(&tc);
    tc.initialVal = 0xfff0;
    tc.initialValB = 0x7ff0;
    posit16_tests(&tc);
    tc.initialVal = 0xfff0;
    tc.initialValB = 0xfff0;
    posit16_tests(&tc);


    tc.initialVal = 0x7ffffff0;
    tc.initialValB = 0x7ffffff0;
    posit32_tests(&tc);
    tc.initialVal = 0xfffffff0;
    tc.initialValB = 0x7ffffff0;
    posit32_tests(&tc);
    tc.initialVal = 0xfffffff0;
    tc.initialValB = 0xfffffff0;
    posit32_tests(&tc);


    tc.initialVal = 0x7ffffffffffffff0ull;
    tc.initialValB = 0x7ffffffffffffff0ull;
    posit64_tests(&tc);
    tc.initialVal = 0xfffffffffffffff0ull;
    tc.initialValB = 0x7ffffffffffffff0ull;
    posit64_tests(&tc);
    tc.initialVal = 0xfffffffffffffff0ull;
    tc.initialValB = 0xfffffffffffffff0ull;
    posit64_tests(&tc);

    mpfr_clear(tc.ma);
    mpfr_clear(tc.mb);
    mpfr_clear(tc.mc);
    mpfr_clear(tc.mref);
}
