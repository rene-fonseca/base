dnl ***************************************************************************
dnl    The Base Framework
dnl    A framework for developing platform independent applications
dnl
dnl    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
dnl
dnl    This framework is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl
dnl    For the licensing terms refer to the file 'LICENSE'.
dnl ***************************************************************************

dnl process this file with autoconf to produce a configure script

AC_INIT(Makefile.am)
AC_CONFIG_SRCDIR(base/features.h)

dnl TAG: fix export of dependencies to other libraries
dnl TAG: protect against absolute path in srcdir

ac_base_build_date=`date --iso-8601=seconds`
ac_base_build_user=`whoami`
ac_base_build_host=`uname -n`

AC_PREREQ(2.52)
AC_COPYRIGHT([Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>])
dnl AC_REVISION

AC_CANONICAL_SYSTEM()

dnl perform program name transformation
AC_ARG_PROGRAM()

AM_INIT_AUTOMAKE(base-framework, 0.9.1)

dnl arguments
ac_base_arguments=$ac_configure_args

dnl extract general arguments
dnl TAG: may remove too many arguments
ac_base_general_arguments=`echo " ${ac_base_arguments}" | \
  sed -e "s/ --\(enable\|disable\)\(-[[a-z]]\+\)\+\(=[[^ =]]*\)\?//g" \
      -e "s/ [[a-zA-Z]][[a-zA-Z_-]]*='[[^ ']]*'//g" \
      -e "s/ [[a-zA-Z]][[a-zA-Z_-]]*=[[^ ']]*//g" \
      -e "s/ '[[^'=]]\+=[[^']]*'//g"`
BASE_MODULE_ARGUMENTS=$ac_base_general_arguments

dnl AC_CHECK_COMPILERS()
dnl AC_EXEEXT() dnl TAG: required before 2.50
dnl AC_PROG_MAKE_SET()
AC_PROG_CXX()
AC_PROG_CPP()
AC_CHECK_TOOL(WINDRES, windres)
dnl AC_ENABLE_SHARED(yes)
dnl AC_ENABLE_STATIC(yes)
AC_LIBTOOL_WIN32_DLL()
AC_PROG_LIBTOOL()



dnl constants and default values
base_symbol_prefix=_DK_SDU_MIP__BASE__
base_cv_flavor=UNIX
base_cv_os=UNSPECIFIED
base_cv_os_minor=0x000000
base_cv_arch=UNSPECIFIED
base_cv_arch_minor=UNSPECIFIED

case $target_os in
aix) base_cv_os=AIX;;
beos) base_cv_os=BEOS;;
cygwin) base_cv_os=CYGWIN;;
linux*) base_cv_os=GNULINUX;;
mingw32) base_cv_os=MINGW32;;
solaris2) base_cv_os=SOLARIS;;
solaris2.7) base_cv_os=SOLARIS base_cv_os_minor=0x020700;;
solaris2.8) base_cv_os=SOLARIS base_cv_os_minor=0x020800;;
solaris2.9) base_cv_os=SOLARIS base_cv_os_minor=0x020900;;
irix) base_cv_os=IRIX;;
irix62) base_cv_os=IRIX65 base_cv_os_minor=0x060200;;
irix63) base_cv_os=IRIX65 base_cv_os_minor=0x060300;;
irix64) base_cv_os=IRIX65 base_cv_os_minor=0x060400;;
irix65) base_cv_os=IRIX65 base_cv_os_minor=0x060500;;
esac

case $target_cpu in
alpha*) base_cv_arch=ALPHA;;
i386) base_cv_arch=X86 base_cv_arch_minor=386;;
i486) base_cv_arch=X86 base_cv_arch_minor=486;;
i586) base_cv_arch=X86 base_cv_arch_minor=586;;
i686) base_cv_arch=X86 base_cv_arch_minor=686;;
x86_64) base_cv_arch=X86_64;;
mips*) base_cv_arch=MIPS;;
powerpc*) base_cv_arch=PPC;;
sparcv9) base_cv_arch=SPARC base_cv_arch_minor=SPARC_V9;;
sparc*) base_cv_arch=SPARC;;
s390) base_cv_arch=S390;;
esac

dnl specifies the generic order of libraries for the Unix flavor (some libraries may not be present)
dnl TAG: FIXME
ac_base_system_lib_order="-lsocket -lnsl -lrt -lpthread -ldl -lm -lc"

# components
base_cv_component_aio=no
base_cv_component_ieee1394=no
base_cv_component_lfs=no
base_cv_component_pthread=no
base_cv_component_socket=no
base_cv_component_inet_ipv6=no
base_cv_component_sound=no

base_cv_components=


dnl configuration
ac_base_public_config=public.tmp
echo "/* This is an automatically generated file. Do NOT edit. */" > $ac_base_public_config
ac_base_private_config=private.tmp
echo "/* This is an automatically generated file. Do NOT edit. */" > $ac_base_private_config



