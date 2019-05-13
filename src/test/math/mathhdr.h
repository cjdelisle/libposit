#pragma once

#include <mpfr.h>

typedef struct TestContext_s {
    uint64_t initialVal;
    uint64_t initialValB;
    uint64_t range;
    uint64_t rangeB;

    mpfr_t ma;
    mpfr_t mb;
    mpfr_t mc;
    mpfr_t mref;
} TestContext_t;
