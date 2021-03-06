# ***************************************************************************
#    The Base Framework
#    A framework for developing platform independent applications
#
#    See COPYRIGHT.txt for details.
#
#    This framework is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#    For the licensing terms refer to the file 'LICENSE'.
# ***************************************************************************

# Problems:
#   The library will use the demangling scheme associated with the compiler
#   used to build the package.

Name: base-framework
Version: 0.9.1
Release: 1
License: GPL
Source: %{name}-%{version}.tar.bz2
Url: https://dev.azure.com/renefonseca/base
Vendor: Rene Moeller Fonseca
Packager: Rene Moeller Fonseca
Distribution: The Base Framework
Prefix: /usr
BuildRoot: %{_builddir}/%{name}-%{version}-%{release}-root



Summary: Runtime support provided by the Base framework
Group: System Environment/Libraries
Requires: libstdc++, pcre, libxml2, glibc
%description
This particular package provides the runtime support needed by applications
which are rely on the Base framework. If you want to develop your own
applications using this framework you need the dedicated development package
which should accompany this package.



%package devel
Summary: Development support for the Base framework
Group: Development/Libraries
Requires: %{name} = %{version}, libstdc++-devel, pcre-devel, libxml2-devel, glibc-devel

%description devel
This package provides the files required for developing applications using the
Base framework. The framework provides a common base, hence the name, for
developing platform independent applications using the C++ programming language
with strict conventions applied. The framework has been designed and implemented
to remove many of the concerns normally imposed on the developer(s) by "legacy"
software.



%prep
%setup
CFLAGS="${RPM_OPT_FLAGS}" \
CXXFLAGS="${RPM_OPT_FLAGS}" \
LDFLAGS=-L/usr/X11R6/lib \
./configure \
  --target=%{_target} \
  --prefix=%{_prefix} \
  --enable-regexp=pcre \
  --enable-xml=xmlsoft \
  --enable-xslt=xmlsoft



%build
processors=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "${processors}" -gt "1" ]; then
  make -j${processors} "MAKE=make -j${processors}"
else
  make
fi



%install
make DESTDIR=%{buildroot} install-strip



# the main package
%files
%attr(-,root,root) %{prefix}/lib/libbase.so*



# development package
%files devel
# package all header files
%attr(-,root,root) %{prefix}/include
# package all static libraries
%attr(-,root,root) %{prefix}/lib/libbase.a
# libtool libraries
%attr(-,root,root) %{prefix}/lib/libbase.la



%clean
# clean up root (may be overwritten by rpm configuration file!)
[ -n "%{buildroot}" -a "%{buildroot}" != "/" ] && rm -rf %{buildroot}

# clean up source tree
rm -rf %{_builddir}/%{name}