dnl AC_BASE_DEFINE(symbol name, value)
AC_DEFUN(AC_BASE_DEFINE, [dnl
ifelse([$2], , ac_base_temp="${base_symbol_prefix}$1", ac_base_temp="${base_symbol_prefix}$1 $2")
cat >> $ac_base_public_config << EOF
[#define] $ac_base_temp
EOF
])

dnl AC_BASE_DEFINE_PRIVATE(symbol name, value)
AC_DEFUN(AC_BASE_DEFINE_PRIVATE, [dnl
ifelse([$2], , ac_base_temp="${base_symbol_prefix}$1", ac_base_temp="${base_symbol_prefix}$1 $2")
cat >> $ac_base_private_config << EOF
[#define] $ac_base_temp
EOF
])

dnl AC_BASE_DEFINE_FEATURE(symbol name, value)
AC_DEFUN(AC_BASE_DEFINE_FEATURE, [dnl
ifelse([$2], , ac_base_temp="$1", ac_base_temp="$1 $2")
cat >> $ac_base_private_config << EOF
[#define] $ac_base_temp
EOF
])

dnl AC_BASE_DEFINE_FUNCTION(function name)
AC_DEFUN(AC_BASE_DEFINE_FUNCTION, [dnl
AC_BASE_DEFINE(["HAVE_`echo $1 | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ`"])
])

dnl AC_BASE_DEFINE_FUNCTION_PRIVATE(function name)
AC_DEFUN(AC_BASE_DEFINE_FUNCTION_PRIVATE, [dnl
AC_BASE_DEFINE_PRIVATE(["HAVE_`echo $1 | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ`"])
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
if test x$ac_base_status = x0; then $1=yes; else $1=no; fi dnl
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
ac_base_arg_valid=yes
for ac_base_temp in $2; do
  ac_base_arg_valid=no
  if test "x[$]{ac_base_temp}" = "x[$]{enableval}"; then
    ac_base_arg_valid=yes
    break
  fi
done
if test "x[$]{ac_base_arg_valid}" = "xyes"; then
  $1=[$]enableval
else
  AC_MSG_ERROR($3)
fi
])

dnl AC_BASE_PREPEND(variable, value)
AC_DEFUN(AC_BASE_PREPEND, [dnl
$1="$2[$]{$1:+ }[$]{$1}"
])

dnl AC_BASE_APPEND(variable, value)
AC_DEFUN(AC_BASE_APPEND, [dnl
$1="[$]{$1}[$]{$1:+ }$2"
])

dnl AC_BASE_ADDLIB(library list) e.g. AC_BASE_ADDLIB(-lxml2 -ldl)
dnl AC_DEFUN(AC_BASE_ADDLIB, [dnl
dnl for ac_base_temp in [$1]; do
dnl   AC_BASE_APPEND(BASE_LIBS, [$]ac_base_temp)
dnl done
dnl ])

dnl AC_BASE_ADDLIB(library list) e.g. AC_BASE_ADDLIB(-lxml2 -ldl)
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



ac_base_config=yes
AC_ARG_ENABLE(
  config,[
  --enable-config=[yes|no]
      This option specifies whether ot not the static target configuration
      should be utilized if available. This option is enabled by default.
      The target configuration is probed if either no static configuration
      is available or this option is disabled. Probing is not possible when
      cross compiling the framework.],
  AC_BASE_CHECK_ARG(ac_base_config, yes no, The static option must be either 'yes' or 'no'.))



ac_base_target=$ac_cv_target
case $ac_base_target in
i386-*-cygwin) ac_base_target=i386-unknown-cygwin;;
i486-*-cygwin) ac_base_target=i486-unknown-cygwin;;
i586-*-cygwin) ac_base_target=i586-unknown-cygwin;;
i686-*-cygwin) ac_base_target=i686-unknown-cygwin;;
i386-*-linux*) ac_base_target=i386-unknown-linux-gnu;;
i486-*-linux*) ac_base_target=i486-unknown-linux-gnu;;
i586-*-linux*) ac_base_target=i586-unknown-linux-gnu;;
i686-*-linux*) ac_base_target=i686-unknown-linux-gnu;;
ia64-*-linux*) ac_base_target=ia64-unknown-linux-gnu;;
s390-ibm-linux*) ac_base_target=s390-ibm-linux-gnu;;
mips-sgi-irix) ac_base_target=mips-sgi-irix;;
mips-sgi-irix6.2) ac_base_target=mips-sgi-irix62;;
mips-sgi-irix6.3) ac_base_target=mips-sgi-irix63;;
mips-sgi-irix6.4) ac_base_target=mips-sgi-irix64;;
mips-sgi-irix6.5) ac_base_target=mips-sgi-irix65;;
sparcv9-sun-solaris2) ac_base_target=sparcv9-sun-solaris2;;
sparcv9-sun-solaris2.8) ac_base_target=sparcv9-sun-solaris2.8;;
sparcv9-sun-solaris2.9) ac_base_target=sparcv9-sun-solaris2.9;;
sparc*-sun-solaris2) ac_base_target=sparc-sun-solaris2;;
sparc*-sun-solaris2.7) ac_base_target=sparc-sun-solaris2.7;;
sparc*-sun-solaris2.8) ac_base_target=sparc-sun-solaris2.8;;
sparc*-sun-solaris2.9) ac_base_target=sparc-sun-solaris2.9;;
i386-*-mingw32) ac_base_target=i386-unknown-mingw32;;
i486-*-mingw32) ac_base_target=i486-unknown-mingw32;;
i586-*-mingw32) ac_base_target=i586-unknown-mingw32;;
i686-*-mingw32) ac_base_target=i686-unknown-mingw32;;
esac



ac_base_static_loaded=no
if test x$ac_base_config = xyes && test -f $srcdir/static/$ac_base_target; then
  echo loading static target configuration...
  rm -f static.tmp
  if (cd $srcdir/static; ./static.sh ./$ac_base_target) > static.tmp 2>&1; then
    :
  else
    AC_MSG_ERROR(Unable to import static target configuration)
  fi
  . ./static.tmp
  ac_base_static_loaded=yes
fi

# make sure static configuration has been loaded if cross compiling
if test x$base_cv_cxx_cross = xyes; then
  if test x$ac_base_static_loaded != xyes; then
    if test x$ac_base_config = xyes; then
      AC_MSG_ERROR(This target currently does not support cross compilation. You could try with a generic target configuration.)
    else
      AC_MSG_ERROR(Static target configuration must be enabled when cross compiling.)
    fi
  fi
fi



AC_BASE_DEFINE(FLAVOR, $base_symbol_prefix$base_cv_flavor)
AC_BASE_DEFINE(OS, $base_symbol_prefix$base_cv_os)
AC_BASE_DEFINE(OS_MINOR, $base_cv_os_minor)
AC_BASE_DEFINE(ARCH, $base_symbol_prefix$base_cv_arch)
AC_BASE_DEFINE(ARCH_MINOR, $base_symbol_prefix$base_cv_arch_minor)



AC_MSG_CHECKING(flavor of target)
AC_MSG_RESULT($base_cv_flavor)



dnl Manual configuration

dnl TAG: need version specification otherwise this option is useless

dnl base_explicit_compiler=auto
dnl AC_ARG_ENABLE(
dnl   compiler,[
dnl   --enable-compiler=[auto|borland|gcc|intel|mipspro|msc|sunwspro]
dnl       Specifies that the default compiler is compatible with the
dnl       specified compiler.],
dnl   AC_BASE_CHECK_ARG(base_explicit_compiler, auto borland gcc intel mipspro msc sunwspro, Invalid argument used with the compiler flag.))

base_explicit_debug=no
AC_ARG_ENABLE(
  debug,[
  --enable-debug=[yes|no]
      This feature enables additional runtime checks. The feature is
      disabled by default.],
  AC_BASE_CHECK_ARG(base_explicit_debug, yes no, The debug flag must be either 'yes' or 'no'.))
AC_BASE_YES(base_explicit_debug, AC_BASE_DEFINE_PRIVATE(DEBUG))

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

base_explicit_experimental=no
AC_ARG_ENABLE(
  experimental,[
  --enable-experimental=[yes|no]
      Specifies whether or not to include experimental components. Experimental
      code is excluded by default. Experimental code should only be enabled by
      the framework developers.],
  AC_BASE_CHECK_ARG(base_explicit_experimental, yes no, Invalid argument used with the experimental option.))
AM_CONDITIONAL(EXPERIMENTAL, test x$base_explicit_experimental = xyes)
AC_BASE_DEFINE(EXPERIMENTAL)

base_explicit_extensions=no
AC_ARG_ENABLE(
  extensions,[
  --enable-extensions=[yes|no]
      Specifies whether of not the extensions should be included. The
      extensions are not included by default.],
  AC_BASE_CHECK_ARG(base_explicit_extensions, yes no, Invalid argument used with the extensions option.))
AM_CONDITIONAL(EXTENSIONS, test x$base_explicit_extensions = xyes)
AC_BASE_DEFINE(EXTENSIONS)

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
  AC_BASE_CHECK_ARG(base_explicit_threads, auto posix win32, The threads flag must be either 'auto', 'posix', or 'win32'.))

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

base_explicit_aio=auto
AC_ARG_ENABLE(
  aio,[
  --enable-aio=[auto|yes|no]
      Specifies that the asynchronous io (aio) interface should be utilized.
      This feature is auto-detected by default. The auto-detection will fail
      if the compiler is a cross compiler.],
  AC_BASE_CHECK_ARG(base_explicit_aio, auto yes no, The asynchronous io (aio) flag must be either 'auto', 'yes', or 'no'.))

