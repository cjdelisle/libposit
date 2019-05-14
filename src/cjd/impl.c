// SPDX-License-Identifier: MIT OR BSD-3-Clause
#include "impl.h"

// This is named differently just to provide an example of how to use posit_*_REGISTER_NAMED()
// see impl.h and docs/writing_an_implementation.md
int posit8_equals_cjd_differentlynamed(posit8_t arg0, posit8_t arg1) {
    return (arg0.v != posit8_NAR.v) & (arg0.v == arg1.v);
}
int posit16_equals_cjd(posit16_t arg0, posit16_t arg1) {
    return (arg0.v != posit16_NAR.v) & (arg0.v == arg1.v);
}
int posit32_equals_cjd(posit32_t arg0, posit32_t arg1) {
    return (arg0.v != posit32_NAR.v) & (arg0.v == arg1.v);
}
int posit64_equals_cjd(posit64_t arg0, posit64_t arg1) {
    return (arg0.v != posit64_NAR.v) & (arg0.v == arg1.v);
}

int posit8_cmp_cjd(posit8_t arg0, posit8_t arg1) {
    if (arg0.v == posit8_NAR.v) { return -1; }
    if (arg1.v == posit8_NAR.v) { return 1; }
    int8_t a = arg0.v;
    int8_t b = arg1.v;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit16_cmp_cjd(posit16_t arg0, posit16_t arg1) {
    if (arg0.v == posit16_NAR.v) { return -1; }
    if (arg1.v == posit16_NAR.v) { return 1; }
    int16_t a = arg0.v;
    int16_t b = arg1.v;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit32_cmp_cjd(posit32_t arg0, posit32_t arg1) {
    if (arg0.v == posit32_NAR.v) { return -1; }
    if (arg1.v == posit32_NAR.v) { return 1; }
    int32_t a = arg0.v;
    int32_t b = arg1.v;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit64_cmp_cjd(posit64_t arg0, posit64_t arg1) {
    if (arg0.v == posit64_NAR.v) { return -1; }
    if (arg1.v == posit64_NAR.v) { return 1; }
    int64_t a = arg0.v;
    int64_t b = arg1.v;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}


int posit8_cmpabs_cjd(posit8_t arg0, posit8_t arg1) {
    if (arg0.v == posit8_NAR.v) { return -1; }
    if (arg1.v == posit8_NAR.v) { return 1; }
    int8_t a = arg0.v;
    int8_t b = arg1.v;
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit16_cmpabs_cjd(posit16_t arg0, posit16_t arg1) {
    if (arg0.v == posit16_NAR.v) { return -1; }
    if (arg1.v == posit16_NAR.v) { return 1; }
    int16_t a = arg0.v;
    int16_t b = arg1.v;
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit32_cmpabs_cjd(posit32_t arg0, posit32_t arg1) {
    if (arg0.v == posit32_NAR.v) { return -1; }
    if (arg1.v == posit32_NAR.v) { return 1; }
    int32_t a = arg0.v;
    int32_t b = arg1.v;
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int posit64_cmpabs_cjd(posit64_t arg0, posit64_t arg1) {
    if (arg0.v == posit64_NAR.v) { return -1; }
    if (arg1.v == posit64_NAR.v) { return 1; }
    int64_t a = arg0.v;
    int64_t b = arg1.v;
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
