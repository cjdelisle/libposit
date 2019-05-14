// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define POSIT_IMPL roundingtest_e0
#define POSIT_NBITS 16
#define POSIT_ES 0
#include "slow/mpz_macros.h"
#undef POSIT_IMPL
#undef POSIT_NBITS
#undef POSIT_ES

#define POSIT_IMPL roundingtest_e1
#define POSIT_NBITS 32
#define POSIT_ES 1
#include "slow/mpz_macros.h"
#undef POSIT_IMPL
#undef POSIT_NBITS
#undef POSIT_ES

#define POSIT_IMPL roundingtest_e2
#define POSIT_NBITS 64
#define POSIT_ES 2
#include "slow/mpz_macros.h"
#undef POSIT_IMPL
#undef POSIT_NBITS
#undef POSIT_ES

#define POSIT_IMPL roundingtest_e3
#define POSIT_NBITS 128
#define POSIT_ES 3
#include "slow/mpz_macros.h"
#undef POSIT_IMPL
#undef POSIT_NBITS
#undef POSIT_ES

static int usage() {
    printf("Usage: findmiddle <nbits> <pattern>\n");
    printf("    findmiddle 8 4f\n");
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        return usage();
    }
    long ptype = strtol(argv[1], NULL, 10);
    switch (ptype) {
        case 8:
        case 16:
        case 32:
        case 64: break;
        default: return usage();
    }
    mpz_t in; mpz_init(in);
    if (mpz_set_str(in, argv[2], 16)) {
        mpz_clear(in);
        return usage();
    }
    mpz_t i1; mpz_init(i1);
    mpz_mul_2exp(i1, in, 1);
    mpz_add_ui(i1, i1, 1);
    mpz_mul_2exp(i1, i1, ptype-1);
    // 1024 bits should be enough...
    mpfr_t out; mpfr_init2(out, 1024);
    switch (ptype) {
        case 8: posit16_mpzToMpfr_roundingtest_e0(out, i1); break;
        case 16: posit32_mpzToMpfr_roundingtest_e1(out, i1); break;
        case 32: posit64_mpzToMpfr_roundingtest_e2(out, i1); break;
        case 64: posit128_mpzToMpfr_roundingtest_e3(out, i1); break;
        default: abort();
    }
    mpz_add_ui(i1, in, 1);
    mpfr_printf("Middle point between %Zx and %Zx is: %.80Rf\n", in, i1, out);

    mpz_clear(in);
    mpz_clear(i1);
    mpfr_clear(out);
}
