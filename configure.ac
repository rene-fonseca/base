dnl ***************************************************************************
dnl    The Base Framework
dnl    A framework for developing platform independent applications
dnl
dnl    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
dnl
dnl    This framework is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl
dnl    For the licensing terms refer to the file 'LICENSE'.
dnl ***************************************************************************

dnl Process this file with autoconf to produce a configure script

AC_INIT(Makefile.am)
AC_CONFIG_SRCDIR(base/features.h)

dnl TAG: protect against absolute path in srcdir

ac_base_build_date=`date -u +"%D %T UTC"`
ac_base_build_user=`whoami`
ac_base_build_host=`uname -n`

AC_PREREQ(2.52)
AC_COPYRIGHT([Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>])
AC_REVISION($Revision$)
AC_CANONICAL_SYSTEM()
dnl perform program name transformation
AC_ARG_PROGRAM()

AM_INIT_AUTOMAKE(base-framework, 0.9.1)

dnl Arguments
ac_base_arguments=${ac_configure_args}

dnl Extract general arguments
dnl TAG: may remove too many arguments
ac_base_general_arguments=`echo " ${ac_base_arguments}" | \
  sed -e "s/ --\(enable\|disable\)\(-[[a-z]]\+\)\+\(=[[^ =]]*\)\?//g" \
      -e "s/ [[a-zA-Z]][[a-zA-Z_-]]*='[[^ ']]*'//g" \
      -e "s/ [[a-zA-Z]][[a-zA-Z_-]]*=[[^ ']]*//g" \
      -e "s/ '[[^'=]]\+=[[^']]*'//g"`
BASE_MODULE_ARGUMENTS=${ac_base_general_arguments}

dnl AC_CHECK_COMPILERS()
dnl AC_EXEEXT() dnl TAG: required before 2.50
AC_PROG_MAKE_SET()
AC_PROG_CXX()
dnl AC_CHECK_PROGS(CXX, ${CCC} c++ g++ gcc CC cxx cc++ cl, gcc)
dnl AC_ENABLE_SHARED(yes)
dnl AC_ENABLE_STATIC(yes)
AC_LIBTOOL_WIN32_DLL()
AC_PROG_LIBTOOL()



ac_base_config=configuration.tmp
rm -f ${ac_base_config}

dnl AC_BASE_DEFINE(symbol name, value)
AC_DEFUN(AC_BASE_DEFINE, [dnl
ifelse([$2], , ac_base_temp="_DK_SDU_MIP__BASE__$1", ac_base_temp="_DK_SDU_MIP__BASE__$1 $2")
cat >> ${ac_base_config} << EOF
[#define] ${ac_base_temp}
EOF
])

dnl AC_BASE_DEFINE_FUNCTION(function name)
AC_DEFUN(AC_BASE_DEFINE_FUNCTION, [dnl
AC_BASE_DEFINE(["HAVE_`echo $1 | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ`"])
])

dnl AC_BASE_EQUAL(left value, right value, equal action)
AC_DEFUN(AC_BASE_EQUAL, [dnl
if test "$1" = "$2"; then
  $3
fi
])

dnl AC_BASE_NOTEQUAL(left value, right value, not equal action)
AC_DEFUN(AC_BASE_NOTEQUAL, [dnl
if test "$1" != "$2"; then
  $3
fi
])

dnl AC_BASE_COMPARE(left value, right value, equal action, unequal action)
AC_DEFUN(AC_BASE_COMPARE, [dnl
if test "$1" = "$2"; then
  $3
else
  $4
fi
])

dnl AC_BASE_MKYESNO(variable)
AC_DEFUN(AC_BASE_MKYESNO, [dnl
if test "${ac_base_status}" = "0"; then $1=yes; else $1=no; fi dnl
])

dnl AC_BASE_YES(variable, yes action)
AC_DEFUN(AC_BASE_YES, [dnl
AC_BASE_EQUAL([$]$1, yes, $2)
])

dnl AC_BASE_NO(variable, no action)
AC_DEFUN(AC_BASE_NO, [dnl
AC_BASE_NOTEQUAL([$]$1, yes, $2)
])

dnl AC_BASE_YESNO(variable, yes action, no action)
AC_DEFUN(AC_BASE_YESNO, [AC_BASE_COMPARE([$]$1, yes, $2, $3)])

dnl AC_BASE_NOT(dest variable, source variable)
AC_DEFUN(AC_BASE_NOT, [dnl
if test "[$]$2" = no; then $1=yes; else $1=no; fi dnl
])

dnl AC_BASE_CHECK_ARG(result variable, valid argument values, error message)
AC_DEFUN(AC_BASE_CHECK_ARG, [dnl
ac_base_arg_valid=no
for ac_base_temp in $2; do
  if test "[$]ac_base_temp" = "[$]enableval"; then
    ac_base_arg_valid=yes
    break
  fi
done
AC_BASE_NOTEQUAL(${ac_base_arg_valid}, yes, AC_MSG_ERROR($3))
$1=[$]enableval
])

dnl AC_BASE_PREPEND(variable, value)
AC_DEFUN(AC_BASE_PREPEND, [dnl
$1="$2[$]{$1:+ }[$]{$1}"
])

dnl AC_BASE_APPEND(variable, value)
AC_DEFUN(AC_BASE_APPEND, [dnl
$1="[$]{$1}[$]{$1:+ }$2"
])

dnl Specifies the required order of libraries (some libraries may not be present)
ac_base_system_lib_order="-lrt -lpthread -ldl -lm -lc"

dnl AC_BASE_ADDLIB(library list) e.g. AC_BASE_ADDLIB(-lxml -ldl)
dnl AC_DEFUN(AC_BASE_ADDLIB, [dnl
dnl for ac_base_temp in [$1]; do
dnl   AC_BASE_APPEND(BASE_LIBS, [$]ac_base_temp)
dnl done
dnl ])

dnl AC_BASE_ADDLIB(library list) e.g. AC_BASE_ADDLIB(-lxml -ldl)
AC_DEFUN(AC_BASE_ADDLIB, [dnl
BASE_LIBS="[$]BASE_LIBS $1"
ac_base_system_libs=
for ac_base_lib in [$]ac_base_system_lib_order; do
  for ac_base_lib in [$]BASE_LIBS; do
    if test "[$]ac_base_lib" = "[$]ac_base_system_lib"; then
      AC_BASE_APPEND(ac_base_system_libs, [$]ac_base_system_lib)dnl
      break
    fi
  done
done
ac_base_ignore_libs=[$]ac_base_system_libs
ac_base_unknown_libs=
for ac_base_lib in [$]BASE_LIBS; do
  ac_base_ignore_lib=no
  for ac_base_temp in [$]ac_base_ignore_libs; do
    if test "[$]ac_base_temp" = "[$]ac_base_lib"; then
      ac_base_ignore_lib=yes
      break
    fi
  done
  if test [$]ac_base_ignore_lib != yes; then
    AC_BASE_APPEND(ac_base_ignore_libs, [$]ac_base_lib)
    AC_BASE_APPEND(ac_base_unknown_libs, [$]ac_base_lib)dnl
  fi
done
BASE_LIBS="[$]{ac_base_unknown_libs}[$]{ac_base_unknown_libs:+ }[$]{ac_base_system_libs}"
])



ac_base_flavour=unix
AC_ARG_ENABLE(
  flavour,[
  --enable-flavour=[unix|win32]
      This feature explicitly specifies the target flavour. This is only
      required if the particular platform supports more than one flavour. The
      default is 'unix'.],
  AC_BASE_CHECK_ARG(ac_base_flavour, unix win32, The flavour flag must be either 'unix' or 'win32'.))

ac_base_platform=
case "${ac_cv_target}" in
i[[3456]]86-pc-cygwin)
  ac_base_platform="i386-pc-cygwin.h"
