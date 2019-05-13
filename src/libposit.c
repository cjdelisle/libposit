#define posit_WITH_MPFR
#include "posit.h"
#define posit_WITH_MPFR
#include "slowimpl.h"
#define posit_WITH_MPFR
#include "useslow.h"

// This is where we should override implementations of functions

#define posit_FUNC(__name__, __args__, __rett__, __argdef__) \
    __rett__ __name__ __argdef__ { return POSIT_GLUE(__name__,_IMPL)__args__; }

#define posit_VFUNC(__name__, __args__, __argdef__) \
    void __name__ __argdef__ { POSIT_GLUE(__name__,_IMPL)__args__; }

#define POSIT_STRING(x) #x
#define POSIT_GLUE(x,y) POSIT__GLUE(x,y)
#define POSIT__GLUE(x,y) x##y

#define posit_WITH_MPFR
#include "positgen.h"