base_explicit_regexp=auto
AC_ARG_ENABLE(
  regexp,[
  --enable-regexp=[auto|pcre|posix|no]
      Specifies that regular expression support should be included. 'pcre'
      selects Perl-Compatible Regular Expression library. 'posix' selects POSIX
      compatible regular expression support provided by the C library. The
      default is no.],
  AC_BASE_CHECK_ARG(base_explicit_regexp, auto pcre posix no, Invalid argument used with the regexp flag.))

base_explicit_bz2=yes
AC_ARG_ENABLE(
  bz2,[
  --enable-bz2=[yes|no]
      Specifies that bz2 support should be included. The default is yes.],
  AC_BASE_CHECK_ARG(base_explicit_bz2, yes no, Invalid argument used with the bz2 flag.))

base_explicit_zlib=yes
AC_ARG_ENABLE(
  zlib,[
  --enable-zlib=[yes|no]
      Specifies that zlib support should be included. The default is yes.],
  AC_BASE_CHECK_ARG(base_explicit_zlib, yes no, Invalid argument used with the zlib flag.))

base_explicit_xml=no
AC_ARG_ENABLE(
  xml,[
  --enable-xml=[xmlsoft|no]
      Specifies that XML support should be included. 'xmlsoft' selects the
      Gnome XML Library. The default is no.],
  AC_BASE_CHECK_ARG(base_explicit_xml, xmlsoft no, Invalid argument used with the xml flag.))

base_explicit_xslt=no
AC_ARG_ENABLE(
  xslt,[
  --enable-xslt=[xmlsoft|no]
      Specifies that XSLT support should be included. 'xmlsoft' selects the
      Gnome XSLT Library. The default is no.],
  AC_BASE_CHECK_ARG(base_explicit_xslt, xmlsoft no, Invalid argument used with the xslt flag.))

base_explicit_ieee1394=yes
AC_ARG_ENABLE(
  ieee1394,[
  --enable-ieee1394=[*|yes|no]
      Specifies the name of the default IEEE 1394 implementation. The built-in
      implementation is utilized if this option is not specified (or enabled
      without a name). Disabling the IEEE 1394 implementation forces the dummy
      implementation to be utilized allowing IEEE 1394 applications to link with
      the library.],
  base_explicit_ieee1394=[$]enableval)

case $base_explicit_ieee1394 in
yes)
  base_cv_component_ieee1394=yes
