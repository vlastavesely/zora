#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([zora], [1.0], [vlastavesely@protonmail.ch])
AC_CONFIG_SRCDIR([indicator/zora-control.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.
PKG_CHECK_MODULES(GTK, gtk+-3.0)
PKG_CHECK_MODULES(AYATANA, ayatana-appindicator3-0.1)
PKG_CHECK_MODULES(XCB, xcb)
PKG_CHECK_MODULES(XCB_RANDR, xcb-randr)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
