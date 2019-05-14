// SPDX-License-Identifier: MIT OR BSD-3-Clause
// this file overrides function names in order that the "all" version will be used
// use this with testing to test all implementations against eachother
#define LITERAL_INCLUDE #include
LITERAL_INCLUDE "test/util/allimpls.h"

#define posit_IFUNC(...)
#define posit_TYPE(...)
#define posit_ENUM(...)
#define posit_CONST(...)
#define LITERAL_DEFINE #define
#define posit_FUNC(__name__, __args__, __rett__, __argdef__) \
    LITERAL_DEFINE __name__ GLUE(__name__,_all)
#define posit_VFUNC(__name__, __args__, __argdef__) //LITERAL_DEFINE __name__ GLUE(__name__,_all)

#include "templates/main.h"
