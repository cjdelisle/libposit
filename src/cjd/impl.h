// SPDX-License-Identifier: MIT OR BSD-3-Clause
#include "posit.h"

// CAUTION: This header file is included in weird ways, DO NOT define any function
// signatures or types here. This file should only contain the implementation name
// and registration macros and should only include posit.h and implement_posit.h.

#define posit_IMPLEMENTATION cjd
#include "implement_posit.h"

// This is not recommended, but if you must register a function with a different name
// than posit<X>_<op>_<yourimpl>, you can redirect the name this way
posit8_equals_REGISTER_NAMED(posit8_equals_cjd_differentlynamed)

posit16_equals_REGISTER
posit32_equals_REGISTER
posit64_equals_REGISTER

posit8_cmp_REGISTER
posit16_cmp_REGISTER
posit32_cmp_REGISTER
posit64_cmp_REGISTER

posit8_cmpabs_REGISTER
posit16_cmpabs_REGISTER
posit32_cmpabs_REGISTER
posit64_cmpabs_REGISTER
