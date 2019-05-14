// SPDX-License-Identifier: MIT OR BSD-3-Clause
// The slow implementation implements *everything*
// so rather than writing a huge implementation.h file, we generate it

#define posit_IFUNC(...)
#define posit_ENUM(...)
#define posit_CONST(...)
#define LITERAL_DEFINE #define
#define LITERAL_UNDEF #undef
#define LITERAL_INCLUDE #include
LITERAL_DEFINE posit_IMPLEMENTATION slow
LITERAL_DEFINE posit_WITH_MPFR

LITERAL_INCLUDE "implement_posit.h"

#define posit_FUNC(__name__, __args__, __rett__, __argdef__) GLUE(__name__, _REGISTER)
#define posit_VFUNC(__name__, __args__, __argdef__) GLUE(__name__, _REGISTER)

#include "templates/main.h"

LITERAL_UNDEF posit_IMPLEMENTATION
LITERAL_UNDEF posit_WITH_MPFR