;;
no)
  base_cv_component_ieee1394=no
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL, DummyIEEE1394Impl)
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL_HEADER, [\"]base/communication/DummyIEEE1394Impl.h[\"])
;;
*.h)
  base_cv_component_ieee1394=yes
  ac_base_temp=`echo ${base_explicit_ieee1394%.h} | sed "s/^.*\///"`
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL, ${ac_base_temp})
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL_HEADER, [\"]${base_explicit_ieee1394}[\"])
;;
[[A-Z]]*)
  base_cv_component_ieee1394=yes
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL, $base_explicit_ieee1394)
  AC_BASE_DEFINE_PRIVATE(IEEE_1394_IMPL_HEADER, [\"]base/communication/${base_explicit_ieee1394}.h[\"])
;;
*)
  AC_MSG_ERROR(Invalid value for IEEE 1394 implementation (@<:@A-Z@:>@@<:@A-Za-z0-9_@:>@* or relative path of header).)
;;
esac

if test x${base_cv_component_ieee1394} = xyes; then
  base_cv_components="${base_cv_components}${base_cv_components:+ }ieee1394"
fi

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

dnl AC_BASE_CHECK(check message, action, result variable [, extra variable])
AC_DEFUN(AC_BASE_CHECK, [dnl
ac_base_temp="$1"
AC_MSG_CHECKING($ac_base_temp)
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
AC_BASE_YESNO(ac_temp_cached, [echo ${ECHO_N} "(static) ${ECHO_C}" >&AC_FD_MSG], $2)
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
base_obj_temp="test_object$ac_cv_objext"
base_app_temp="test_app$ac_cv_exeext"
base_cleanup="rm -f $base_src_temp $base_obj_temp $base_app_temp"
base_run="./$base_app_temp"

dnl C++ specific
base_cxx_ext=cpp
base_cxx_preprocess='$CXXCPP -I. -I$srcdir $CPPFLAGS'
base_cxx_compile='$CXX -o $base_obj_temp -c -I. -I$srcdir $CPPFLAGS $CXXFLAGS $srcdir/config/$base_cxx_src $base_cxx_opts'
base_cxx_link='$CXX -o $base_app_temp -I. -I$srcdir $CPPFLAGS $CXXFLAGS $srcdir/config/$base_cxx_src $base_cxx_opts $LDFLAGS $LIBS $BASE_LIBS'

dnl AC_BASE_TRIM(string)
AC_DEFUN(AC_BASE_TRIM, [`echo $1`])

dnl AC_BASE_TRY_EXEC(variable)
AC_DEFUN(AC_BASE_TRY_EXEC, [ { eval [$]$1 >&AC_FD_CC 2>&AC_FD_CC; ac_base_status=`echo $?`; } ])

dnl AC_BASE_TRY_CMD(command)
AC_DEFUN(AC_BASE_TRY_CMD, [ { ac_base_temp='$1'; AC_BASE_TRY_EXEC(ac_base_temp); } ])

dnl AC_BASE_TRY_CXX_COMPILE(result variable, source file, additional options)
AC_DEFUN(AC_BASE_TRY_CXX_COMPILE, [dnl
if (test ! -r $srcdir/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_compile)
AC_BASE_MKYESNO([$1])
])

dnl AC_BASE_TRY_CXX_LINK(result variable, source file, additional options)
AC_DEFUN(AC_BASE_TRY_CXX_LINK, [dnl
if (test ! -r $srcdir/config/[$2]); then
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
if (test ! -r $srcdir/config/[$2]); then
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
if (test ! -r $srcdir/config/[$2]); then
  AC_MSG_ERROR(The source file cannot be read.)
fi
base_cxx_src=[$2]
base_cxx_opts=[$3]
AC_BASE_TRY_EXEC(base_cxx_link)
AC_BASE_MKYESNO(ac_base_temp)
AC_BASE_YESNO(
  ac_base_temp,
  AC_BASE_TRY_EXEC(base_run)
  [$1]=$ac_base_status,
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

dnl AC_BASE_CHECK_CFUNCTION_PRIVATE(name)
AC_DEFUN(AC_BASE_CHECK_CFUNCTION_PRIVATE, [dnl
AC_BASE_CHECK(
  whether API function $1 is available,
  AC_BASE_CACHE(
    base_cv_function_$1,
    AC_BASE_TRY_CXX_LINK(base_cv_function_$1, $1.c)
  ),
  base_cv_function_$1)
AC_BASE_YES(base_cv_function_$1, AC_BASE_DEFINE_FUNCTION_PRIVATE($1))
])



AC_BASE_CHECK(
  whether C++ compiler (AC_BASE_TRIM($CXX $CPPFLAGS $CXXFLAGS)) works,
  AC_BASE_TRY_CXX_LINK(base_cxx_compiler_works, cross++.cpp),
  base_cxx_compiler_works)

AC_BASE_CHECK(
  whether C++ compiler (AC_BASE_TRIM($CXX $CPPFLAGS $CXXFLAGS $LDFLAGS $LIBS $BASE_LIBS)) is a cross compiler,
  AC_BASE_CACHE(
    base_cv_cxx_cross base_cv_cxx_libs,
    AC_BASE_ATTEMPT(
      base_cxx_not_cross,
      base_cv_cxx_libs,
      "" -lpthread,
      AC_BASE_TRY_CXX_RUN(base_cxx_not_cross, cross++.cpp, ${base_cv_cxx_libs})dnl
    )dnl
    AC_BASE_NOT(base_cv_cxx_cross, base_cxx_not_cross)dnl
  ),
  base_cv_cxx_cross,
  base_cv_cxx_libs)
AC_BASE_ADDLIB(${base_cv_cxx_libs})

AC_BASE_NO(
  base_cv_cxx_cross,
  AC_BASE_CHECK(
    whether Standard Template Library is working,
    AC_BASE_CACHE(
      base_cv_stl_available base_cv_stl_libs,
      AC_BASE_ATTEMPT(
        base_cv_stl_available,
        base_cv_stl_libs,
        "" -lpthread,
        AC_BASE_TRY_CXX_RUN(base_cv_stl_available, stdc++.cpp, ${base_cv_stl_libs})dnl
      )
    ),
    base_cv_stl_available,
    base_cv_stl_libs)
  AC_BASE_ADDLIB(${base_cv_stl_libs})
)



dnl Lets play "name that c++ compiler"

if test -z "$ac_base_cxx_name"; then
  # Check if GNU Compiler Collection (C++ compiler)
  AC_BASE_TRY_CXX_COMPILE(base_cv_gcc_compiler, gcc.cpp)
  if test "x$base_cv_gcc_compiler" = xyes; then
    ac_base_cxx_version=`$CXX -dumpversion 2>&1`
    ac_base_cxx_major=`echo $ac_base_cxx_version | sed "s/\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`echo $ac_base_cxx_version | sed "s/[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="GNU Compiler Collection"
    ac_base_cxx_vendor="gnu.org"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  # Check if Sun WorkShop Compiler (C++ compiler)
  AC_BASE_TRY_CXX_COMPILE(base_cv_sunwspro_compiler, sunwspro.cpp)
  if test "x$base_cv_sunwspro_compiler" = xyes; then
    ac_base_cxx_major=`$CXX -V 2>&1 | sed "s/.*C++.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -V 2>&1 | sed "s/.*C++.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="Sun WorkShop"
    ac_base_cxx_vendor="sun.com"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  # Check if MIPSpro Compiler
  AC_BASE_TRY_CXX_COMPILE(base_cv_mipspro_compiler, mipspro.cpp)
  if test "x$base_cv_mipspro_compiler" = xyes; then
    ac_base_cxx_major=`$CXX -version 2>&1 | sed "s/.*Version.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -version 2>&1 | sed "s/.*Version.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="MIPSpro Compilers"
    ac_base_cxx_vendor="sgi.com"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  # Check if Intel Compiler
  AC_BASE_TRY_CXX_COMPILE(base_cv_intel_compiler, intel.cpp)
  if test "x$base_cv_intel_compiler" = xyes; then
    ac_base_cxx_major=`$CXX -V 2>&1 | sed "s/.*Version.*\([[0-9]]\)\+\.[[0-9]]\+.*/\1/"`
    ac_base_cxx_minor=`$CXX -V 2>&1 | sed "s/.*Version.*[[0-9]]\+\.\([[0-9]]\+\).*/\1/"`
    ac_base_cxx_name="Intel Compiler"
    ac_base_cxx_vendor="intel.com"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  # Check if Borland Compiler
  AC_BASE_TRY_CXX_COMPILE(base_cv_borland_compiler, borland.cpp)
  if test "x$base_cv_borland_compiler" = xyes; then
    ac_base_cxx_name="Borland Compiler"
    ac_base_cxx_vendor="borland.com"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  # Check if Microsoft Compiler
  AC_BASE_TRY_CXX_COMPILE(base_cv_msc_compiler, msc.cpp)
  if test "x$base_cv_msc_compiler" = xyes; then
    ac_base_cxx_name="Microsoft Optimizing Compiler"
    ac_base_cxx_vendor="microsoft.com"
  fi
fi

if test -z "$ac_base_cxx_name"; then
  ac_base_cxx_name=generic
  ac_base_cxx_vendor=unknown
fi

# make generic version
if test -z "$ac_base_cxx_version"; then
  if test -z "$ac_base_cxx_micro"; then
    ac_base_cxx_version="$ac_base_cxx_major.$ac_base_cxx_minor"
  else
    ac_base_cxx_version="$ac_base_cxx_major.$ac_base_cxx_minor.$ac_base_cxx_micro"
  fi
fi

AC_MSG_CHECKING(identity of C++ compiler)
AC_MSG_RESULT([$ac_base_cxx_name $ac_base_cxx_version ($ac_base_cxx_vendor)])



dnl Explicit features.

AC_MSG_CHECKING(ABI)
base_abi=
case "$ac_base_cxx_vendor/$ac_base_cxx_name" in
"gnu.org/GNU Compiler Collection")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_GCC)
  if test x$ac_base_cxx_major = x3; then
    AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_V3MV)
    base_abi=v3mv
  elif test x$ac_base_cxx_major = x2; then
    AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_GCCV2)
    base_abi=gccv2
  fi
;;
"sun.com/Sun WorkShop")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_SUNWSPRO)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_SUNWSPRO)
  base_abi=sunwspro
;;
"sgi.com/MIPSpro Compilers")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_MIPSPRO)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_MIPSPRO)
  base_abi=mipspro
;;
"intel.com/Intel Compiler")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_INTEL)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_INTEL)
  base_abi=intel
;;
"borland.com/Borland Compiler")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_BORLAND)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_BORLAND)
  base_abi=borland
;;
"microsoft.com/Microsoft Optimizing Compiler")
  AC_BASE_DEFINE(DEFAULT_COMPILER, ${base_symbol_prefix}COMPILER_MSC)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}ABI_MSC)
  base_abi=intel
;;
*)
  AC_BASE_DEFINE_PRIVATE(ABI, ${base_symbol_prefix}UNSPECIFIED)
;;
esac
AC_MSG_RESULT([$base_abi])



AC_MSG_CHECKING(selected demangling scheme)
if test x$base_explicit_demangle = xauto; then
  base_demangle=$base_abi
else
  base_demangle=$base_explicit_demangle
fi

case $base_demangle in
v3mv)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_V3MV)
;;
gccv3)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_GCCV3)
  AC_BASE_ADDLIB(-liberty)
;;
gccv2)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_GCCV2)
  AC_BASE_ADDLIB(-liberty)
;;
mipspro)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_MIPSPRO)
  AC_BASE_ADDLIB(-lmangle)
;;
sunwspro)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_SUNWSPRO)
  AC_BASE_ADDLIB(-ldemangle)
