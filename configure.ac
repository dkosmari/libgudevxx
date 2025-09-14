#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([libgudevxx], [0.3], [])
AC_CONFIG_SRCDIR([src/Client.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_LANG([C++])
#AX_CXX_COMPILE_STDCXX([20])
AX_APPEND_COMPILE_FLAGS([-std=c++20], [CXX])

PKG_INSTALLDIR

PKG_CHECK_MODULES([GUDEV], [gudev-1.0])


AC_TYPE_SIZE_T
AC_TYPE_UINT64_T


ENABLE_EXAMPLES=no
AC_ARG_ENABLE([examples],
              [AS_HELP_STRING([--enable-examples], [Enable building example programs.])],
              [ENABLE_EXAMPLES=$enableval])
AS_VAR_IF([ENABLE_EXAMPLES], [yes],
          [
              PKG_CHECK_MODULES([GLIBMM], [glibmm-2.68],
                                [],
                                [PKG_CHECK_MODULES([GLIBMM], [glibmm-2.4])])
          ])
AM_CONDITIONAL([BUILD_EXAMPLES], [test "x$ENABLE_EXAMPLES" = "xyes"])


TARBALL_NAME="${PACKAGE_TARNAME}-${PACKAGE_VERSION}.tar.gz"
AC_SUBST([TARBALL_NAME])


AC_CONFIG_FILES([Makefile
                 examples/Makefile
                 libgudevxx.pc])
AC_OUTPUT
