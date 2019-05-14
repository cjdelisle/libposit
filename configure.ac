#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libposit], [0.0.1], [cjd@cjdns.fr])
AC_CONFIG_SRCDIR([src/libposit.c])
AM_INIT_AUTOMAKE([1.11 dist-bzip2 tar-ustar foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_AWK
AC_PROG_SED
PKG_PROG_PKG_CONFIG
m4_pattern_forbid([^PKG_[A-Z_]+$], [missing some pkg-config macros (pkg-config package)])

gmplibs="-lmpfr -lgmp"
gmpinc=

AC_ARG_WITH(gmp,
[AS_HELP_STRING([--with-gmp=PATH],
		[Specify directory for MPFR.
		 Same as --with-gmp-include=PATH/include and --with-gmp-lib=PATH/lib])])
AC_ARG_WITH(gmp-include,
[AS_HELP_STRING([--with-gmp-include=PATH],
		[Specify directory for MPFR header files])])
AC_ARG_WITH(gmp-lib,
[AS_HELP_STRING([--with-gmp-lib=PATH],
		[Specify directory for MPFR library])])
if test "x$with_gmp" != x; then
  gmplibs="-L$with_gmp/lib $gmplibs"
  gmpinc="-I$with_gmp/include $gmpinc"
fi
if test "x$with_gmp_include" != x; then
  gmpinc="-I$with_gmp_include $gmpinc"
fi
if test "x$with_gmp_lib" != x; then
  gmplibs="-L$with_gmp_lib $gmplibs"
fi

AC_ARG_WITH(mpfr,
[AS_HELP_STRING([--with-mpfr=PATH],
		[Specify directory for MPFR.
		 Same as --with-mpfr-include=PATH/include and --with-mpfr-lib=PATH/lib])])
AC_ARG_WITH(mpfr-include,
[AS_HELP_STRING([--with-mpfr-include=PATH],
		[Specify directory for MPFR header files])])
AC_ARG_WITH(mpfr-lib,
[AS_HELP_STRING([--with-mpfr-lib=PATH],
		[Specify directory for MPFR library])])
if test "x$with_mpfr" != x; then
  gmplibs="-L$with_mpfr/lib $gmplibs"
  gmpinc="-I$with_mpfr/include $gmpinc"
fi
if test "x$with_mpfr_include" != x; then
  gmpinc="-I$with_mpfr_include $gmpinc"
fi
if test "x$with_mpfr_lib" != x; then
  gmplibs="-L$with_mpfr_lib $gmplibs"
fi

AC_MSG_CHECKING([for gmp/mpfr])
saved_LIBS="$LIBS"
LIBS="$LIBS $gmplibs"
saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $gmpinc"
AC_TRY_LINK([
    #include <gmp.h>
    #include <mpfr.h>
],[
    mpfr_t n;
    mpz_t x;
    mpfr_init2(n, 128);
    mpz_init(x);
],
    [ AC_MSG_RESULT([yes]) ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
            libmpfr and/or libgmp not found, mpfr and gmp are required to build libposit.
            Make sure libmpfr and libgmp are installed, if they're installed in a non-typical
            location, use ./configure --help to see how you can specify that location.
        ])
    ]
)
LIBS="$saved_LIBS"
CFLAGS="$saved_CFLAGS"

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# set pkgconfigdir, allow override
AC_ARG_WITH([pkgconfigdir],
    AS_HELP_STRING([--with-pkgconfigdir=PATH],
        [Path to the pkgconfig directory [[LIBDIR/pkgconfig]]]),
    [pkgconfigdir="$withval"],
    [pkgconfigdir='${libdir}/pkgconfig'])
AC_SUBST([pkgconfigdir])
AC_SUBST(gmplibs)
AC_SUBST(gmpinc)

AC_CONFIG_FILES([ \
    Makefile \
    src/libposit.pc ])
AC_OUTPUT
