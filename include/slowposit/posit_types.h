#pragma once

#include <stdint.h>

typedef struct Posit8_s { uint8_t v; } Posit8_t;
_Static_assert(sizeof(Posit8_t) == 1, "");

typedef struct Posit16_s { uint16_t v; } Posit16_t;
_Static_assert(sizeof(Posit16_t) == 2, "");

typedef struct Posit32_s { uint32_t v; } Posit32_t;
_Static_assert(sizeof(Posit32_t) == 4, "");

typedef struct Posit64_s { uint64_t v; } Posit64_t;
_Static_assert(sizeof(Posit64_t) == 8, "");

typedef struct Posit128_s {
    uint64_t x;
    uint64_t y;
} Posit128_t;
_Static_assert(sizeof(Posit128_t) == 16, "");

typedef struct Posit256_s {
    uint64_t x;
    uint64_t y;
    uint64_t z;
    uint64_t w;
} Posit256_t;
_Static_assert(sizeof(Posit256_t) == 32, "");
