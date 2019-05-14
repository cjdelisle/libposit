// SPDX-License-Identifier: MIT OR BSD-3-Clause
#include "posit.h"

#define posit_IMPLEMENTATION softposit
#include "implement_posit.h"

// to/from double and float
posit8_tod_REGISTER_NAMED(convertP8ToDouble)
posit16_tod_REGISTER_NAMED(convertP16ToDouble)
posit32_tod_REGISTER_NAMED(convertP32ToDouble)

posit16_fromf_REGISTER_NAMED(convertFloatToP16)
posit32_fromf_REGISTER_NAMED(convertFloatToP32)

posit8_fromd_REGISTER_NAMED(convertDoubleToP8)
posit16_fromd_REGISTER_NAMED(convertDoubleToP16)
posit32_fromd_REGISTER_NAMED(convertDoubleToP32)

// from integer
posit8_fromui_REGISTER_NAMED(ui32_to_p8)
posit16_fromui_REGISTER_NAMED(ui32_to_p16)
posit32_fromui_REGISTER_NAMED(ui32_to_p32)

posit8_fromul_REGISTER_NAMED(ui64_to_p8)
posit16_fromul_REGISTER_NAMED(ui64_to_p16)
posit32_fromul_REGISTER_NAMED(ui64_to_p32)

// to integer
posit8_tosi_REGISTER_NAMED(p8_to_i32)
posit16_tosi_REGISTER_NAMED(p16_to_i32)
posit32_tosi_REGISTER_NAMED(p32_to_i32)

posit8_toui_REGISTER_NAMED(p8_to_ui32)
posit16_toui_REGISTER_NAMED(p16_to_ui32)
posit32_toui_REGISTER_NAMED(p32_to_ui32)

posit8_tosl_REGISTER_NAMED(p8_to_i64)
posit16_tosl_REGISTER_NAMED(p16_to_i64)
posit32_tosl_REGISTER_NAMED(p32_to_i64)

posit8_toul_REGISTER_NAMED(p8_to_ui64)
posit16_toul_REGISTER_NAMED(p16_to_ui64)
posit32_toul_REGISTER_NAMED(p32_to_ui64)

// posit/posit conversions
posit8_fromp16_REGISTER_NAMED(p16_to_p8)
posit8_fromp32_REGISTER_NAMED(p32_to_p8)
posit16_fromp8_REGISTER_NAMED(p8_to_p16)
posit16_fromp32_REGISTER_NAMED(p32_to_p16)
posit32_fromp8_REGISTER_NAMED(p8_to_p32)
posit32_fromp16_REGISTER_NAMED(p16_to_p32)

posit8_add_REGISTER_NAMED(p8_add)
posit16_add_REGISTER_NAMED(p16_add)
posit32_add_REGISTER_NAMED(p32_add)


posit8_sub_REGISTER_NAMED(p8_sub)
posit16_sub_REGISTER_NAMED(p16_sub)
posit32_sub_REGISTER_NAMED(p32_sub)


posit8_mul_REGISTER_NAMED(p8_mul)
posit16_mul_REGISTER_NAMED(p16_mul)
//posit32_mul_REGISTER_NAMED(p32_mul) // https://gitlab.com/cerlane/SoftPosit/issues/31


posit8_div_REGISTER_NAMED(p8_div)
posit16_div_REGISTER_NAMED(p16_div)
posit32_div_REGISTER_NAMED(p32_div)


posit8_sqrt_REGISTER_NAMED(p8_sqrt)
posit16_sqrt_REGISTER_NAMED(p16_sqrt)
posit32_sqrt_REGISTER_NAMED(p32_sqrt)

#undef posit_IMPLEMENTATION