;;
intel)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_INTEL)
;;
borland)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_BORLAND)
;;
msc)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}DEMANGLE_MSC)
;;
*)
  AC_BASE_DEFINE_PRIVATE(DEMANGLE, ${base_symbol_prefix}UNSPECIFIED)
;;
esac
AC_MSG_RESULT([$base_demangle])



if test x$base_explicit_threads = xauto; then
  AC_BASE_EQUAL($base_cv_flavor, UNIX, base_explicit_threads=posix)
fi

if test x$base_explicit_xml = xxmlsoft; then
  AC_BASE_DEFINE_PRIVATE(XML_XMLSOFT_ORG)
  AC_BASE_ADDLIB(-lxml2)
fi

if test x$base_explicit_xslt = xxmlsoft; then
  AC_BASE_DEFINE_PRIVATE(XSLT_XMLSOFT_ORG)
  AC_BASE_ADDLIB(-lxslt)
fi



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
AC_BASE_DEFINE(CHAR_SIZE, $base_cv_char_size)

AC_BASE_CHECK(
  size of short,
  AC_BASE_CACHE(
    base_cv_short_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_short_size, short_size.cpp)dnl
  ),
  base_cv_short_size)
AC_BASE_DEFINE(SHORT_SIZE, $base_cv_short_size)

AC_BASE_CHECK(
  size of int,
  AC_BASE_CACHE(
    base_cv_int_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_int_size, int_size.cpp)dnl
  ),
  base_cv_int_size)
AC_BASE_DEFINE(INT_SIZE, $base_cv_int_size)

AC_BASE_CHECK(
  size of long,
  AC_BASE_CACHE(
    base_cv_long_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_long_size, long_size.cpp)dnl
  ),
  base_cv_long_size)
AC_BASE_DEFINE(LONG_SIZE, $base_cv_long_size)

AC_BASE_CHECK(
  size of long long,
  AC_BASE_CACHE(
    base_cv_long_long_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_long_long_size, long_long_size.cpp)dnl
  ),
  base_cv_long_long_size)
AC_BASE_DEFINE(LONG_LONG_SIZE, $base_cv_long_long_size)

AC_BASE_CHECK(
  size of pointer,
  AC_BASE_CACHE(
    base_cv_pointer_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_pointer_size, pointer_size.cpp)dnl
  ),
  base_cv_pointer_size)
AC_BASE_DEFINE(POINTER_SIZE, $base_cv_pointer_size)

AC_BASE_CHECK(
  size of wchar,
  AC_BASE_CACHE(
    base_cv_wchar_size,
    AC_BASE_TRY_CXX_RUNSTATUS(base_cv_wchar_size, wchar_size.cpp)dnl
   ),
   base_cv_wchar_size)
AC_BASE_DEFINE(WCHAR_SIZE, $base_cv_wchar_size)




dnl AC_BASE_VALUE2BYTEORDER(result variable, value)
AC_DEFUN(AC_BASE_VALUE2BYTEORDER, [dnl
  if test "x[$2]" = x1; then
    [$1]="BIG_ENDIAN"
  elif test "x[$2]" = x2; then
    [$1]="LITTLE_ENDIAN"
  else
    [$1]="UNSPECIFIED"
  fi
])

dnl AC_BASE_BYTEORDER2MSG(result variable, value)
AC_DEFUN(AC_BASE_BYTEORDER2MSG, [dnl
  if test "x[$]$2" = xBIG_ENDIAN; then
    [$1]="big endian"
  elif test "x[$]$2" = xLITTLE_ENDIAN; then
    [$1]="little endian"
  else
    [$1]="unspecified"
  fi
])

if test -n "$base_cv_byte_order"; then
  AC_BASE_BYTEORDER2MSG(base_cv_byte_order_msg, base_cv_byte_order)
fi
AC_BASE_COMPARE(
  ${base_explicit_byte_order}, auto,
  AC_BASE_CHECK(
    byte order of architecture,
    AC_BASE_CACHE(
      base_cv_byte_order,
      AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, byteorder.c, , Auto-detection of byte order does not work with cross compiler. Please specify byte order explicitly using --enable-byte-order.)dnl
      AC_BASE_VALUE2BYTEORDER(base_cv_byte_order, ${ac_base_temp})dnl
      AC_BASE_BYTEORDER2MSG(base_cv_byte_order_msg, base_cv_byte_order)dnl
    ),
    base_cv_byte_order_msg),
  base_cv_byte_order=${base_explicit_byte_order})
AC_BASE_DEFINE(BYTE_ORDER, ${base_symbol_prefix}${base_cv_byte_order})

if test -n "$base_cv_word_order"; then
  AC_BASE_BYTEORDER2MSG(base_cv_word_order_msg, base_cv_word_order)
fi
AC_BASE_COMPARE(
  ${base_explicit_word_order}, auto,
  AC_BASE_CHECK(
    word order of architecture,
    AC_BASE_CACHE(
      base_cv_word_order,
      AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, float_word_order.cpp, , Auto-dection of word order does not work with cross compiler.)dnl
      AC_BASE_VALUE2BYTEORDER(base_cv_word_order, ${ac_base_temp})dnl
      AC_BASE_BYTEORDER2MSG(base_cv_word_order_msg, base_cv_word_order)dnl
    ),
    base_cv_word_order_msg),
  base_cv_word_order=${base_explicit_word_order})
AC_BASE_DEFINE(FLOAT_WORD_ORDER, ${base_symbol_prefix}${base_cv_word_order})



AC_DEFUN(AC_BASE_VALUE2FLOATING, [dnl
  if test "x[$2]" = x1; then
    [$1]="IEEE_754_SINGLE_PRECISION"
  elif test "x[$2]" = x2; then
    [$1]="IEEE_754_DOUBLE_PRECISION"
  elif test "x[$2]" = x3; then
    [$1]="IEEE_EXTENDED_DOUBLE_PRECISION_96"
  elif test "x[$2]" = x4; then
    [$1]="IEEE_EXTENDED_DOUBLE_PRECISION_128"
  elif test "x[$2]" = x5; then
    [$1]="IEEE_QUADRUPLE_PRECISION"
  else
    [$1]="UNSPECIFIED"
  fi
])

AC_DEFUN(AC_BASE_FLOATING2MSG, [dnl
  if test "x[$]$2" = xIEEE_754_SINGLE_PRECISION; then
    [$1]="ieee 754 single"
  elif test "x[$]$2" = xIEEE_754_DOUBLE_PRECISION; then
    [$1]="ieee 754 double"
  elif test "x[$]$2" = xIEEE_EXTENDED_DOUBLE_PRECISION_96; then
    [$1]="ieee extended 96"
  elif test "x[$]$2" = xIEEE_EXTENDED_DOUBLE_PRECISION_128; then
    [$1]="ieee extended 128"
  elif test "x[$]$2" = xIEEE_QUADRUPLE_PRECISION; then
    [$1]="ieee quadruple"
  else
    [$1]="unspecified"
  fi
])

AC_BASE_FLOATING2MSG(base_cv_float_msg, base_cv_float)
AC_BASE_CHECK(
  representation of float,
  AC_BASE_CACHE(
    base_cv_float,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, float.cpp, )dnl
    AC_BASE_VALUE2FLOATING(base_cv_float, ${ac_base_temp})dnl
    AC_BASE_FLOATING2MSG(base_cv_float_msg, base_cv_float)dnl
  ),
  base_cv_float_msg)
