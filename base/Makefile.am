####### kdevelop will overwrite this part!!! (begin)##########
####### kdevelop will overwrite this part!!! (end)############
DEFS =
DEFAULT_INCLUDES =

SUBDIRS = concurrency filesystem mathematics io iterator string mem net dl collection xml security sound

lib_LTLIBRARIES = libbase.la

libbase_la_SOURCES = Character.cpp Construct.cpp Date.cpp DateException.cpp Destruct.cpp DeviceException.cpp Dimension.cpp Exception.cpp Integer.cpp UnsignedInteger.cpp LongInteger.cpp UnsignedLongInteger.cpp MemoryException.cpp Object.cpp OutOfBounds.cpp OutOfDomain.cpp OutOfRange.cpp Overflow.cpp Range2D.cpp ResourceException.cpp ShortInteger.cpp Underflow.cpp Trace.cpp Timer.cpp TypeInfo.cpp Application.cpp SingletonException.cpp Daemon.cpp SystemLogger.cpp FloatingPoint.cpp NotImplemented.cpp NotSupported.cpp Version.cpp OperatingSystem.cpp

sectiondir = $(includedir)/base
section_HEADERS = configuration.h features.h Dimension.h Exception.h Object.h Character.h Range2D.h ShortInteger.h Iterator.h Trace.h RandomIterator.h ForwardIterator.h BidirectionalIterator.h Comparable.h Copyable.h Integer.h UnsignedInteger.h LongInteger.cpp UnsignedLongInteger.h Overflow.h Underflow.h OutOfRange.h OutOfBounds.h OutOfDomain.h ResourceException.h MemoryException.h DeviceException.h Construct.h Destruct.h Base.h Functor.h Date.h DateException.h Listener.h Timer.h Type.h TypeInfo.h ByteOrder.h Application.h SingletonException.h Daemon.h SystemLogger.h NotImplemented.h NotSupported.h Version.h OperatingSystem.h

libbase_la_LIBADD = collection/libcollection.la concurrency/libconcurrency.la filesystem/libfilesystem.la io/libio.la iterator/libiterator.la mathematics/libmathematics.la mem/libmem.la net/libnet.la string/libstring.la dl/libdl.la xml/libxml.la security/libsecurity.la sound/libsound.la @BASE_ADDITIONAL_LIBADD@ @BASE_LIBS@

libbase_la_LDFLAGS = -avoid-version -module -no-undefined -version-info 0:9:0 @BASE_ADDITIONAL_LDFLAGS@

libbase_la_DEPENDENCIES = @BASE_ADDITIONAL_DEPENDENCIES@

winrcver.o: base.rc
	windres --include-dir .. --output-format coff $< $@