;;
i[[3456]]86-pc-linux*)
  ac_base_platform="i386-pc-gnu-linux.h"
  AC_BASE_ADDLIB(-ldl)
;;
ia64-pc-linux*)
  ac_base_platform="ia64-pc-gnu-linux.h"
;;
mips-sgi-irix6.5)
  dnl minor architecture?
  ac_base_platform="mips-sgi-irix65.h"
  AC_BASE_ADDLIB(-ldl)
;;
sparc-sun-solaris*)
  dnl minor architecture?
  ac_base_platform="sparc-sun-solaris.h"
  AC_BASE_ADDLIB(-ldl)
;;
i[[3456]]86-*-mingw32)
  ac_base_flavour=win32
  ac_base_platform="i386-pc-winnt4.h"
;;
*)
  AC_MSG_ERROR(This target platform is not directly supported by the framework. Please refer to the section describing how to manually configure the framework.)
;;
esac

if (test -f base/features.h) then
  dnl We are building directly in the source tree

  rm -f base/platforms/platform.h
  ${LN_S} ${ac_base_platform} base/platforms/platform.h
else
  dnl We are building in a separate directory tree

  mkdir -p base/platforms
  rm -f base/platforms/platform.h
  ${LN_S} ../../${srcdir}/base/platforms/${ac_base_platform} base/platforms/platform.h
fi



dnl Manual configuration

base_explicit_debug=no
AC_ARG_ENABLE(
  debug,[
  --enable-debug=[yes|no]
      This feature enables additional runtime checks. The feature is
      disabled by default.],
  AC_BASE_CHECK_ARG(base_explicit_debug, yes no, The debug flag must be either 'yes' or 'no'.))
AC_BASE_YES(base_explicit_debug, AC_BASE_DEFINE(DEBUG))

base_explicit_memory=no
AC_ARG_ENABLE(
  memory,[
  --enable-memory=[yes|no]
      This feature enables additional runtime checks for dynamic memory
      allocation and deallocation. The feature is disabled by default.],
  AC_BASE_CHECK_ARG(base_explicit_memory, yes no, The debug flag must be either 'yes' or 'no'.))
AC_BASE_YES(base_explicit_memory, AC_BASE_DEFINE(MEMORY))

base_explicit_trace=no
AC_ARG_ENABLE(
  trace,[
  --enable-trace=[yes|no]
      Specifies that selected functions should dump tracing information to
      standard error. This feature in only used to a locate bug within the
      framework. This feature is disabled by default.],
  AC_BASE_CHECK_ARG(base_explicit_trace, yes no, The trace flag must be either 'yes' or 'no'.))
AC_BASE_YES(base_explicit_trace, AC_BASE_DEFINE(TRACE))

base_explicit_namespace=yes
AC_ARG_ENABLE(
  namespace,[
  --enable-namespace=[yes|no]
      Specifies that the symbols of this framework should by placed in the
      namespace 'base'. Disabling this feature will put the symbols into the
      global namespace. This feature is enabled by default.],
  AC_BASE_CHECK_ARG(base_explicit_namespace, yes no, The namespace flag must be either 'yes' or 'no'.))
AC_BASE_YES(base_explicit_namespace, AC_BASE_DEFINE(NAMESPACE))

base_explicit_demangle=auto
AC_ARG_ENABLE(
  demangle,[
  --enable-demangle=[auto|v3mv|gccv3|gccv2|sunwspro|mipspro|no]
      Specifies that support for type name demangling should be included for
      the given compiler/ABI. If this option is not specified the demangling
      API will be auto-detected. 'v3mv' selects builtin support for the V3
      multi-vendor C++ standard ABI. 'gccv3' and 'gccv2' enables support for
      GCC version 3.0 and 2.95, respectively, using the libiberty library.
      'sunwspro' enables support for the SUN WorkShop C++ compiler and
      'mipspro' enables support for the SGI MIPSpro Compiler.],
  AC_BASE_CHECK_ARG(base_explicit_demangle, auto v3mv gccv3 gccv2 sunwspro mipspro no, Invalid argument used with the demangle flag.))

base_explicit_threads=auto
AC_ARG_ENABLE(
  threads,[
  --enable-threads=[auto|posix|win32]
      Specifies the thread API to be utilized. This feature is auto-detected
      by default. The auto-detection will fail if the compiler is a cross
      compiler. The option is only valid in Unix mode. The default is posix.],
  AC_BASE_CHECK_ARG(base_explicit_threads, auto posix win32, The
  threads flag must be either 'auto', 'posix', or 'win32'.))

