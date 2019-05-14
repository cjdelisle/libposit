// SPDX-License-Identifier: MIT OR BSD-3-Clause
#pragma once

typedef struct allimpls_impl_s allimpls_impl_t;
struct allimpls_impl_s {
    const char* name;
    allimpls_impl_t* next;
};

typedef struct allimpls_func_s allimpls_func_t;
struct allimpls_func_s {
    const char* name;
    uintptr_t generic;
    allimpls_func_t* next;
    allimpls_impl_t* impls;
};

allimpls_func_t* g_posit_functions;

void allimpls_register();

// returns non-zero if there was a disagreement between implementations
int allimpls_had_disagreement();

void allimpls_crash_on_disagree();

#define ANYFUNC(__name__, __args__, __rett__, __argdef__) \
    typedef struct posit__GLUE(__name__, _impl_s) posit__GLUE(__name__, _impl_t); \
    struct posit__GLUE(__name__, _impl_s) { \
        allimpls_impl_t impl; \
        __rett__ (*call)__argdef__; \
    };

#define posit_FUNC(__name__,__args__,__rett__,__argdef__) \
    __rett__ posit__GLUE(__name__,_all)__argdef__; \
    ANYFUNC(__name__, __args__, __rett__, __argdef__)
#define posit_VFUNC(__name__,__args__,__argdef__) \
    ANYFUNC(__name__, __args__, void, __argdef__)
#include "positgen.h"
#undef posit_FUNC
#undef posit_VFUNC
#undef ANYFUNC
