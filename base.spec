# ***************************************************************************
#    The Base Framework
#    A framework for developing platform independent applications
#
#    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
#
#    This framework is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#    For the licensing terms refer to the file 'LICENSE'.
# ***************************************************************************

Summary: A framework for developing platform independent applications in C++.
Name: base-framework
Version: 0.9.1
Release: 1
License: GPL
Group: Development/Libraries
Source: http://www.mip.sdu.dk/~fonseca/base/src/%{name}-%{version}.tar.gz
Url: http://www.mip.sdu.dk/~fonseca/base
Vendor: Rene Moeller Fonseca <fonseca@mip.sdu.dk>
Packager: Rene Moeller Fonseca <fonseca@mip.sdu.dk>
Distribution: The Base Framework

%description
A framework for developing platform independent applications in C++.



#%package examples
#Summary: Sample applications using the Base framework.
#Group: Development/Libraries
#%description examples
#This package contains sample applications which illustrate the use of the Base
#framework.



Prefix: /usr
BuildRoot: %{_builddir}/%{name}-%{version}-%{release}-root



%prep
%setup
mv base/test testsuite
CFLAGS="${RPM_OPT_FLAGS}" CXXFLAGS="${RPM_OPT_FLAGS}" ./configure \
  --target=%{_target} \
  --prefix=%{prefix} \
  --enable-demangle=v3mv \
  --enable-regexp=pcre \
  --enable-xml=xmlsoft



%build
processors=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "${processors}" -gt "1" ]; then
  make -j${processors} "MAKE=make -j${processors}"
else
  make
fi



%install
make DESTDIR=${RPM_BUILD_ROOT} install-strip

cd ${RPM_BUILD_ROOT}
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > ${RPM_BUILD_DIR}/%{name}.file.list
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >> ${RPM_BUILD_DIR}/%{name}.file.list
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> ${RPM_BUILD_DIR}/%{name}.file.list

#make -C testsuite DESTDIR=${RPM_BUILD_ROOT} install-strip

#find %{prefix}/bin -type f | sed 's,^\.,\%attr(-\,root\,root) ,' > ${RPM_BUILD_DIR}/%{name}.examples.file.list



%clean
rm -rf ../%{name}.file.list
rm -rf ${RPM_BUILD_DIR}/%{name}
[ -n "${RPM_BUILD_ROOT}" -a "${RPM_BUILD_ROOT}" != "/" ] && rm -rf ${RPM_BUILD_ROOT}



%files -f ../%{name}.file.list

#%files examples -f ../%{name}.examples.file.list