base_explicit_wide=auto
AC_ARG_ENABLE(
  unicode,[
  --enable-wide=[auto|yes|no]
      Specifies whether wide-character support should be utilized. This
      feature is auto-detected by default for non cross compilers.],
  AC_BASE_CHECK_ARG(base_explicit_wide, auto yes no, Invalid argument used with the wide-character flag.))

base_explicit_inet_ipv6=auto
AC_ARG_ENABLE(
  ipv6,[
  --enable-ipv6=[auto|yes|no]
      Specifies whether the framework should use the interface extensions for
      IP version 6 or not. If the protocol version is not specified then IPv6
      will be used if available. Automatic detection will fail if using a
      cross compiler.],
  AC_BASE_CHECK_ARG(base_explicit_inet_ipv6, auto yes no, IP version must be either 'auto', 'yes', or 'no'.))

base_explicit_byte_order=auto
AC_ARG_ENABLE(
  byte-order,[
  --enable-byte-order=[auto|big|little]
      Specifies the byte order of the target architecture. The byte order will
      be detected by default. The byte order detection, however, will fail if
      the compiler is a cross compiler.],
  AC_BASE_CHECK_ARG(base_explicit_byte_order, auto big little, The byte order must be either 'auto', 'big', or 'little'.))

base_explicit_word_order=auto
AC_ARG_ENABLE(
  word-order,[
  --enable-word-order=[auto|big|little]
      Specifies the word order of the target architecture. The word order
      refers to the order of the individual words (native of the architecture)
      within a multi-word primitive. The order will be detected by default if 
      the compiler isn't a cross compiler.],
  AC_BASE_CHECK_ARG(base_explicit_word_order, auto big little, The word order must be either 'auto', 'big', or 'little'.))

base_explicit_large_file_system=auto
AC_ARG_ENABLE(
  large-file-system,[
  --enable-large-file-system=[auto|yes|no]
      Specifies that the Large File System (LFS) should be utilized. This
      feature is auto-detected by default. The auto-detection will fail if the
      compiler is a cross compiler. The option is only valid in Unix mode.],
  AC_BASE_CHECK_ARG(base_explicit_large_file_system, auto yes no, The Large File System (LFS) flag must be either 'auto', 'yes', or 'no'.))

base_explicit_regexp=auto
AC_ARG_ENABLE(
  regexp,[
  --enable-regexp=[auto|pcre|posix|no]
      Specifies that regular expression support should be included. 'pcre'
      selects Perl-Compatible Regular Expression library. 'posix' selects POSIX
      compatible regular expression support provided by the C library. The
      default is no.],
  AC_BASE_CHECK_ARG(base_explicit_regexp, auto pcre posix no, Invalid argument used with the regexp flag.))

base_explicit_xml=no
AC_ARG_ENABLE(
  xml,[
  --enable-xml=[xmlsoft|no]
      Specifies that XML support should be included. 'xmlsoft' selects the
      Gnome XML Library. The default is no.],
  AC_BASE_CHECK_ARG(base_explicit_xml, xmlsoft no, Invalid argument used with the xml flag.))

base_explicit_testsuite=no
AC_ARG_ENABLE(
  testsuite,[
  --enable-testsuite=[auto|yes|no]
      This option specifies whether or not the framework test suite should be
      configured automatically. By default the test suite will be configured
      if it is available.],
  AC_BASE_CHECK_ARG(base_explicit_testsuite, auto yes no, The test suite option must be either 'auto', 'yes', or 'no'.))

AC_ARG_ENABLE(
  ,[
  Please report any errors to <fonseca@mip.sdu.dk>.])



dnl Checks for libraries.

dnl AC_BASE_CHECK(check message, action, result variable[, extra variable])
AC_DEFUN(AC_BASE_CHECK, [dnl
ac_base_temp="$1"
AC_MSG_CHECKING(${ac_base_temp})
$2
ifelse([$4], , AC_MSG_RESULT([$]$3), [dnl
dnl if test "[$]$3" = yes; then
  if test -n "[$]$4"; then
    AC_MSG_RESULT([[$]$3 ([$]$4)])
  else
    AC_MSG_RESULT([$]$3)
  fi
])dnl
])

dnl AC_BASE_CACHE(variable list, action that sets all the specified variables)
AC_DEFUN(AC_BASE_CACHE, [dnl
ac_temp_cached=yes
for ac_base_temp in $1; do
  if eval "test \"`echo '$''{'$ac_base_temp'+set}'`\" != set"; then
    ac_temp_cached=no
    break
  fi
done
AC_BASE_YESNO(ac_temp_cached, [echo ${ECHO_N} "(cached) ${ECHO_C}" >&AC_FD_MSG], $2)
])

dnl AC_BASE_ATTEMPT(result variable, value variable, value list, action setting result)
AC_DEFUN(AC_BASE_ATTEMPT, [dnl
for $2 in $3; do
  $4
  test "[$]$1" = yes && break
done
test "[$]$1" != yes && $2=
])



base_src_temp="test_source"
base_obj_temp="test_object${ac_cv_objext}"
base_app_temp="test_app${ac_cv_exeext}"
base_cleanup="rm -f ${base_src_temp} ${base_obj_temp} ${base_app_temp}"
base_run="./${base_app_temp}"

dnl C++ specific
base_cxx_ext=cpp
base_cxx_preprocess='${CXXCPP} -I. -I${srcdir} ${CPPFLAGS}'
base_cxx_compile='${CXX} -o ${base_obj_temp} -c -I. -I${srcdir} ${CPPFLAGS} ${CXXFLAGS} ${srcdir}/config/${base_cxx_src} ${base_cxx_opts}'
base_cxx_link='${CXX} -o ${base_app_temp} -I. -I${srcdir} ${CPPFLAGS} ${CXXFLAGS} ${srcdir}/config/${base_cxx_src} ${base_cxx_opts} ${LDFLAGS} ${LIBS} ${BASE_LIBS}'

dnl AC_BASE_TRIM(string)
AC_DEFUN(AC_BASE_TRIM, [`echo $1`])

dnl AC_BASE_TRY_EXEC(variable)
AC_DEFUN(AC_BASE_TRY_EXEC, [ { eval [$]$1 >&AC_FD_CC 2>&AC_FD_CC; ac_base_status=`echo $?`; } ])

dnl AC_BASE_TRY_CMD(command)
AC_DEFUN(AC_BASE_TRY_CMD, [ { ac_base_temp='$1'; AC_BASE_TRY_EXEC(ac_base_temp); } ])

dnl AC_BASE_TRY_CXX_COMPILE(result variable, source file, additional options)
AC_DEFUN(AC_BASE_TRY_CXX_COMPILE, [dnl
if (test ! -r ${srcdir}/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_compile)
AC_BASE_MKYESNO([$1])
])

dnl AC_BASE_TRY_CXX_LINK(result variable, source file, additional options)
AC_DEFUN(AC_BASE_TRY_CXX_LINK, [dnl
if (test ! -r ${srcdir}/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_link)
AC_BASE_MKYESNO([$1])
])

dnl AC_BASE_TRY_CXX_RUN(result var, src file, add options[, cross compiler message])
define(AC_BASE_TRY_CXX_RUN, [dnl
[$1]=no
AC_BASE_YES(
  base_cv_cxx_cross,
  ifelse([$4], , AC_MSG_ERROR(C++ compiler is a cross compiler), AC_MSG_ERROR([$4]))dnl
)
if (test ! -r ${srcdir}/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_link)
AC_BASE_MKYESNO(ac_base_temp)
AC_BASE_YES(
  ac_base_temp,
  AC_BASE_TRY_EXEC(base_run)
  AC_BASE_MKYESNO([$1]))
])

dnl AC_BASE_TRY_CXX_RUNSTATUS(result var, src file, add options[, cross compiler message])
define(AC_BASE_TRY_CXX_RUNSTATUS, [dnl
[$1]=
AC_BASE_YES(
  base_cv_cxx_cross,
  ifelse([$4], , AC_MSG_ERROR(C++ compiler is a cross compiler), AC_MSG_ERROR([$4]))dnl
)
if (test ! -r ${srcdir}/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_link)
AC_BASE_MKYESNO(ac_base_temp)
AC_BASE_YESNO(
  ac_base_temp,
  AC_BASE_TRY_EXEC(base_run)
  [$1]=${ac_base_status},
  AC_MSG_ERROR(Unable to compile and link testing application.))
])

dnl AC_BASE_CHECK_CFUNCTION(name)
AC_DEFUN(AC_BASE_CHECK_CFUNCTION, [dnl
AC_BASE_CHECK(
  whether API function $1 is available,
  AC_BASE_CACHE(
    base_cv_function_$1,
    AC_BASE_TRY_CXX_LINK(base_cv_function_$1, $1.c)
  ),
  base_cv_function_$1)
AC_BASE_YES(base_cv_function_$1, AC_BASE_DEFINE_FUNCTION($1))
])



AC_BASE_CHECK(
  whether C++ compiler (AC_BASE_TRIM(${CXX} ${CPPFLAGS} ${CXXFLAGS})) works,
  AC_BASE_TRY_CXX_LINK(base_cxx_compiler_works, cross++.cpp),
  base_cxx_compiler_works)

dnl Additional libraries to test
base_additional_attempts=
AC_BASE_EQUAL(${ac_base_flavour}, unix, AC_BASE_APPEND(base_additional_attempts, -lpthread))

AC_BASE_CHECK(
  whether C++ compiler (AC_BASE_TRIM(${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} ${LIBS} ${BASE_LIBS})) is a cross compiler,
  AC_BASE_CACHE(
    base_cv_cxx_cross base_cv_cxx_libs,
    AC_BASE_ATTEMPT(
      base_cxx_not_cross,
      base_cv_cxx_libs,
      "" [${base_additional_attempts}],
      AC_BASE_TRY_CXX_RUN(base_cxx_not_cross, cross++.cpp, ${base_cv_cxx_libs})dnl
    )dnl
    AC_BASE_NOT(base_cv_cxx_cross, base_cxx_not_cross)dnl
  ),
  base_cv_cxx_cross,
  base_cv_cxx_libs)
AC_BASE_ADDLIB(${base_cv_cxx_libs})

AC_BASE_CHECK(
  whether Standard Template Library is working,
  AC_BASE_CACHE(
    base_cv_stl_available base_cv_stl_libs,
    AC_BASE_ATTEMPT(
      base_cv_stl_available,
      base_cv_stl_libs,
      "" ${base_additional_attempts},
      AC_BASE_TRY_CXX_RUN(base_cv_stl_available, stdc++.cpp, ${base_cv_stl_libs})dnl
    )
  ),
  base_cv_stl_available,
  base_cv_stl_libs)
AC_BASE_ADDLIB(${base_cv_stl_libs})



dnl Lets play name that c++ compiler

if test "x$ac_base_cxx_name" = "x"; then
  # Check if GNU Compiler Collection
  if AC_BASE_TRY_CMD($CXX -v 2>&1 | grep -e "gcc.*version"); then
    ac_base_cxx_major=`$CXX -dumpversion 2>&1 | sed "s/\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -dumpversion 2>&1 | sed "s/[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="GNU Compiler Collection"
    ac_base_cxx_vendor="gnu.org"
  fi
fi

if test "x$ac_base_cxx_name" = "x"; then
  # Check if Sun WorkShop Compiler
  if AC_BASE_TRY_CMD($CXX -V 2>&1 | grep -e "Sun.*WorkShop"); then
    ac_base_cxx_major=`$CXX -V 2>&1 | sed "s/.*C++.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -V 2>&1 | sed "s/.*C++.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="Sun WorkShop"
    ac_base_cxx_vendor="sun.com"
  fi
fi

if test "x$ac_base_cxx_name" = "x"; then
  # Check if MIPSpro Compiler
  if AC_BASE_TRY_CMD($CXX -version 2>&1 | grep -e "MIPSpro.*Compilers"); then
    ac_base_cxx_major=`$CXX -version 2>&1 | sed "s/.*Version.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -version 2>&1 | sed "s/.*Version.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="MIPSpro Compilers"
    ac_base_cxx_vendor="sgi.com"
  fi
fi

if test "x$ac_base_cxx_name" = "x"; then
  # Check if Intel Compiler
  if AC_BASE_TRY_CMD($CXX -V 2>&1 | grep -e "Intel.*C++.*Compiler"); then
    ac_base_cxx_major=`$CXX -V 2>&1 | sed "s/.*Version.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -V 2>&1 | sed "s/.*Version.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="Intel Compiler"
    ac_base_cxx_vendor="intel.com"
  fi
fi

AC_MSG_CHECKING(identity of c++ compiler)
AC_MSG_RESULT([$ac_base_cxx_name $ac_base_cxx_major.$ac_base_cxx_minor ($ac_base_cxx_vendor)])



dnl Explicit features.

if test "${base_explicit_demangle}" = auto; then
  base_demangle=no
  case "${ac_base_cxx_vendor}/${ac_base_cxx_name}" in
  "gnu.org/GNU Compiler Collection")
    if test "x$ac_base_cxx_major" = "x3"; then
      base_demangle=v3mv
    elif test "x$ac_base_cxx_major" = "x2"; then
      base_demangle=gccv2
    fi
  ;;
  "sun.com/SUN WorkShop")
    base_demangle=sunwspro
  ;;
  "sgi.com/MIPSpro Compilers")
    base_demangle=mipspro
  ;;
  "intel.com/Intel Compiler")
  ;;
  esac
