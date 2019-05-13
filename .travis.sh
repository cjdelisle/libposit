#!/bin/bash
die() { exit 1; }
eval "${MATRIX_EVAL}";
if test "`uname`" = "Darwin"; then
    brew install mpfr || die
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:`echo /usr/local/Cellar/mpfr/*/lib/pkgconfig`"
    ./configure || die
    make || die
else
    ./configure CC=$CC CXX=$CXX || die
    make || die
fi
