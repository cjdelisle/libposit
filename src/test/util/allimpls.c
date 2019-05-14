// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define posit_WITH_MPFR
#include "posit.h"
#define posit_WITH_MPFR
#include "enumerate_implementations.h"

static int g_had_disagreement;
static int g_crash_on_disagree;

int allimpls_had_disagreement() {
    return g_had_disagreement;
}

void allimpls_crash_on_disagree() {
    g_crash_on_disagree = 1;
}

void allimpls_register() {
    static int isregged;
    if (isregged) { return; }
    isregged = 1;
    #include "implementations.h"
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG(...) //printf(__VA_ARGS__)

static char arg1 = '\0';
static uint8_t* g_arg1p = (uint8_t*) &arg1;

#define posit_FUNC(__name__,__args__,__rett__,__argdef__) \
    __rett__ posit__GLUE(__name__,_all)__argdef__ { \
        allimpls_register(); \
        DEBUG("running %s", STRING(__name__)); \
        __rett__ posit__GLUE(__name__,_slow)__argdef__; \
        __rett__ slowout = posit__GLUE(__name__,_slow)__args__; \
        allimpls_func_t* func = &posit__GLUE3(g_, __name__, _func); \
        allimpls_impl_t* impl = func->impls; \
        for (; impl; impl = impl->next) { \
            DEBUG(" %s", impl->name); \
            posit__GLUE(__name__, _impl_t)* realimpl = (posit__GLUE(__name__, _impl_t)*) impl; \
            if (realimpl->call == posit__GLUE(__name__,_slow)) { continue; } \
            __rett__ implout = realimpl->call __args__; \
            if (memcmp(&implout, &slowout, sizeof(__rett__))) { \
                g_had_disagreement = 1; \
                uint8_t* so = (uint8_t*) &slowout; \
                uint8_t* io = (uint8_t*) &implout; \
                uint8_t* _a = (uint8_t*) &arg0; \
                uint8_t* _b = (uint8_t*) &arg1; \
                printf("function %s: slow and [%s] disagree\nslow : ", func->name, impl->name); \
                for (int i = (int)sizeof(__rett__) - 1; i >= 0; i--) { printf("%02x", so[i]); } \
                printf("\n     : "); \
                for (int i = (int)sizeof(__rett__) - 1; i >= 0; i-- ) { printf("%02x", io[i]); } \
                printf("\n arg0: "); \
                for (int i = (int)(sizeof arg0) - 1; i >= 0; i-- ) { printf("%02x", _a[i]); } \
                if (_b != g_arg1p) { \
                    printf("\n arg1: "); \
                    for (int i = (int)(sizeof arg1) - 1; i >= 0; i-- ) { printf("%02x", _b[i]); } \
                } \
                printf("\n"); \
                if (g_crash_on_disagree) { abort(); } \
            } \
        } \
        DEBUG("\n"); \
        return slowout; \
    }
#define posit_VFUNC(__name__,__args__,__argdef__)
#define STRING(x) _STRING(x)
#define _STRING(x) #x

#pragma GCC diagnostic ignored "-Wsizeof-array-argument"
#define posit_WITH_MPFR
#include "positgen.h"