AC_BASE_DEFINE(FLOAT, ${base_symbol_prefix}${base_cv_float})

AC_BASE_FLOATING2MSG(base_cv_double_msg, base_cv_double)
AC_BASE_CHECK(
  representation of double,
  AC_BASE_CACHE(
    base_cv_double,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, double.cpp, )dnl
    AC_BASE_VALUE2FLOATING(base_cv_double, ${ac_base_temp})dnl
    AC_BASE_FLOATING2MSG(base_cv_double_msg, base_cv_double)dnl
  ),
  base_cv_double_msg)
AC_BASE_DEFINE(DOUBLE, ${base_symbol_prefix}${base_cv_double})

AC_BASE_FLOATING2MSG(base_cv_long_double_msg, base_cv_long_double)
AC_BASE_CHECK(
  representation of long double,
  AC_BASE_CACHE(
    base_cv_long_double,
    AC_BASE_TRY_CXX_RUNSTATUS(ac_base_temp, long_double.cpp, )dnl
    AC_BASE_VALUE2FLOATING(base_cv_long_double, ${ac_base_temp})dnl
    AC_BASE_FLOATING2MSG(base_cv_long_double_msg, base_cv_long_double)dnl
  ),
  base_cv_long_double_msg)
AC_BASE_DEFINE(LONG_DOUBLE, ${base_symbol_prefix}${base_cv_long_double})



dnl Checks for typedefs, structures, and compiler characteristics.

if test x$base_cv_flavor = xUNIX; then
AC_BASE_CHECK(
  whether the type socklen_t is available,
  AC_BASE_CACHE(
    base_cv_type_socklen_t,
    AC_BASE_TRY_CXX_LINK(base_cv_type_socklen_t, socklen_t.c)dnl
  ),
  base_cv_type_socklen_t)
AC_BASE_YES(base_cv_type_socklen_t, AC_BASE_DEFINE_PRIVATE(SOCKLEN_T))
fi dnl end of unix



dnl checks for library functions.

dnl network support
base_cv_component_socket=yes
base_cv_components="${base_cv_components}${base_cv_components:+ }socket"

case $base_cv_flavor in
UNIX)
  if test x$base_explicit_inet_ipv6 = xauto; then
    AC_BASE_CHECK(
      whether IPv6 is supported,
      AC_BASE_CACHE(
        base_cv_api_inet_ipv6,
        AC_BASE_TRY_CXX_LINK(base_cv_api_inet_ipv6, ipv6.c)dnl
      ),
      base_cv_api_inet_ipv6)
  fi
;;
*)
  base_cv_api_inet_ipv6=$base_explicit_inet_ipv6
;;
esac
AC_BASE_YES(base_cv_api_inet_ipv6, AC_BASE_DEFINE_PRIVATE(INET_IPV6))



# sound support
base_cv_component_sound=yes
base_cv_components="${base_cv_components}${base_cv_components:+ }sound"



if test x$base_cv_flavor = xUNIX; then

if test x$base_explicit_large_file_system = xauto; then
  AC_BASE_CHECK(
    whether Large File System is supported by the operating system,
    AC_BASE_CACHE(
      base_cv_api_lfs,
      AC_BASE_TRY_CXX_LINK(base_cv_api_lfs, largefile.c, , Auto-detection of Large File System does not work with cross compiler.)
    ),
    base_cv_api_lfs)
else
  base_cv_api_lfs=base_explicit_large_file_system
fi
AC_BASE_YES(base_cv_api_lfs, AC_BASE_DEFINE_PRIVATE(LARGE_FILE_SYSTEM))
base_cv_component_lfs=$base_cv_api_lfs
if test x${base_cv_component_lfs} = xyes; then
  base_cv_components="${base_cv_components}${base_cv_components:+ }lfs"
fi



if test x$base_explicit_aio = xauto; then
  AC_BASE_CHECK(
    whether asynchronous io is supported by the operating system,
    AC_BASE_CACHE(
      base_cv_api_aio,
      AC_BASE_TRY_CXX_COMPILE(base_cv_api_aio, aio.c, , Auto-detection of asynchronous io does not work with cross compiler.)
    ),
    base_cv_api_aio)
else
  base_cv_api_aio=$base_explicit_aio
fi
AC_BASE_YES(base_cv_api_aio, AC_BASE_DEFINE_PRIVATE(AIO))
base_cv_component_aio=$base_cv_api_aio
if test x${base_cv_component_aio} = xyes; then
  base_cv_components="${base_cv_components}${base_cv_components:+ }aio"
fi

fi dnl end of unix



AC_BASE_EQUAL(${base_explicit_threads}, posix,
dnl check POSIX thread support

AC_BASE_CHECK(
  whether POSIX thread is supported,
  AC_BASE_CACHE(
    base_cv_api_pthread,
    AC_BASE_TRY_CXX_LINK(base_cv_api_pthread, pthread.c)dnl
  ),
  base_cv_api_pthread)
AC_BASE_YES(base_cv_api_pthread, AC_BASE_DEFINE_PRIVATE(PTHREAD))
base_cv_component_pthread=$base_cv_api_pthread
if test x${base_cv_component_pthread} = xyes; then
  base_cv_components="${base_cv_components}${base_cv_components:+ }pthread"
fi

AC_BASE_CHECK_CFUNCTION_PRIVATE(pthread_yield)

AC_BASE_CHECK(
  whether POSIX semaphore is supported,
  AC_BASE_CACHE(
    base_cv_api_pthread_semaphore,
    AC_BASE_TRY_CXX_LINK(base_cv_api_pthread_semaphore, pthread_semaphore.c)dnl
  ),
  base_cv_api_pthread_semaphore)
AC_BASE_YES(base_cv_api_pthread_semaphore, AC_BASE_DEFINE_PRIVATE(PTHREAD_SEMAPHORE))

AC_BASE_CHECK(
  whether POSIX optimal read-write lock is supported,
  AC_BASE_CACHE(
    base_cv_api_pthread_rwlock,
    AC_BASE_TRY_CXX_LINK(base_cv_api_pthread_rwlock, pthread_rwlock.c)dnl
  ),
  base_cv_api_pthread_semaphore)
AC_BASE_YES(base_cv_api_pthread_rwlock, AC_BASE_DEFINE_PRIVATE(PTHREAD_RWLOCK))

) dnl end of posix thread support



AC_BASE_EQUAL(${base_explicit_regexp}, auto,
  AC_BASE_CHECK(
    whether POSIX Regular Expression interface is available,
    AC_BASE_CACHE(
      base_cv_api_regex,
      AC_BASE_TRY_CXX_RUN(base_cv_api_regex, regex.c)dnl
    ),
    base_cv_api_regex)
  AC_BASE_YES(base_cv_api_regex, AC_BASE_DEFINE_PRIVATE(REGEXP_POSIX)))

AC_BASE_EQUAL(${base_explicit_regexp}, posix, AC_BASE_DEFINE_PRIVATE(REGEXP_POSIX))

AC_BASE_EQUAL(${base_explicit_regexp}, pcre,
  AC_BASE_DEFINE_PRIVATE(REGEXP_PCRE)
  AC_BASE_ADDLIB(-lpcre))



