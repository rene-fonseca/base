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

DEFS =
AUTOMAKE_OPTIONS = 1.4 foreign nostdinc dist-bzip2 dist-zip

SUBDIRS = config base # testsuite

dist-hook:
# generate doxygen documentation

module-testsuite-configure:
	@if test ! -f testsuite/Makefile; then \
	  echo "Configuring test suite..."; \
	  MODULE_CC="@CC@"; \
	  MODULE_CFLAGS="@CFLAGS@"; \
	  MODULE_CXX="@CXX@"; \
	  MODULE_CXXFLAGS="@CXXFLAGS@"; \
	  MODULE_CPPFLAGS="@CPPFLAGS@"; \
	  MODULE_LDFLAGS="@LDFLAGS@"; \
	  mkdir -p testsuite; \
	  if (cd testsuite; \
	      export CC="$${MODULE_CC}"; \
	      export CFLAGS="$${MODULE_CFLAGS}"; \
	      export CXX="$${MODULE_CXX}"; \
	      export CXXFLAGS="$${MODULE_CXXFLAGS}"; \
	      export CPP="$${MODULE_CPP}"; \
	      export CXXCPP="$${MODULE_CXXCPP}"; \
	      export CPPFLAGS="-I.. -I../${srcdir}$${MODULE_CPPFLAGS:+ }$${MODULE_CPPFLAGS}"; \
	      export LDFLAGS="-L../base$${MODULE_LDFLAGS:+ }$${MODULE_LDFLAGS}"; \
	      ../${srcdir}/testsuite/configure @BASE_MODULE_ARGUMENTS@); then \
	    echo "Test suite has been configured"; \
	  else \
	    echo "Test suite configuration failed"; \
	    exit 1; \
	  fi \
	fi
.phony: module-testsuite-configure

module-testsuite-build: module-testsuite-configure
	@echo "Building test suite"
	@if test -f testsuite/Makefile; then \
	  make -C testsuite; \
	fi
.phony: module-testsuite-build

testsuite: module-testsuite-build
.phony: testsuite

EXTRA_DIST = base.kdevprj AUTHORS COPYING ChangeLog INSTALL README TODO base.lsm common.am base.spec
