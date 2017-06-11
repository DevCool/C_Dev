#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([game], [1.0], [drunkenmaster1988@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
LT_INIT
AC_PROG_CC
AM_INIT_AUTOMAKE

# Checks for libraries.
PKG_CHECK_MODULES([allegro], [allegro >= 4.0],, AC_MSG_ERROR([Cannot find allegro 4!]))
AC_SUBST([allegro_CFLAGS])
AC_SUBST([allegro_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