AC_BASE_EQUAL(${base_explicit_bz2}, yes,
  AC_BASE_DEFINE_PRIVATE(BZ2)
  AC_BASE_ADDLIB(-lbz2))



AC_BASE_EQUAL(${base_explicit_zlib}, yes,
  AC_BASE_DEFINE_PRIVATE(ZLIB)
  AC_BASE_ADDLIB(-lz))



AC_BASE_EQUAL(${base_explicit_wide}, auto,
  AC_BASE_CHECK(
    whether wide-character interface is available,
    AC_BASE_CACHE(
      base_cv_api_wide,
      AC_BASE_TRY_CXX_LINK(base_cv_api_wide, wide.c)dnl
    ),
    base_cv_api_wide)
  AC_BASE_YES(base_cv_api_wide, AC_BASE_DEFINE_PRIVATE(WIDE)))
AC_BASE_YES(base_explicit_wide, AC_BASE_DEFINE_PRIVATE(WIDE))



dnl detect whether some functions are available
AC_BASE_CHECK_CFUNCTION(memchr)
AC_BASE_CHECK_CFUNCTION(memcmp)
AC_BASE_CHECK_CFUNCTION(memcpy)
AC_BASE_CHECK_CFUNCTION(memmove)
AC_BASE_CHECK_CFUNCTION(memset)

if test x$base_cv_flavor = xUNIX; then
  AC_BASE_CHECK_CFUNCTION_PRIVATE(pselect)
  AC_BASE_CHECK_CFUNCTION_PRIVATE(readdir_r)
  AC_BASE_CHECK_CFUNCTION_PRIVATE(nanosleep)
  AC_BASE_CHECK_CFUNCTION_PRIVATE(mbrtowc)
  AC_BASE_CHECK_CFUNCTION_PRIVATE(mbsrtowcs)
  AC_BASE_CHECK_CFUNCTION_PRIVATE(sigaction)
fi



dnl add VERSION resource to the dynamic library on Windows platforms (i.e. cygwin or mingw targets)
dnl the -Wl option is used to circumvent libtool
case $base_cv_target in
*-*-cygwin|*-*-mingw32)
  BASE_LIBADD="${BASE_LIBADD}${BASE_LIBADD:+ }"'-Wl,win32rc.o'
  BASE_DEPENDENCIES="${BASE_DEPENDENCIES}${BASE_DEPENDENCIES:+ }"'win32rc.o'
;;
esac



