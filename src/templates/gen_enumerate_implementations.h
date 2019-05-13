#define posit_IFUNC(...)
#define posit_TYPE(...)
#define posit_ENUM(...)
#define posit_CONST(...)
#define LITERAL_DEFINE #define
#define LITERAL_UNDEF #undef
#define LITERAL_INCLUDE #include

LITERAL_INCLUDE "test/util/allimpls.h"
LITERAL_INCLUDE "posit.h"
LITERAL_INCLUDE "stddef.h"
LITERAL_DEFINE posit_REGISTER // in order to avoid implement_posit.h generating register macros

LITERAL_DEFINE CURRENT_FUNCTION NULL

#define ANYFUNC(__name__, __args__, __rett__, __argdef__) \
    static allimpls_func_t GLUE3(g_, __name__, _func) = {LITERAL(\n)\
        .name = #__name__,LITERAL(\n)\
        .generic = (void*) __name__,LITERAL(\n)\
        .next = CURRENT_FUNCTION LITERAL(\n)\
    };LITERAL(\n)\
    LITERAL_UNDEF CURRENT_FUNCTION LITERAL(\n)\
    LITERAL_DEFINE CURRENT_FUNCTION &GLUE3(g_, __name__, _func) LITERAL(\n)\
    LITERAL_DEFINE GLUE(__name__,_REGISTER_NAMED)(_impl_)LITERAL(\)LITERAL(\n)\
        __rett__ _impl_ __argdef__; \
        static GLUE(__name__, _impl_t) posit__GLUE4(g_,__name__,posit_IMPLEMENTATION,_impl) = {LITERAL(\)LITERAL(\n)\
            .impl = { .name = STRING(posit_IMPLEMENTATION) },LITERAL(\)LITERAL(\n)\
            .call = _impl_ LITERAL(\)LITERAL(\n)\
        };LITERAL(\)LITERAL(\n)\
        posit__GLUE4(g_,__name__,posit_IMPLEMENTATION,_impl).impl.next = GLUE3(g_, __name__, _func).impls;LITERAL(\)LITERAL(\n)\
        GLUE3(g_, __name__, _func).impls = &posit__GLUE4(g_,__name__,posit_IMPLEMENTATION,_impl).impl;LITERAL(\)LITERAL(\n)\
        LITERAL(\n)

LITERAL_DEFINE STRING(x) _STRING(x)
LITERAL_DEFINE _STRING(x) #x

#define posit_FUNC(__name__, __args__, __rett__, __argdef__) \
    ANYFUNC(__name__, __args__, __rett__, __argdef__)
#define posit_VFUNC(__name__, __args__, __argdef__) \
    ANYFUNC(__name__, __args__, void, __argdef__)
#include "templates/main.h"

allimpls_func_t* g_posit_functions = CURRENT_FUNCTION;
