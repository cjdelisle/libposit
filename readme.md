# Libposit

A library for working with the
[posit](https://www.johndcook.com/blog/2018/04/11/anatomy-of-a-posit-number/) number type. It
contains basic math operations for `posit8_t`, `posit16_t`, `posit32_t` and `posit64_t`.

## Example example example example !!

This example will run exactly the same on whatever machine you run it on, no matter what hardware
floating point implementation exists (if any).

```c
#include <posit.h>

int main() {
    posit32_t pi = posit32_fromd(3.14159265358979323846264338327950288);
    posit32_t r = posit32_fromui(3);
    posit32_t pi_r = posit32_mul(pi, r);
    posit32_t pi_r_squared = posit32_mul(pi_r, pi_r);
    printf("pi_r_squared = %.40f\n", posit32_tod(pi_r_squared));
}
```

## What are posits ?

[![John L. Gustafson - Posit Lecture](https://img.youtube.com/vi/aP0Y1uAA-2Y/0.jpg)](https://www.youtube.com/watch?v=aP0Y1uAA-2Y)

Numbers in a computer are limited in size to what the hardware can support. For example: a 32 bit
integer type supports only integers from `0` to `4294967296`. Therefore computing the area of a
circle using pi * r squared requires first multiplying everything by a constant such as 10 or 100,
doing the calculation, then dividing it back again, being very careful that at no point does the
number overflow the type. If we use 10 to compute the area of a circle with a 3 inch radius, we
multiply `30*32` and square it, getting `921600` which we divide by the square of `10*10` to get
`92.16`. If we want more precision, we might be tempted to try 100 but `300*314` squared is
greater than the maximum 32 bit value so the result will be wrong. "How wrong?" you might ask,
using unsigned math the result will *wrap* at `4294967296`, meaning it will become `283705408`,
which divided by the square of `100*100` is `2.83`. The actual result is about
`88.8264396098042275695104189988853602178233`.

Integer types are excellent for "mechanical" things such as counters, indexes and text characters
but they're exceedingly difficult to use in anything inherently "mathmatical". for performing math,
a programmer can either use an
[arbitrary-precision math library](https://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic) or
use [floating point numbers](https://en.wikipedia.org/wiki/Floating-point_arithmetic).

Arbitrary precision math libraries are the best for getting an accurate answer, but since they
only *emulate* big numbers, they are far slower than integer or floating point math. Floating point
numbers are defined by the [IEEE-754](https://en.wikipedia.org/wiki/IEEE_754) standard and contain
*exponent* and *fraction* components, making them able to represent numbers as small as
`0.000000000000000000000000000000000000011754943`<sup>1</sup> and as large as
`319014700000000000000000000000000000000` in only 32 bits. The way this works is by covering
different parts of the number-line with different levels of precision. For example: the difference
between the smallest `float32` and the second smallest `float32` is
`0.0000000000000000000000000000000000000000000020000000001750232` but the difference between the
largest `float32` and the second largest `float32` is a whopping `29999999998076804000000000000000`.

Floating point numbers work quite well for mathmatical operations, but they have two significant
limitations:

1. Floating point types have many undefined behaviors, for example the result of `sqrt()` may be
different on different machines making results not always reproducable. Fortunately, simple
operations such as add and multiply are specification-defined for ordinary numbers, but for
"impossible operations" such as dividing zero by zero, the result is
[NaN](https://en.wikipedia.org/wiki/NaN) which can be represented by a variety of different bit
patterns so even when the numeric results are guaranteed, the bit patterns of a result cannot
always be relied upon.
2. The floating point standard does not make provision for lossless operations. Integer operations
such as multiply and divide provide for capturing the entire result (and in the case of division,
the remainder) meaning that the result of a multiplication or division never has to be rounded.
Floating point types always round the result of whatever operation you do into something that will
fit in a single output. Therefore, doing mathmatics *accurately* can be exceedingly difficult.

### A better float ?

The posit number type is a re-thinking of floating points. Like floating point numbers, posits are
designed to allow mathmatical operations which may have very large or very small results but like
integer types, posits are designed to allow exact mathmatics when it is needed. Specific features
of posits include:

* No undefined behavior at all: Every posit operation must return exactly the same bit pattern on
every posit implementation.
* Addition and multiplication are possible without loss of precision.
* Very small numbers and very large numbers are less precise, the numbers with the highest
precision are numbers closest to 1. The difference between the float32 value 1 and the next larger
float32 is `0.0000001`, but the difference between the posit32 1 and the next larger posit is less
than `0.000000008`, a 12x increase in precision.

This "heaping" of precision where it's needed rather than where it's not also shows in the radius
example:

* `88.8264389038085937500000000000000000000000` area of a radius 3 circle (computed with float32)
* `88.8264398574829101562500000000000000000000` area of a radius 3 circle (computed with posit32)


## About libposit
Libposit is designed to provide a convenient interface for using posits as well as a place where
different posit implementations can coexist and the most efficient can be used depending on the
hardware.

### Objective 1: Be correct
Correctness is the primary objective. To aid in verification, there is a "slow" implementation of
every function which is backed by the arbitrary precision math library
[libmpfr](https://www.mpfr.org), this implementation simply the posit to an mpfr type, performs
the operation in libmpfr, then converts it back.

Secondly, we use [fuzz testing](https://en.wikipedia.org/wiki/Fuzzing) in order to check that the
results each implementations of a function are bit-for-bit identical. See
[libposit fuzzing](https://github.com/cjdelisle/libposit/blob/master/docs/libposit_fuzzing.md) for
more information about how to run the fuzz tests.

### Objective 2: Make fast hardware implementations possible
The objective of this library is to assist in specifying functions which will later be implemented
in hardware. It is *not* designed to simply be a software math library. Therefore, certain design
decisions have been made which *limit* the library, in order to make the functions more reasonable
to implement in hardware.

#### No 128 bit math
Though there is a `posit128_t` type defined, there are almost no operations for it. The reason for
this is because 128 bit processors are exceedingly rare, most processors are 32 or 64 bit. There
are a couple of operations such as `posit64_mul_promote()` and `posit64_div_promote()` which
*output* a `posit128_t`, but a 64 bit processor already needs a 128 bits to send the two operands
to the FPU, so accepting a 128 bit result is not onerous. However adding two `posit128_t`s would
require a 256 bit bus, something that is not available in most 64 bit hardware.

If you need to perform math operations on 128 bit or wider numbers, it is very unlikely that you
will find hardware which can perform your operations natively so you are probably best off avoiding
posits and instead using a purpose-built multi-precision library such as libmpfr.

#### No hardware quire
The posit specification describes a "quire" type which is big enough to allow adding of any possible
pair of posits without rounding. Unfortunately, implementing a quire in hardware would require far
more tha 128 bits of bandwidth to the FPU, so instead of using a quire, this library contains four
additional functions `add_exact()`, `sub_exact()`, `mul_promote()` and `div_promote()`.

* `add_exact() and sub_exact()`: These functions take two inputs `a` and `b` and they return two
outputs `s` and `r` where the sum of `s` and `r` is exactly the same as the sum of `a` and `b`
*and* given that constraint, the absolute value of `r` is as small as possible. These functions can
be used to perform exact summation of lists of posits by using a sort/add algorithm.
* `mul_promote() and div_promote()`: These functions are equivilent to casting to the next larger
size and then multiplying. `posit64_mul_promote()` and `posit64_div_promote()` output a
`posit128_t`. These are the only functions which create a `posit128_t`.

These four functions allow for performing arithmetic without loss of precision, therefore the
implementation of a quire can be done later using software.

### 3. Be flexible
Each function provided by this library allows multiple implementations. First, the fuzz test checks
these implementations against eachother with random inputs and secondly, the `perf_measure` utility
identifies the fastest implementation and generates a header file which can be used to re-compile
libposit with this implementation as the default.

To create a new implementation of a given function, please see
[writing an implementation](https://github.com/cjdelisle/libposit/blob/master/docs/writing_an_implementation.md)

### 4. Be fast
This library makes use of optimized functions from [softposit](https://gitlab.com/cerlane/SoftPosit)
in order to be as fast as possible for those implementations while still having broad support for
posit functions, using the slow implementations where necessary.

## Compiling

To compile libposit, you will need [libmpfr](https://www.mpfr.org).

### Apple OSX

1. Make sure you have [homebrew](https://brew.sh/) installed
2. `brew install mpfr`
3. `PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ ./configure`
4. `make`

### Apline Linux

1. `sudo apk add mpfr-dev gmp-dev`
2. `mpfr_CFLAGS=" " mpfr_LIBS="-lmpfr -lgmp" ./configure`


## Libposit API

### Types

libposit defines the following types:

* `posit8_t` 8 bit posit
  * `posit8x2_t` pair of 8 bit posits, result of `*_exact` functions.
* `posit16_t` 16 bit posit
  * `posit16x2_t` pair of 16 bit posits, result of `*_exact` functions.
* `posit32_t` 32 bit posit
  * `posit32x2_t` pair of 32 bit posits, result of `*_exact` functions.
* `posit64_t` 64 bit posit
  * `posit64x2_t` pair of 64 bit posits, result of `*_exact` functions.
* `posit128_t` 128 bit posit, result of posit64 `*_promote` functions. Only functions which work
on a `posit128_t` are `posit128_iexp()` and `posit128_fract()`.

### Operations

For each of these types, libposit provides the following functions:

* `posit<X>_t posit<X>_add(posit<X>_t x, posit<X>_t y)`: Add two posits together, output a rounded
result.
* `posit<X>_t posit<X>_sub(posit<X>_t x, posit<X>_t y)`: Subtract one posit from another, output a
rounded result.
* `posit<X>x2_t posit<X>_add_exact(posit<X>_t x, posit<X>_t y)`: Output 2 results, `sum` and
`remainder` which when added will result in the exact same value as the inputs `x` and `y` but
where the absolute value of `remainder` is as small as possible.
* `posit<X>x2_t posit<X>_sub_exact(posit<X>_t x, posit<X>_t y)`: Exactly the same as
`posit<X>_add_exact(x, y)` but with the sign of `y` flipped.
* `posit<X>_t posit<X>_mul(posit<X>_t x,posit<X>_t y)`: Multiply two posits, round the result to
nearest
* `posit<X>_t posit<X>_div(posit<X>_t x, posit<X>_t y)`: Divide two posits, round the result to
nearest
* `posit<X*2>_t posit<X>_mul_promote(posit<X>_t x, posit<X>_t y)`: Multiply two posits, promote
the result to the next larger posit type, this does not round.
* `posit<X*2>_t posit<X>_div_promote(posit<X>_t x, posit<X>_t y)`: Divide two posits, promote the
result to the next larger posit type, this will round to nearest for values such as `2/3` which
cannot be expressed exactly in the larger posit size.
* `int posit<X>_equals(posit<X>_t x, posit<X>_t y)`: Return integer value 1 if the posits `x` and
`y` are equal, 0 otherwise. If `x` and `y` are both `NaR`, they are considered unequal.
* `int posit<X>_cmp(posit<X>_t x, posit<X>_t y)`: Return integer value 1 if `x > y`, -1 if `x < y`
and 0 if they are equal. `NaR` is considered *less than* every other value, positive and negative.
If both `x` and `y` are `NaR`, `x` is considered smaller than `y`.
* `int posit<X>_cmpabs(posit<X>_t x, posit<X>_t y)`: Same semantics as `posit<X>_cmp(x, y)` but
performed over the absolute values of `x` and `y`.
* `posit<X>_t posit<X>_sqrt(posit<X>_t x)`: Returns the rounded square root of `x`.
* `posit<X>_t posit<X>_abs(posit<X>_t x)`: Returns the absolute value of `x`, does not round.
* `posit<X>_t posit<X>_log(posit<X>_t x)`: Returns the natural log of `x` rounded to nearest
representable posit value identical to
[floating point log()](http://www.cplusplus.com/reference/cmath/log/).
* `posit<X>_t posit<X>_exp(posit<X>_t x)`: Returns the exponential function of `x`, same as
[floating point exp()](http://www.cplusplus.com/reference/cmath/exp/) with the exception that
posits never round to zero.
* `posit<X>_t posit<X>_shift(posit<X>_t x, int bits)`: Returns the posit multiplied by 2 to the
power of bits. This is equivilent to a bit shift in integer math.
* `char* posit<X>_debug(char* output, posit<X>_t x)`: Print the binary representation of a posit.
The `output` buffer must be at lease `posit<X>_debug_SIZE` bytes long.
* `int posit<X>_iexp(posit<X>_t x)`: Get the exponent of the posit as an integer. This is
equivilent to the value stored in the pointer by
[floating point frexp()](http://www.cplusplus.com/reference/cmath/frexp/). This function
is available for `posit128_t`.
* `posit<X>_t posit<X>_fract(posit<X>_t x)`: Get the fractional part of the posit, essentially
this means multiply the posit value by 2 to the power of negative `posit<X>_iexp()`, the result
is a number which is between `0.5` and `1` but has the same fractional part. It is equivilent to
the return value of [floating point frexp()](http://www.cplusplus.com/reference/cmath/frexp/).
This function is available for `posit128_t`.

### Conversions

For each of the following types, libposit provides `posit<X>_from<N>(t)` and `posit<X>_to<N>(t)`.
Libposit provides conversions for the following types:

* **long double**: short name: `ld`
    * `posit<X>_t posit<X>_fromld(long double x)`
    * `long double posit<X>_told(posit<X>_t x)`
* **double**: short name: `d`
    * `posit<X>_t posit<X>_fromd(double x)`
    * `double posit<X>_tod(posit<X>_t x)`
* **float**: short name: `f`
    * `posit<X>_t posit<X>_fromf(float x)`
    * `float posit<X>_tof(posit<X>_t x)`
* **uint64_t**: short name: `ul`
    * `posit<X>_t posit<X>_fromul(uint64_t x)`
    * `uint64_t posit<X>_toul(posit<X>_t x)`
* **int64_t**: short name: `sl`
    * `posit<X>_t posit<X>_fromsl(int64_t x)`
    * `int64_t posit<X>_tosl(posit<X>_t x)`
* **uint32_t**: short name: `ui`
    * `posit<X>_t posit<X>_fromui(uint32_t x)`
    * `uint32_t posit<X>_toui(posit<X>_t x)`
* **int32_t**: short name: `si`
    * `posit<X>_t posit<X>_fromsi(int32_t x)`
    * `int32_t posit<X>_tosi(posit<X>_t x)`

There are also conversions to other posit types:

* **posit8_t**: short name: `p8`
    * `posit<X>_t posit<X>_fromp8(posit8_t x)`
    * `posit8_t posit<X>_top8(posit<X>_t x)`
* **posit16_t**: short name: `p16`
    * `posit<X>_t posit<X>_fromp16(posit16_t x)`
    * `posit16_t posit<X>_top16(posit<X>_t x)`
* **posit32_t**: short name: `p32`
    * `posit<X>_t posit<X>_fromp32(posit32_t x)`
    * `posit32_t posit<X>_top32(posit<X>_t x)`
* **posit64_t**: short name: `p64`
    * `posit<X>_t posit<X>_fromp64(posit64_t x)`
    * `posit64_t posit<X>_top64(posit<X>_t x)`

Additionally, if you define `posit_USE_MPFR` before including posit.h, you also get:

* `posit<X>_t posit<X>_fromMpfr(mpfr_t x)` convert mpfr to a posit.
* `void posit<X>_toMpfr(mpfr_t output, posit<X>_t x)` convert a posit to an mpfr type,
must be initialized before calling.

### Convenience functions

To simplify your code, you don't need to perform conversions explicitly, instead you
can use the pnumonic at the end of the operation function, for example:

```c
#include <posit.h>
int main() {
    posit32_t pi = posit32_fromd(3.14159265358979323846264338327950288);

    // Why do this?
    //posit32_t r = posit32_fromui(3);
    //posit32_t pi_r = posit32_mul(pi, posit32_fromui(3));

    // When you can do this!
    posit32_t pi_r = posit32_mului(pi, 3);

    posit32_t pi_r_squared = posit32_mul(pi_r, pi_r);
    printf("pi_r_squared = %.40f\n", posit32_tod(pi_r_squared));
}
```

If you want to subtract your posit from a constant, for example 5, tou can still use the
convenience functions but just put the short name before the name of the operation:

```c
#include <posit.h>
int main() {
    posit32_t pi = posit32_fromd(3.14159265358979323846264338327950288);
    posit32_t five_minus_pi = posit32_uimul(5, pi);
    printf("five_minus_pi = %.40f\n", posit32_tod(five_minus_pi));
}
```

### Generic API

C11 [generic selection](https://en.cppreference.com/w/c/language/generic) is supported in libposit
which means if you want to depend on C11, you can usually specify just the operation you want and
the compiler will select the exact function based on the types.

```c
#include <posit.h>
int main() {
    // Here we need to say posit32() so that it knows what output is desired, but the fact that
    // it's being converted from a double can be detected.
    posit32_t pi = posit32(3.14159265358979323846264338327950288);

    // Here we can use posit_mul() because it will detect that we're multiplying a posit32 and
    // it will detect that the second operand is an integer so it should select posit32_mulsi().
    posit32_t pi_r = posit_mul(pi, 3);

    // Again, we're detecting that it's a posit32, beware when both arguments are posits because
    // the output will be the type of the first operand.
    posit32_t pi_r_squared = posit_mul(pi_r, pi_r);

    // We need to tell it that we're converting to a double, but we don't need to explain that it's
    // a posit32 that we're converting.
    printf("pi_r_squared = %.40f\n", posit_tod(pi_r_squared));
}
```

**NOTE:** Generic selection is a *C* feature, it is not present in C++ so if you're using
Microsoft Visual Studio C++, you should stick to the standard functions until a C++ fronted is
created.

### Bit hacking

To conveniently convert a posit to a number or back with binary re-interpretation, you can use

* `uint<X>_t posit<X>_bits(posit<X>_t x)`: Output an unsigned integer with the same bit pattern as
the posit of the same width.
* `posit<X>_t posit<X>_reinterpret(uint<X>_t x)`: Interpret the bit pattern of a given unsigned
integer as a posit of the same width.

## Notes:
1. This is discounting [denormal numbers](https://en.wikipedia.org/wiki/Denormal_numbers)