else
  base_demangle=${base_explicit_demangle}
fi

if test "${base_demangle}" = v3mv; then
  AC_BASE_DEFINE(DEMANGLE_V3MV)
elif test "${base_demangle}" = gccv3; then
  AC_BASE_DEFINE(DEMANGLE_GCCV3)
  AC_BASE_ADDLIB(-liberty)
elif test "${base_demangle}" = gccv2; then
  AC_BASE_DEFINE(DEMANGLE_GCCV2)
  AC_BASE_ADDLIB(-liberty)
elif test "${base_demangle}" = sunwspro; then
  AC_BASE_DEFINE(DEMANGLE_SUNWSPRO)
  AC_BASE_ADDLIB(-ldemangle)
elif test "${base_demangle}" = mipspro; then
  AC_BASE_DEFINE(DEMANGLE_MIPSPRO)
  AC_BASE_ADDLIB(-lmangle)
fi

AC_MSG_CHECKING(selected demangling API)
AC_MSG_RESULT([$base_demangle])



if test "${base_explicit_threads}" = auto; then
  AC_BASE_EQUAL(${ac_base_flavour}, unix, base_explicit_threads=posix)
fi

if test "${base_explicit_xml}" = xmlsoft; then
  AC_BASE_DEFINE(XML_XMLSOFT_ORG)
  AC_BASE_ADDLIB(-lxml -lz)
