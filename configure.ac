#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([slowposit], [0.0.1], [cjd@cjdns.fr])
AC_CONFIG_SRCDIR([src/posit_macros_impl.h])
AM_INIT_AUTOMAKE([1.11 dist-bzip2 tar-ustar foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

PKG_CHECK_MODULES([gmp], [libgmp], [], [
    AC_MSG_ERROR([
        libgmp not found, gmp is required to build slowposit.
    ])
])
PKG_CHECK_MODULES([mpfr], [libmpfr], [], [
    AC_MSG_ERROR([
        libmpfr not found, mpfr is required to build slowposit.
    ])
])

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

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
