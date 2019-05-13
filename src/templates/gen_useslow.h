// this file generates defines which cause every function to use the slow implementation
// after this, individual functions can be overridden.

#define posit_IFUNC(...)
#define posit_TYPE(...)
#define posit_ENUM(...)
#define posit_CONST(...)
#define LITERAL_DEFINE #define

#define posit_FUNC(__name__, __args__, __rett__, __argdef__) \
    LITERAL_DEFINE GLUE(__name__, _IMPL) GLUE(__name__,_slow)
#define posit_VFUNC(__name__, __args__, __argdef__) \
    LITERAL_DEFINE GLUE(__name__, _IMPL) GLUE(__name__,_slow)

#include "templates/main.h"