fi

dnl Check primitives.

AC_BASE_CHECK(
  whether char is unsigned,
  AC_BASE_CACHE(
    base_cv_unsigned_char,
    AC_BASE_TRY_CXX_RUN(base_cv_unsigned_char, unsigned_char.cpp)dnl
  ),
  base_cv_unsigned_char)
AC_BASE_YES(base_cv_unsigned_char, AC_BASE_DEFINE(UNSIGNED_CHAR))

AC_BASE_CHECK(
  size of char,
  AC_BASE_CACHE(
    base_cv_char_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_char_size, char_size.cpp)dnl
  ),
  base_cv_char_size)
AC_BASE_DEFINE(CHAR_SIZE, ${base_cv_char_size})

AC_BASE_CHECK(
  size of short,
  AC_BASE_CACHE(
    base_cv_short_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_short_size, short_size.cpp)dnl
  ),
  base_cv_short_size)
AC_BASE_DEFINE(SHORT_SIZE, ${base_cv_short_size})

AC_BASE_CHECK(
  size of int,
  AC_BASE_CACHE(
    base_cv_int_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_int_size, int_size.cpp)dnl
  ),
  base_cv_int_size)
AC_BASE_DEFINE(INT_SIZE, ${base_cv_int_size})

AC_BASE_CHECK(
  size of long,
  AC_BASE_CACHE(
    base_cv_long_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_long_size, long_size.cpp)dnl
  ),
  base_cv_long_size)
AC_BASE_DEFINE(LONG_SIZE, ${base_cv_long_size})

AC_BASE_CHECK(
  size of long long,
  AC_BASE_CACHE(
    base_cv_long_long_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_long_long_size, long_long_size.cpp)dnl
  ),
  base_cv_long_long_size)
AC_BASE_DEFINE(LONG_LONG_SIZE, ${base_cv_long_long_size})

AC_BASE_CHECK(
  size of pointer,
  AC_BASE_CACHE(
    base_cv_pointer_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_pointer_size, pointer_size.cpp)dnl
  ),
  base_cv_pointer_size)
AC_BASE_DEFINE(POINTER_SIZE, ${base_cv_pointer_size})

dnl AC_BASE_CHECK(
dnl   size of wchar_t,
dnl   AC_BASE_CACHE(
dnl     base_cv_wchar_t_size,
dnl     AC_BASE_TRY_CXX_RUNSTATUS(base_cv_wchar_t_size, wchar_t_size.cpp)dnl
dnl   ),
dnl   base_cv_wchar_t_size)
dnl AC_BASE_DEFINE(WCHAR_T_SIZE, ${base_cv_wchar_t_size})



dnl AC_BASE_VALUE2BYTEORDER(var result symbol, var to receive result message of check, value)
AC_DEFUN(AC_BASE_VALUE2BYTEORDER, [dnl
  if test "[$3]" = 1; then
    [$2]="big endian"
    [$1]="_DK_SDU_MIP__BASE__BIG_ENDIAN"
  elif test "[$3]" = 2; then
    [$2]="little endian"
    [$1]="_DK_SDU_MIP__BASE__LITTLE_ENDIAN"
  else
    [$2]="unspecified"
    [$1]="_DK_SDU_MIP__BASE__UNSPECIFIED"
  fi
])

