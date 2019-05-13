LITERAL(/)*
This file is auto-generated from src/templates/main.h
Please only edit files inside of src/templates/
*LITERAL(/)

#define GLUE4(a,b,c,d) GLUE(GLUE(a,b),GLUE(c,d))
#define GLUE3(a,b,c) GLUE(GLUE(a,b),c)
#define GLUE(a,b) GLUE_(a,b)
#define GLUE_(a,b) a ## b

LITERAL(#define) posit__GLUE4(a,b,c,d) posit__GLUE(posit__GLUE(a,b),posit__GLUE(c,d))
LITERAL(#define) posit__GLUE3(a,b,c) posit__GLUE(posit__GLUE(a,b),c)
LITERAL(#define) posit__GLUE(a,b) posit__GLUE_(a,b)
LITERAL(#define) posit__GLUE_(a,b) a ## b

// These need to be pre-defined for everything because they're used in the macros
posit_FUNC(posit8_fromp16, (arg0), posit8_t, (posit16_t arg0))
posit_FUNC(posit8_fromp32, (arg0), posit8_t, (posit32_t arg0))
posit_FUNC(posit8_fromp64, (arg0), posit8_t, (posit64_t arg0))

posit_FUNC(posit16_fromp8, (arg0), posit16_t, (posit8_t arg0))
posit_FUNC(posit16_fromp32, (arg0), posit16_t, (posit32_t arg0))
posit_FUNC(posit16_fromp64, (arg0), posit16_t, (posit64_t arg0))

posit_FUNC(posit32_fromp16, (arg0), posit32_t, (posit16_t arg0))
posit_FUNC(posit32_fromp8, (arg0), posit32_t, (posit8_t arg0))
posit_FUNC(posit32_fromp64, (arg0), posit32_t, (posit64_t arg0))

posit_FUNC(posit64_fromp16, (arg0), posit64_t, (posit16_t arg0))
posit_FUNC(posit64_fromp32, (arg0), posit64_t, (posit32_t arg0))
posit_FUNC(posit64_fromp8, (arg0), posit64_t, (posit8_t arg0))

#define POSIT_NBITS 8
#define POSIT_NEXT_BITS 16
#define POSIT_ES 0
#include "templates/macros.h"

#define POSIT_LAST_BITS 8
#define POSIT_NBITS 16
#define POSIT_NEXT_BITS 32
#define POSIT_ES 1
#include "templates/macros.h"

#define POSIT_LAST_BITS 16
#define POSIT_NBITS 32
#define POSIT_NEXT_BITS 64
#define POSIT_ES 2
#include "templates/macros.h"

#define POSIT_LAST_BITS 32
#define POSIT_NBITS 64
#define POSIT_NEXT_BITS 128
#define POSIT_ES 3
#include "templates/macros.h"

#define POSIT_NBITS 128
#define POSIT_ES 4
#define POSIT_LIMITED
#include "templates/macros.h"
