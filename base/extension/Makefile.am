# ***************************************************************************
#    The Base Framework
#    A framework for developing platform independent applications
#
#    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
#
#    This framework is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#    For the licensing terms refer to the file 'LICENSE'.
# ***************************************************************************

include $(top_srcdir)/common.am

AUTOMAKE_OPTIONS += no-dependencies

noinst_LTLIBRARIES = libextension.la

BUILT_SOURCES = extension.h

sectiondir = $(includedir)/base/extension

if EXTENSIONS

include extension.am

${srcdir}/Makefile.am: extension.am

extension.h extension.am:
	${srcdir}/findextensions.sh ${srcdir}

else

libextension_la_SOURCES =
nobase_section_HEADERS = extension.h

extension.h: ${srcdir}/Makefile.in
	@echo Generating dummy extension header file
	@echo "/* This is an automatically generated file. Do NOT edit. */" > extension.h
	@echo "/* Extensions have not been enabled */" >> extension.h

extension.am:
	@echo Generating dummy extension automake file
	@echo "# This is an automatically generated file. Do NOT edit." > extension.am
	@echo "# Extensions have not been enabled" >> extension.am

endif # EXTENSIONS