AC_BASE_COMPARE(
  ${base_explicit_byte_order}, auto,
  AC_BASE_CHECK(
    byte order of architecture,
    AC_BASE_CACHE(
      base_cv_byte_order base_cv_byte_order_msg,
      AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, byteorder.c, , Auto-detection of byte order does not work with cross compiler. Please specify byte order explicitly using --enable-byte-order.)dnl
      AC_BASE_VALUE2BYTEORDER(base_cv_byte_order, base_cv_byte_order_msg, ${ac_base_temp})dnl
    ),
    base_cv_byte_order_msg),
  base_cv_byte_order=${base_explicit_byte_order})
AC_BASE_DEFINE(BYTE_ORDER, ${base_cv_byte_order})

AC_BASE_COMPARE(
  ${base_explicit_word_order}, auto,
  AC_BASE_CHECK(
    word order of architecture/operating system/compiler,
    AC_BASE_CACHE(
      base_cv_word_order,
      AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, float_word_order.cpp, , Auto-dection of word order does not work with cross compiler.)
      AC_BASE_VALUE2BYTEORDER(base_cv_word_order, base_cv_word_order_msg, ${ac_base_temp})
    ),
    base_cv_word_order_msg),
  base_cv_word_order=${base_explicit_word_order})
AC_BASE_DEFINE(FLOAT_WORD_ORDER, ${base_cv_word_order})



AC_DEFUN(AC_BASE_VALUE2FLOAT, [dnl
  if test "[$3]" = 1; then
    [$2]="ieee 754 single"
    [$1]="_DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION"
  elif test "[$3]" = 2; then
    [$2]="ieee 754 double"
    [$1]="_DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION"
  elif test "[$3]" = 3; then
    [$2]="ieee extended 96"
    [$1]="_DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96"
  elif test "[$3]" = 4; then
    [$2]="ieee extended 128"
    [$1]="_DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128"
  elif test "[$3]" = 5; then
    [$2]="ieee quadruple"
    [$1]="_DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION"
  else
    [$2]="unspecified"
    [$1]="_DK_SDU_MIP__BASE__UNSPECIFIED"
  fi
])

AC_BASE_CHECK(
  representation of float,
  AC_BASE_CACHE(
    base_cv_float base_cv_float_msg,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, float.cpp, )dnl
    AC_BASE_VALUE2FLOAT(base_cv_float, base_cv_float_msg, ${ac_base_temp})dnl
  ),
  base_cv_float_msg)
AC_BASE_DEFINE(FLOAT, ${base_cv_float})

AC_BASE_CHECK(
  representation of double,
  AC_BASE_CACHE(
    base_cv_double base_cv_doublestr,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, double.cpp, )dnl
    AC_BASE_VALUE2FLOAT(base_cv_double, base_cv_doublestr, ${ac_base_temp})dnl
  ),
  base_cv_doublestr)
AC_BASE_DEFINE(DOUBLE, ${base_cv_double})

AC_BASE_CHECK(
  representation of long double,
  AC_BASE_CACHE(
    base_cv_long_double base_cv_long_doublestr,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, long_double.cpp, )dnl
    AC_BASE_VALUE2FLOAT(base_cv_long_double, base_cv_long_doublestr, ${ac_base_temp})dnl
  ),
  base_cv_long_doublestr)
AC_BASE_DEFINE(LONG_DOUBLE, ${base_cv_long_double})



dnl Check for language features.

AC_BASE_CHECK(
  whether bool primitive is supported by the compiler,
  AC_BASE_CACHE(
    base_cv_cpp_bool,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_bool, bool.cpp)dnl
  ),
  base_cv_cpp_bool)
AC_BASE_YES(base_cv_cpp_bool, AC_BASE_DEFINE(CPP_BOOL))

AC_BASE_CHECK(
  whether const qualifier is supported by the compiler,
  AC_BASE_CACHE(
    base_cv_cpp_const,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_const, const.cpp)dnl
  ),
  base_cv_cpp_const)
AC_BASE_YES(base_cv_cpp_const, AC_BASE_DEFINE(CPP_CONST))

AC_BASE_CHECK(
  whether restrict qualifier is supported by the compiler,
  AC_BASE_CACHE(
    base_cv_cpp_restrict,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_restrict, restrict.cpp)dnl
  ),
  base_cv_cpp_restrict)
AC_BASE_YES(base_cv_cpp_restrict, AC_BASE_DEFINE(CPP_RESTRICT))

AC_BASE_CHECK(
  whether inline specifier is supported by the compiler,
  AC_BASE_CACHE(
    base_cv_cpp_inline,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_inline, inline.cpp)dnl
  ),
  base_cv_cpp_inline)
AC_BASE_YES(base_cv_cpp_inline, AC_BASE_DEFINE(CPP_INLINE))

AC_BASE_CHECK(
  whether static const with init is supported inside class def,
  AC_BASE_CACHE(
    base_cv_cpp_static_const,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_static_const, static_const.cpp)dnl
  ),
  base_cv_cpp_static_const)
AC_BASE_YES(base_cv_cpp_static_const, AC_BASE_DEFINE(CPP_STATIC_CONST))

AC_BASE_CHECK(
  whether anonymous struct is supported by the compiler,
  AC_BASE_CACHE(
    base_cv_cpp_anonymous_struct,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_anonymous_struct, anonymous_struct.cpp)dnl
  ),
  base_cv_cpp_anonymous_struct)
AC_BASE_YES(base_cv_cpp_anonymous_struct, AC_BASE_DEFINE(CPP_ANONYMOUS))

AC_BASE_CHECK(
  whether compiler supports exception specifications,
  AC_BASE_CACHE(
    base_cv_cpp_specification,
    AC_BASE_TRY_CXX_COMPILE(base_cv_cpp_specification, specification.cpp)dnl
  ),
  base_cv_cpp_specification)
AC_BASE_YES(base_cv_cpp_specification, AC_BASE_DEFINE(CPP_SPECIFICATION))



