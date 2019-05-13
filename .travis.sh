#!/bin/bash
die() { exit 1; }
eval "${MATRIX_EVAL}";
if test "`uname`" = "Darwin"; then
    brew install mpfr || echo 'install mpfr failed, continuing anyway...'
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:`echo /usr/local/Cellar/mpfr/*/lib/pkgconfig`"
    ./configure || die
    make || die
else
    export mpfr_CFLAGS=" "
    export mpfr_LIBS="-lmpfr -lgmp"
    ./configure CC=$CC CXX=$CXX || die
    make || die
    make check || die
fi
