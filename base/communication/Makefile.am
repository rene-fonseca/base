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

noinst_LTLIBRARIES = libcommunication.la

libcommunication_la_SOURCES = CommunicationsException.cpp CommunicationsPort.cpp EUI64.cpp IEEE1394.cpp IEEE1394Exception.cpp SerialPort.cpp 

sectiondir = $(includedir)/base/communication
section_HEADERS = CommunicationsException.h CommunicationsPort.h EUI64.h IEEE1394.h IEEE1394Exception.h SerialPort.h