dnl Checks for typedefs, structures, and compiler characteristics.

if test "${ac_base_flavour}" = unix; then
AC_BASE_CHECK(
  whether the type socklen_t is available,
  AC_BASE_CACHE(
    base_cv_type_socklen_t,
    AC_BASE_TRY_CXX_LINK(base_cv_type_socklen_t, socklen_t.c)dnl
  ),
  base_cv_type_socklen_t)
AC_BASE_YES(base_cv_type_socklen_t, AC_BASE_DEFINE(SOCKLEN_T))
fi dnl end of unix



dnl Checks for library functions.

dnl Handle network support
case "${ac_base_flavour}" in
unix)
  if test "${base_explicit_inet_ipv6}" = auto; then
    AC_BASE_CHECK(
      whether IPv6 is supported by the operating system,
      AC_BASE_CACHE(
        base_cv_inet_ipv6 base_cv_inet_ipv6_libs,
        AC_BASE_ATTEMPT(
          base_cv_inet_ipv6,
          base_cv_inet_ipv6_libs,
          "" -lsocket "-lsocket -lnsl",
          AC_BASE_TRY_CXX_RUN(base_cv_inet_ipv6, ipv6.c, ${base_cv_inet_ipv6_libs}, Auto-detection of IPv6 support does not work with cross compiler.)dnl
        )
      ),
      base_cv_inet_ipv6,
      base_cv_inet_ipv6_libs)
    base_explicit_inet_ipv6=${base_cv_inet_ipv6}
  fi
  AC_BASE_YES(
    base_explicit_inet_ipv6,
    AC_BASE_DEFINE(INET_IPV6)
    AC_BASE_ADDLIB(${base_cv_inet_ipv6_libs}))
;;
win32)
  BASE_LIBADD="${BASE_LIBADD}${BASE_LIBADD:+ }"'-lws2_32'
;;
esac



dnl Handle sound support
case "${ac_base_flavour}" in
unix)
;;
win32)
  BASE_LIBADD="${BASE_LIBADD}${BASE_LIBADD:+ }"'-lwinmm'
;;
esac



if test "${ac_base_flavour}" = unix; then
if test ${base_explicit_large_file_system} = auto; then
  AC_BASE_CHECK(
    whether Large File System is supported by the operating system,
    AC_BASE_CACHE(
      base_cv_large_file_system,
      AC_BASE_TRY_CXX_LINK(base_cv_large_file_system, largefile.c, , Auto-detection of Large File System does not work with cross compiler.)
    ),
    base_cv_large_file_system)
  base_explicit_large_file_system=${base_cv_large_file_system}
fi
AC_BASE_YES(base_explicit_large_file_system, AC_BASE_DEFINE(LARGE_FILE_SYSTEM))
fi dnl end of unix



AC_BASE_EQUAL(${base_explicit_threads}, posix,
dnl Check POSIX thread support

AC_BASE_CHECK(
  whether POSIX thread API is supported,
  AC_BASE_CACHE(
    base_cv_pthread base_cv_pthread_libs,
    AC_BASE_ATTEMPT(
      base_cv_pthread,
      base_cv_pthread_libs,
      "" -lpthread,
      AC_BASE_TRY_CXX_RUN(base_cv_pthread, pthread.c, ${base_cv_pthread_libs}, Auto-detection of POSIX thread support does not work with cross compiler.)dnl
    )
  ),
  base_cv_pthread,
  base_cv_pthread_libs)
AC_BASE_NO(base_cv_pthread, AC_MSG_ERROR(The compiler does not support POSIX threads))
AC_BASE_ADDLIB(${base_cv_pthread_libs})

AC_BASE_CHECK(
  whether pthread_yield is provided by the POSIX thread library,
  AC_BASE_CACHE(
    base_cv_pthread_yield base_cv_pthread_yield_libs,
    AC_BASE_ATTEMPT(
      base_cv_pthread_yield,
      base_cv_pthread_yield_libs,
      "" -lpthread,
      AC_BASE_TRY_CXX_LINK(base_cv_pthread_yield, pthread_yield.c, ${base_cv_pthread_yield_libs})dnl
    )
  ),
  base_cv_pthread_yield,
  base_cv_pthread_yield_libs)
AC_BASE_YES(
  base_cv_pthread_yield,
  AC_BASE_DEFINE(PTHREAD_YIELD)
  AC_BASE_ADDLIB(${base_cv_pthread_yield_libs}))

AC_BASE_CHECK(
  whether POSIX semaphore is supported,
  AC_BASE_CACHE(
    base_cv_pthread_semaphore base_cv_pthread_semaphore_libs,
    AC_BASE_ATTEMPT(
      base_cv_pthread_semaphore,
      base_cv_pthread_semaphore_libs,
      "" -lpthread -lrt,
      AC_BASE_TRY_CXX_LINK(base_cv_pthread_semaphore, pthread_semaphore.c, ${base_cv_pthread_semaphore_libs})dnl
    )
  ),
  base_cv_pthread_semaphore,
  base_cv_pthread_semaphore_libs)
AC_BASE_YES(
  base_cv_pthread_semaphore,
  AC_BASE_DEFINE(PTHREAD_SEMAPHORE)
  AC_BASE_ADDLIB(${base_cv_pthread_semaphore_libs}))

AC_BASE_CHECK(
  whether POSIX read-write lock is supported,
  AC_BASE_CACHE(
    base_cv_pthread_rwlock base_cv_pthread_rwlock_libs,
    AC_BASE_ATTEMPT(
      base_cv_pthread_rwlock,
      base_cv_pthread_rwlock_libs,
      "" -lpthread,
      AC_BASE_TRY_CXX_LINK(base_cv_pthread_rwlock, pthread_rwlock.c, ${base_cv_pthread_rwlock_libs})dnl
    )
  ),
  base_cv_pthread_rwlock,
  base_cv_pthread_rwlock_libs)
AC_BASE_YES(base_cv_pthread_rwlock,
  AC_BASE_DEFINE(PTHREAD_RWLOCK)
  AC_BASE_ADDLIB(${base_cv_pthread_rwlock_libs}))

) dnl end of posix thread support



