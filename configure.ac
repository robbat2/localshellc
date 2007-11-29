# Copyright (C) 2005 Robin Hugh Johnson <robbat2 AT orbis-terrarum.net>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([localshell],
        [1.3],
        [Robin H. Johnson robbat2 AT gentoo.org],
        [localshell])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/structures.hh])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CXX
AC_PROG_CC
# These come from the autotoolset package
LF_CONFIGURE_CC
LF_CONFIGURE_CXX
LF_HOST_TYPE
LF_SET_WARNINGS
AC_PROG_RANLIB

# -- autoscan
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stddef.h regex.h string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_GETGROUPS
AC_CHECK_FUNCS([memset regcomp strdup strndup strstr])
# -- autoscan

AC_CONFIG_FILES([
   Makefile
   README
   doc/Makefile
   src/Makefile
])

AC_OUTPUT
