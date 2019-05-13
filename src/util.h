#pragma once

#include <mpfr.h>

#ifdef MPFR_FLAGS_INEXACT
#define util_CHK_FLAGS mpfr_flags_save
#else
#define util_CHK_FLAGS() ( \
    mpfr_underflow_p() | mpfr_overflow_p() | mpfr_divby0_p() | \
    mpfr_nanflag_p() | mpfr_inexflag_p() | mpfr_erangeflag_p() )
#endif

#define util_CANT_ROUND(expr) do {\
    mpfr_clear_flags(); \
    assert(!(expr)); \
    assert(!util_CHK_FLAGS() && #expr "should not round"); \
} while (0)