AC_BASE_EQUAL(${base_explicit_regexp}, auto,
  AC_BASE_CHECK(
    whether POSIX Regular Expression interface is available,
    AC_BASE_CACHE(
      base_cv_regex_available,
      AC_BASE_TRY_CXX_RUN(base_cv_regex_available, regex.c)dnl
    ),
    base_cv_regex_available)
  AC_BASE_YES(base_cv_regex_available, AC_BASE_DEFINE(REGEXP_POSIX)))

AC_BASE_EQUAL(${base_explicit_regexp}, posix, AC_BASE_DEFINE(REGEXP_POSIX))

AC_BASE_EQUAL(${base_explicit_regexp}, pcre,
  AC_BASE_DEFINE(REGEXP_PCRE)
  AC_BASE_ADDLIB(-lpcre))



AC_BASE_EQUAL(${base_explicit_wide}, auto,
  AC_BASE_CHECK(
    whether wide-character interface is available,
    AC_BASE_CACHE(
      base_cv_wide_available,
      AC_BASE_TRY_CXX_LINK(base_cv_wide_available, wide.c)dnl
    ),
    base_cv_wide_available)
  AC_BASE_YES(base_cv_wide_available, AC_BASE_DEFINE(WIDE)))
AC_BASE_YES(base_explicit_wide, AC_BASE_DEFINE(WIDE))



dnl Detect whether some functions are available
AC_BASE_CHECK_CFUNCTION(memchr)
AC_BASE_CHECK_CFUNCTION(memcmp)
AC_BASE_CHECK_CFUNCTION(memcpy)
AC_BASE_CHECK_CFUNCTION(memmove)
AC_BASE_CHECK_CFUNCTION(memset)
AC_BASE_CHECK_CFUNCTION(nanosleep)
AC_BASE_CHECK_CFUNCTION(mbrtowc)
AC_BASE_CHECK_CFUNCTION(mbsrtowcs)



dnl Add VERSION resource to the dynamic library on Windows platforms (i.e. cygwin or mingw targets)
case "${ac_cv_target}" in
*-*-cygwin|*-*-mingw32)
  dnl The -Wl option is used to circumvent libtool
  BASE_LIBADD="${BASE_LIBADD}${BASE_LIBADD:+ }"'-Wl,winrcver.o'
  BASE_DEPENDENCIES="${BASE_DEPENDENCIES}${BASE_DEPENDENCIES:+ }"'winrcver.o'
  ;;
esac



dnl Define some describing symbols
AC_BASE_DEFINE(CONFIGURE, [\"]${ac_configure_args}[\"])
AC_BASE_DEFINE(BUILD_DATE, [\"]${ac_base_build_date}[\"])
AC_BASE_DEFINE(BUILD_USER, [\"]${ac_base_build_user}[\"])
AC_BASE_DEFINE(BUILD_HOST, [\"]${ac_base_build_host}[\"])
AC_BASE_DEFINE(BUILD_SYSTEM, [\"]${ac_cv_build}[\"])
AC_BASE_DEFINE(TARGET_SYSTEM, [\"]${ac_cv_target}[\"])

dnl Tell the compiler where it should look for the generated and static framework headers
if (test -f base/features.h) then
  dnl We are building directly in the source tree
  BASE_INCLUDES='-I${top_srcdir}'"${BASE_INCLUDES:+ }${BASE_INCLUDES}"
else
  dnl We are building in a separate directory tree
  dnl Tell the compiler where it should look for the generated and static framework headers
  BASE_INCLUDES='-I${top_builddir} -I${top_srcdir}'"${BASE_INCLUDES:+ }${BASE_INCLUDES}"
fi



dnl Remove temporary files
${base_cleanup}



dnl Configure test suite
ac_base_testsuite=no
if test "${base_explicit_testsuite}" = auto; then
  if (test -d ${srcdir}/testsuite); then
    ac_base_testsuite=yes
  else
    ac_base_testsuite=no
  fi
elif test "${base_explicit_testsuite}" = yes; then
  if (test -d ${srcdir}/testsuite); then
    ac_base_testsuite=yes
  else
    AC_MSG_ERROR("The test suite is not available.")
  fi
fi

if test ${ac_base_testsuite} = yes; then
  BASE_MODULES="${BASE_MODULES}${BASE_MODULES:+ }testsuite"
fi



AC_SUBST(BASE_MODULE_ARGUMENTS)
AC_SUBST(BASE_INCLUDES)
AC_SUBST(BASE_LIBS)
AC_SUBST(BASE_LIBADD)
AC_SUBST(BASE_LDFLAGS)
AC_SUBST(BASE_DEPENDENCIES)
AC_SUBST(BASE_MODULES)



dnl Store the symbols
ac_base_header=base/configuration.h
echo "updating configuration header..."
mkdir -p base
mv ${ac_base_config} ${ac_base_header}



dnl Generate make files
AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(config/Makefile)
AC_CONFIG_FILES(base/Makefile)
AC_CONFIG_FILES(base/docs/Makefile)
AC_CONFIG_FILES(base/docs/en/Makefile)
AC_CONFIG_FILES(base/platforms/Makefile)
AC_CONFIG_FILES(base/collection/Makefile)
AC_CONFIG_FILES(base/communication/Makefile)
AC_CONFIG_FILES(base/concurrency/Makefile)
AC_CONFIG_FILES(base/filesystem/Makefile)
AC_CONFIG_FILES(base/iterator/Makefile)
AC_CONFIG_FILES(base/dl/Makefile)
AC_CONFIG_FILES(base/io/Makefile)
AC_CONFIG_FILES(base/io/async/Makefile)
AC_CONFIG_FILES(base/mathematics/Makefile)
AC_CONFIG_FILES(base/mem/Makefile)
AC_CONFIG_FILES(base/net/Makefile)
AC_CONFIG_FILES(base/security/Makefile)
AC_CONFIG_FILES(base/sound/Makefile)
AC_CONFIG_FILES(base/string/Makefile)
AC_CONFIG_FILES(base/xml/Makefile)

AC_OUTPUT()
