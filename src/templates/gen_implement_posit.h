// SPDX-License-Identifier: MIT OR BSD-3-Clause
#define LITERAL_IFNDEF #ifndef
#define LITERAL_DEFINE #define
#define LITERAL_ENDIF #endif

LITERAL_IFNDEF posit_REGISTER

#define posit_IFUNC(...)
#define posit_TYPE(...)
#define posit_ENUM(...)
#define posit_CONST(...)

#define posit_FUNC(__name__, __args__, __rett__, __argdef__) \
    LITERAL_DEFINE GLUE(__name__, _REGISTER_NAMED)(_impl_) \
        __rett__ _impl_ __argdef__; \
        static inline __rett__ posit__GLUE3(__name__,_,posit_IMPLEMENTATION)__argdef__ { \
            return _impl_ __args__; \
        }
#define posit_VFUNC(__name__, __args__, __argdef__) \
    LITERAL_DEFINE GLUE(__name__, _REGISTER_NAMED)(_impl_) \
        void _impl_ __argdef__; \
        static inline void posit__GLUE3(__name__,_,posit_IMPLEMENTATION)__argdef__ { \
            _impl_ __args__; \
        }
#include "templates/main.h"

LITERAL_ENDIF // posit_REGISTER
