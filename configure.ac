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
        [1.3.3],
        [Robin H. Johnson robbat2 AT gentoo.org],
        [localshell])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/structures.hh])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CXX
AC_PROG_CC
# These come from the autotoolset package
#LF_CONFIGURE_CC
AC_PROG_CC
AC_PROG_CPP
AC_AIX
AC_ISC_POSIX
AC_MINIX 
AC_HEADER_STDC

#LF_CONFIGURE_CXX
AC_PROG_CXX
AC_PROG_CXXCPP
AC_DEFUN([LF_CPP_PORTABILITY],[

  dnl
  dnl Check for common C++ portability problems
  dnl

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  dnl Check whether we have bool
  AC_MSG_CHECKING(whether C++ has bool)
  AC_TRY_RUN([main() { bool b1=true; bool b2=false; }],
             [ AC_MSG_RESULT(yes) ],
             [ AC_MSG_RESULT(no)
               AC_DEFINE([CXX_HAS_NO_BOOL],[],[C++ has bool]) ],
             [ AC_MSG_WARN([Don't cross-compile])]
            )

  dnl Test whether C++ has buggy for-loops
  AC_MSG_CHECKING(whether C++ has buggy scoping in for-loops)
  AC_TRY_COMPILE([#include <iostream.h>], [
   for (int i=0;i<10;i++) { }
   for (int i=0;i<10;i++) { }
], [ AC_MSG_RESULT(no) ],
   [ AC_MSG_RESULT(yes)
     AC_DEFINE([CXX_HAS_BUGGY_FOR_LOOPS],[],[C++ has buggy scoping in for-loops]) ])

  dnl Test whether the user wants to enable assertions
  AC_MSG_CHECKING(whether user wants assertions)
  AC_ARG_ENABLE(assert,
                [  --disable-assert        don't use cpp.h assert],
                [ AC_DEFINE([NDEBUG],[],[user wants assertions])
                  AC_MSG_RESULT(no)  ],
                [ AC_MSG_RESULT(yes) ],
               )

  dnl Done with the portability checks
  AC_LANG_RESTORE
])
LF_CPP_PORTABILITY

AC_DEFUN([LF_HOST_TYPE], [
  AC_CANONICAL_HOST
  if test -z "$host"
  then
    host=unknown
  fi
  canonical_host_type=$host
  if test "$host" = unknown
  then
    AC_MSG_WARN(configuring for unknown system type)
  fi
  AC_SUBST(canonical_host_type)
  AC_DEFINE_UNQUOTED([YOUR_OS],"$canonical_host_type",[system type])
])
LF_HOST_TYPE
# --------------------------------------------------------------------------
# Check whether the C++ compiler accepts a certain flag
# If it does it adds the flag to CXXFLAGS
# If it does not then it returns an error to lf_ok
# Usage:
#   LF_CHECK_CXX_FLAG(-flag1 -flag2 -flag3 ...)
# -------------------------------------------------------------------------

AC_DEFUN([LF_CHECK_CXX_FLAG],[
  echo 'void f(){}' > conftest.cc
  for i in $1
  do
    AC_MSG_CHECKING([whether $CXX accepts $i])
    if test -z "`${CXX} $i -c conftest.cc 2>&1`"
    then
      CXXFLAGS="${CXXFLAGS} $i"
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
    fi
  done
  rm -f conftest.cc conftest.o
])

# --------------------------------------------------------------------------
# Check whether the C compiler accepts a certain flag
# If it does it adds the flag to CFLAGS
# If it does not then it returns an error to lf_ok
# Usage:
#  LF_CHECK_CC_FLAG(-flag1 -flag2 -flag3 ...)
# -------------------------------------------------------------------------

AC_DEFUN([LF_CHECK_CC_FLAG],[
  echo 'void f(){}' > conftest.c
  for i in $1
  do
    AC_MSG_CHECKING([whether $CC accepts $i])
    if test -z "`${CC} $i -c conftest.c 2>&1`"
    then
      CFLAGS="${CFLAGS} $i"
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
    fi
  done
  rm -f conftest.c conftest.o
])

# ----------------------------------------------------------------------
# Provide the configure script with an --with-warnings option that
# turns on warnings. Call this command AFTER you have configured ALL your
# compilers. 
# ----------------------------------------------------------------------

AC_DEFUN([LF_SET_WARNINGS],[
  dnl Check for --with-warnings
  AC_MSG_CHECKING([whether user wants warnings])
  AC_ARG_WITH(warnings,
              [  --with-warnings         Turn on warnings],
              [ lf_warnings=yes ], [ lf_warnings=no ])
  AC_MSG_RESULT($lf_warnings)
  
  dnl Warnings for the two main compilers
  cc_warning_flags="-Wall"
  cxx_warning_flags="-Wall -Woverloaded-virtual -Wtemplate-debugging"
  if test $lf_warnings = yes
  then
    if test -n "${CC}"
    then
      LF_CHECK_CC_FLAG($cc_warning_flags)
    fi
    if test -n "${CXX}" 
    then
      LF_CHECK_CXX_FLAG($cxx_warning_flags)
    fi
  fi
])
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
   doc/Makefile
   src/Makefile
])

AC_OUTPUT
