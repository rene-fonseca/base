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

include $(top_srcdir)/common.am

# the "platforms" directory must be built before all other source
SUBDIRS = docs platforms archive collection communication compression concurrency dl extension filesystem io iterator mathematics mem net rmi security sound string xml

lib_LTLIBRARIES = libbase.la

libbase_la_SOURCES = AnyValue.cpp Character.cpp Construct.cpp Date.cpp DateException.cpp Destruct.cpp DeviceException.cpp Dimension.cpp Exception.cpp FloatingPoint.cpp Integer.cpp UnsignedInteger.cpp LongInteger.cpp UnsignedLongInteger.cpp MemoryException.cpp Object.cpp OutOfBounds.cpp OutOfDomain.cpp OutOfRange.cpp Overflow.cpp Primitives.cpp Range2D.cpp ResourceException.cpp ShortInteger.cpp Underflow.cpp Trace.cpp Timer.cpp Type.cpp TypeInfo.cpp Application.cpp SingletonException.cpp Daemon.cpp SystemLogger.cpp NotImplemented.cpp NotSupported.cpp UnexpectedFailure.cpp Version.cpp OperatingSystem.cpp SystemInformation.cpp ProcessPreinitialization.cpp initialization.cpp

sectiondir = $(includedir)/base
section_HEADERS = configuration.h features.h AnyValue.h Dimension.h Exception.h FloatingPoint.h Object.h Character.h Range2D.h ShortInteger.h Iterator.h Trace.h RandomIterator.h ForwardIterator.h BidirectionalIterator.h Comparable.h Copyable.h Integer.h UnsignedInteger.h LongInteger.h UnsignedLongInteger.h Overflow.h Underflow.h OutOfRange.h OutOfBounds.h OutOfDomain.h Primitives.h ResourceException.h MemoryException.h DeviceException.h Construct.h Destruct.h Base.h Functor.h Date.h DateException.h Listener.h Timer.h Type.h TypeInfo.h ByteOrder.h Application.h SingletonException.h Daemon.h SystemLogger.h NotImplemented.h NotSupported.h UnexpectedFailure.h Version.h OperatingSystem.h SystemInformation.h ProcessPreinitialization.h

libbase_la_LIBADD = archive/libarchive.la collection/libcollection.la communication/libcommunication.la compression/libcompression.la concurrency/libconcurrency.la filesystem/libfilesystem.la io/libio.la io/async/libaio.la iterator/libiterator.la mathematics/libmathematics.la mem/libmem.la net/libnet.la platforms/libplatform.la string/libstring.la dl/libdl.la xml/libxml.la rmi/librmi.la security/libsecurity.la sound/libsound.la @LIBADD_BASE@ @LIBS_BASE@

libbase_la_LDFLAGS = -no-undefined -version-info 0:0:0 @LDFLAGS_BASE@

libbase_la_DEPENDENCIES = archive/libarchive.la collection/libcollection.la communication/libcommunication.la compression/libcompression.la concurrency/libconcurrency.la filesystem/libfilesystem.la io/libio.la io/async/libaio.la iterator/libiterator.la mathematics/libmathematics.la mem/libmem.la net/libnet.la platforms/libplatform.la string/libstring.la dl/libdl.la xml/libxml.la security/libsecurity.la sound/libsound.la @DEPENDENCIES_BASE@

# Windows version resource
# TAG: move to platform specific directory
winrcver.o: base.rc features.h
	cat $< | windres --preprocessor "${CXXCPP} ${AM_CPPFLAGS} ${CPPFLAGS}" -o $@ --output-format coff
