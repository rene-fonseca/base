dnl ***************************************************************************
dnl    The Base Framework (Test Suite)
dnl    A framework for developing platform independent applications
dnl
dnl    See COPYRIGHT.txt for details.
dnl
dnl    This framework is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl
dnl    For the licensing terms refer to the file 'LICENSE'.
dnl ***************************************************************************

dnl Process this file with autoconf to produce a configure script

AC_INIT(Makefile.am)
AC_CONFIG_SRCDIR(testsuite)
AC_CONFIG_AUX_DIR(..)

AC_PREREQ(2.52)
AC_REVISION($Revision$)
dnl AC_CHECK_COMPILERS()
AC_PROG_CXX()
dnl AC_ENABLE_SHARED(yes)
dnl AC_ENABLE_STATIC(yes)
AC_LIBTOOL_WIN32_DLL()
AC_PROG_LIBTOOL()

AC_CANONICAL_SYSTEM()
dnl perform program name transformation
AC_ARG_PROGRAM()

AM_INIT_AUTOMAKE(base-framework-testsuite, 0.9.1)



AC_ARG_WITH(base,
  [  --with-base=DIR         where the Base Framework is installed],
  [
     ac_base_include="${withval}/include"
     ac_base_lib="${withval}/lib"
  ])

AC_ARG_WITH(base-include,
  [  --with-base-include=DIR location of the Base Framework includes],
  [
    ac_base_include="${withval}"
  ])

AC_ARG_WITH(base-lib,
  [  --with-base-lib=DIR     location of the Base Framework libraries],
  [
    ac_base_lib="${withval}"
  ])

if test "x${ac_base_include}" != "x"; then
  CPPFLAGS="${CPPFLAGS}${CPPFLAGS:+ }-I${ac_base_include}"
fi
if test "x${ac_base_lib}" != "x"; then
  LDFLAGS="${LDFLAGS}${LDFLAGS:+ }-L${ac_base_lib}"
fi

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(orb/Makefile)
AC_CONFIG_FILES(ui/Makefile)

AC_OUTPUT()