dnl define some describing symbols
AC_BASE_DEFINE_PRIVATE(CONFIGURE, [\"]${ac_configure_args}[\"])
AC_BASE_DEFINE_PRIVATE(BUILD_DATE, [\"]${ac_base_build_date}[\"])
AC_BASE_DEFINE_PRIVATE(BUILD_USER, [\"]${ac_base_build_user}[\"])
AC_BASE_DEFINE_PRIVATE(BUILD_HOST, [\"]${ac_base_build_host}[\"])
AC_BASE_DEFINE_PRIVATE(BUILD_SYSTEM, [\"]${ac_cv_build}[\"])
AC_BASE_DEFINE_PRIVATE(TARGET_SYSTEM, [\"]${ac_cv_target}[\"])

dnl tell the compiler where it should look for the generated and static framework headers
if (test -f base/features.h) then
  dnl we are building directly in the source tree
  BASE_INCLUDES='-I${top_srcdir}'"${BASE_INCLUDES:+ }${BASE_INCLUDES}"
else
  dnl we are building in a separate directory tree
  dnl tell the compiler where it should look for the generated and static framework headers
  BASE_INCLUDES='-I${top_builddir} -I${top_srcdir}'"${BASE_INCLUDES:+ }${BASE_INCLUDES}"
fi



# add required libraries for enabled components
for ac_base_entry in ${base_cv_libraries-""}; do
  ac_base_library=
  case $ac_base_entry in
  *:*)
    ac_base_component=`echo ${ac_base_entry} | sed "s/:.*//"`
    for ac_base_temp in ${base_cv_components-""}; do
      if test x$ac_base_temp = x$ac_base_component; then
        ac_base_library=`echo ${ac_base_entry} | sed "s/.*://"`
        break;
      fi
    done
  ;;
  *)
    ac_base_library=$ac_base_entry
  ;;
  esac
  if test -n "$ac_base_library"; then
    AC_BASE_ADDLIB("-l$ac_base_library")
  fi
done

# TAG: make sure the same lib is given only once (use the last one)

# add libc feature enabling defines
for ac_base_entry in ${base_cv_features-""}; do
  ac_base_feature=
  case $ac_base_entry in
  *:*)
    ac_base_component=`echo ${ac_base_entry} | sed "s/:.*//"`
    for ac_base_temp in ${base_cv_components-""}; do
      if test x$ac_base_temp = x$ac_base_component; then
        ac_base_feature=`echo ${ac_base_entry} | sed "s/.*://"`
        break;
      fi
    done
  ;;
  *)
    ac_base_feature=$ac_base_entry
  ;;
  esac
  if test -n "$ac_base_feature"; then
    case $ac_base_feature in
    *=*)
      ac_base_symbol=`echo ${ac_base_feature} | sed "s/=.*//"`
      ac_base_value=`echo ${ac_base_feature} | sed "s/.*=//"`
      AC_BASE_DEFINE_FEATURE(${ac_base_symbol}, ${ac_base_value})
    ;;
    *)
      AC_BASE_DEFINE_FEATURE(${ac_base_feature})
    ;;
    esac
  fi
done



dnl remove temporary files
$base_cleanup



dnl configure test suite
ac_base_testsuite=no
if test x$base_explicit_testsuite = xauto; then
  if (test -d $srcdir/testsuite); then
    ac_base_testsuite=yes
  else
    ac_base_testsuite=no
  fi
elif test x$base_explicit_testsuite = xyes; then
  if (test -d $srcdir/testsuite); then
    ac_base_testsuite=yes
  else
    AC_MSG_ERROR(The test suite is not available.)
  fi
fi

if test x$ac_base_testsuite = xyes; then
  BASE_MODULES="${BASE_MODULES}${BASE_MODULES:+ }testsuite"
fi



dnl we rename some variables to avoid automake reporting them as being unused
INCLUDES_BASE=${BASE_INCLUDES}
AC_SUBST(INCLUDES_BASE)
LIBS_BASE=${BASE_LIBS}
AC_SUBST(LIBS_BASE)
LIBADD_BASE=${BASE_LIBADD}
AC_SUBST(LIBADD_BASE)
LDFLAGS_BASE=${BASE_LDFLAGS}
AC_SUBST(LDFLAGS_BASE)
DEPENDENCIES_BASE=${BASE_DEPENDENCIES}
AC_SUBST(DEPENDENCIES_BASE)
MODULES_BASE=${BASE_MODULES}
AC_SUBST(MODULES_BASE)
MODULE_ARGUMENTS_BASE=${BASE_MODULES_ARGUMENTS}
AC_SUBST(MODULE_ARGUMENTS_BASE)



dnl store information for uninstalled project
echo "DATETIME: `date --iso-8601=seconds`" > base-framework.dsc
echo "VERSION: 0.9.1" >> base-framework.dsc
echo "SRCDIR: ${srcdir}" >> base-framework.dsc
echo "INCLUDES: . ${srcdir}" >> base-framework.dsc
echo "CC: ${CC}" >> base-framework.dsc
echo "CFLAGS: ${CFLAGS}" >> base-framework.dsc
echo "CXX: ${CXX}" >> base-framework.dsc
echo "CXXFLAGS: ${CXXFLAGS}" >> base-framework.dsc
echo "LDFLAGS: ${LDFLAGS}" >> base-framework.dsc
echo "LIBDIR: base" >> base-framework.dsc
echo "LIBS: -lbase" >> base-framework.dsc
echo "STATIC LIBS: " >> base-framework.dsc



dnl store probed and/or static configuration
echo "dumping probed (final) target configuration..."

# functions
available_functions=
if test x$base_cv_function_mbrtowc = xyes; then
  available_functions=${available_functions}${available_functions:+ }mbrtowc
fi
if test x$base_cv_function_mbsrtowcs = xyes; then
  available_functions=${available_functions}${available_functions:+ }mbsrtowcs
fi
if test x$base_cv_function_memchr = xyes; then
  available_functions=${available_functions}${available_functions:+ }memchr
fi
if test x$base_cv_function_memcmp = xyes; then
  available_functions=${available_functions}${available_functions:+ }memcmp
fi
if test x$base_cv_function_memcpy = xyes; then
  available_functions=${available_functions}${available_functions:+ }memcpy
fi
if test x$base_cv_function_memmove = xyes; then
  available_functions=${available_functions}${available_functions:+ }memmove
fi
if test x$base_cv_function_memset = xyes; then
  available_functions=${available_functions}${available_functions:+ }memset
fi
if test x$base_cv_function_nanosleep = xyes; then
  available_functions=${available_functions}${available_functions:+ }nanosleep
fi
if test x$base_cv_function_pselect = xyes; then
  available_functions=${available_functions}${available_functions:+ }pselect
fi
if test x$base_cv_function_pthread_yield = xyes; then
  available_functions=${available_functions}${available_functions:+ }pthread_yield
fi
if test x$base_cv_function_readdir_r = xyes; then
  available_functions=${available_functions}${available_functions:+ }readdir_r
fi
if test x$base_cv_function_yield = xyes; then
  available_functions=${available_functions}${available_functions:+ }yield
fi

# apis
available_apis=
if test x$base_cv_api_aio = xyes; then
  available_apis=${available_apis}${available_apis:+ }aio
fi
if test x$base_cv_api_inet_ipv6 = xyes; then
  available_apis=${available_apis}${available_apis:+ }inet_ipv6
fi
if test x$base_cv_api_lfs = xyes; then
  available_apis=${available_apis}${available_apis:+ }lfs
fi
if test x$base_cv_api_pthread = xyes; then
  available_apis=${available_apis}${available_apis:+ }pthread
fi
if test x$base_cv_api_pthread_rwlock = xyes; then
  available_apis=${available_apis}${available_apis:+ }pthread_rwlock
fi
if test x$base_cv_api_pthread_semaphore = xyes; then
  available_apis=${available_apis}${available_apis:+ }pthread_semaphore
fi
if test x$base_cv_api_regexp = xyes; then
  available_apis=${available_apis}${available_apis:+ }regexp
fi
if test x$base_cv_api_wide = xyes; then
  available_apis=${available_apis}${available_apis:+ }wide
fi

cat > "${ac_base_target-target}.probed" << EOF
flavor=$base_cv_flavor

# architecture
arch=$base_cv_arch
arch_minor=$base_cv_arch_minor

# primitive type
unsigned_char=$base_cv_unsigned_char

# size of primitive
char_size=$base_cv_char_size
short_size=$base_cv_short_size
int_size=$base_cv_int_size
long_size=$base_cv_long_size
long_long_size=$base_cv_long_long_size
wchar_size=$base_cv_wchar_size
pointer_size=$base_cv_pointer_size

# byte order
byte_order=$base_cv_byte_order
word_order=$base_cv_word_order

# floating-point representation
float=$base_cv_float
double=$base_cv_double
long_double=$base_cv_long_double

# os
os=$base_cv_os
os_minor=$base_cv_os_minor

# functions
functions="$available_functions"

# apis
apis="$available_apis"
EOF



dnl store the public configuration
ac_base_header=base/configuration.h
if test -f $ac_base_header && diff -q $ac_base_public_config $ac_base_header >&AC_FD_CC 2>&AC_FD_CC; then
  echo "public configuration is already up-to-date..."
else
  echo "updating public configuration..."
  mkdir -p base
  mv $ac_base_public_config $ac_base_header
fi



dnl store the private configuration
ac_base_header=base/platforms/configuration.h
if test -f $ac_base_header && diff -q $ac_base_private_config $ac_base_header >&AC_FD_CC 2>&AC_FD_CC; then
  echo "private configuration is already up-to-date..."
else
  if grep -v ${base_symbol_prefix}BUILD_DATE $ac_base_private_config > private.new.tmp 2>&AC_FD_CC &&
     grep -v ${base_symbol_prefix}BUILD_DATE $ac_base_header > private.old.tmp 2>&AC_FD_CC &&
     diff -q private.new.tmp private.old.tmp >&AC_FD_CC 2>&AC_FD_CC; then
    echo "private configuration not updated because only DATE would change"
  else
    echo "updating private configuration header..."
    mkdir -p base/platforms
    mv $ac_base_private_config $ac_base_header
  fi
  test -f private.new.tmp && rm private.new.tmp
  test -f private.old.tmp && rm private.old.tmp
fi



dnl generate make files
AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(config/Makefile)
AC_CONFIG_FILES(static/Makefile)

AC_CONFIG_FILES(base/Makefile)
AC_CONFIG_FILES(base/docs/Makefile)
AC_CONFIG_FILES(base/docs/en/Makefile)
AC_CONFIG_FILES(base/platforms/Makefile)
AC_CONFIG_FILES(base/archive/Makefile)
AC_CONFIG_FILES(base/collection/Makefile)
AC_CONFIG_FILES(base/communication/Makefile)
AC_CONFIG_FILES(base/compression/Makefile)
AC_CONFIG_FILES(base/concurrency/Makefile)
AC_CONFIG_FILES(base/extension/Makefile)
AC_CONFIG_FILES(base/filesystem/Makefile)
AC_CONFIG_FILES(base/iterator/Makefile)
AC_CONFIG_FILES(base/dl/Makefile)
AC_CONFIG_FILES(base/io/Makefile)
AC_CONFIG_FILES(base/io/async/Makefile)
AC_CONFIG_FILES(base/mathematics/Makefile)
AC_CONFIG_FILES(base/mem/Makefile)
AC_CONFIG_FILES(base/net/Makefile)
AC_CONFIG_FILES(base/rmi/Makefile)
AC_CONFIG_FILES(base/security/Makefile)
AC_CONFIG_FILES(base/sound/Makefile)
AC_CONFIG_FILES(base/string/Makefile)
AC_CONFIG_FILES(base/xml/Makefile)

AC_CONFIG_FILES(base/opengl/Makefile)
AC_CONFIG_FILES(base/ui/Makefile)

AC_OUTPUT()
