// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"

#include <gmp.h>
#include <mpfr.h>
#include <stdint.h>
#include <stdio.h>

#define POSIT_IMPL slowimpl

char* posit8_debug_slowimpl(char* buf, posit8_t x) {
    snprintf(buf, posit8_debug_SIZE, "8.0x%02x", x.v);
    return buf;
}
char* posit16_debug_slowimpl(char* buf, posit16_t x) {
    snprintf(buf, posit16_debug_SIZE, "16.1x%04x", x.v);
    return buf;
}
char* posit32_debug_slowimpl(char* buf, posit32_t x) {
    snprintf(buf, posit32_debug_SIZE, "32.2x%08x", x.v);
    return buf;
}
char* posit64_debug_slowimpl(char* buf, posit64_t x) {
    snprintf(buf, posit64_debug_SIZE, "64.3x%016llx", (unsigned long long)x.v);
    return buf;
}

#define POSIT_NBITS 8
#define POSIT_NEXT_BITS 16
#define POSIT_ES 0
#include "slow/mpz_macros.h"
#include "slow/macros.h"
#undef POSIT_NBITS
#undef POSIT_NEXT_BITS
#undef POSIT_ES

#define POSIT_LAST_BITS 8
#define POSIT_NBITS 16
#define POSIT_NEXT_BITS 32
#define POSIT_ES 1
#include "slow/mpz_macros.h"
#include "slow/macros.h"
#undef POSIT_LAST_BITS
#undef POSIT_NBITS
#undef POSIT_NEXT_BITS
#undef POSIT_ES

#define POSIT_LAST_BITS 16
#define POSIT_NBITS 32
#define POSIT_NEXT_BITS 64
#define POSIT_ES 2
#include "slow/mpz_macros.h"
#include "slow/macros.h"
#undef POSIT_LAST_BITS
#undef POSIT_NBITS
#undef POSIT_NEXT_BITS
#undef POSIT_ES

#define POSIT_LAST_BITS 32
#define POSIT_NBITS 64
#define POSIT_NEXT_BITS 128
#define POSIT_ES 3
#include "slow/mpz_macros.h"
#include "slow/macros.h"
#undef POSIT_LAST_BITS
#undef POSIT_NBITS
#undef POSIT_NEXT_BITS
#undef POSIT_ES

#define POSIT_NBITS 128
#define POSIT_ES 4
#define POSIT_LIMITED
#include "slow/mpz_macros.h"
#include "slow/macros.h"
#undef POSIT_NBITS
#undef POSIT_ES
#undef POSIT_LIMITED
