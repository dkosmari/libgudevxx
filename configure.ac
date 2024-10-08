#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([libgudevxx], [0.2+], [])
AC_CONFIG_SRCDIR([src/Client.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
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

# Checks for libraries.
PKG_CHECK_MODULES([GUDEV], [gudev-1.0])
PKG_CHECK_MODULES([GLIBMM], [glibmm-2.4])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.

TARBALL_NAME="${PACKAGE_TARNAME}-${PACKAGE_VERSION}.tar.gz"
AC_SUBST([TARBALL_NAME])

AC_CONFIG_FILES([Makefile
                 include/gudevxx/Makefile
                 libgudevxx.pc
                 libgudevxx.spec
                 src/Makefile
                 tests/Makefile
])
AC_OUTPUT
