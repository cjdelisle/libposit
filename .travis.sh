#!/bin/bash
die() { exit 1; }
eval "${MATRIX_EVAL}";
if test "`uname`" = "Darwin"; then
    brew install gmp mpfr || echo 'install mpfr failed, continuing anyway...'
    ./configure || die
    make || die
else
    ./configure CC=$CC CXX=$CXX || die
    make || die
    make check || die
fi
