dnl Copyright 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
dnl Process this file with autoconf to produce a configure script.

AC_INIT(Makefile.am)

AC_PREREQ(2.50)
dnl AC_CHECK_COMPILERS()
AC_PROG_CXX()
dnl AC_ENABLE_SHARED(yes)
dnl AC_ENABLE_STATIC(yes)
AC_LIBTOOL_WIN32_DLL()
AC_PROG_LIBTOOL()

AC_CANONICAL_SYSTEM()
dnl perform program name transformation
AC_ARG_PROGRAM()

AM_INIT_AUTOMAKE(testsuite, 0.1)



AC_ARG_WITH(base,
  [  --with-base=DIR         where the Base Framework is installed],
  [
     ac_base_include="$withval/include"
     ac_base_lib="$withval/lib"
  ])

AC_ARG_WITH(base-include,
  [  --with-base-include=DIR location of the Base Framework includes],
  [
    ac_base_include="$withval"
  ])

AC_ARG_WITH(base-lib,
  [  --with-base-lib=DIR     location of the Base Framework libraries],
  [
    ac_base_lib="$withval"
  ])

if test "x$ac_base_include" != "x"; then
  CPPFLAGS="-I${ac_base_include}${CPPFLAGS:+ }${CPPFLAGS}"
fi
if test "x$ac_base_lib" != "x"; then
  LIBS="${LIBS}${LIBS:+ }${ac_base_lib}/libbase.la"
fi

AC_OUTPUT(Makefile)
