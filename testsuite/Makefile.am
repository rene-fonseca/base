# ***************************************************************************
#    The Base Framework (Test Suite)
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

LIBS=-lbase

bin_PROGRAMS = Array List Queue Set Map String io client server datagramClient datagramServer Thread referenceCounting fileInput mtServer filesystem xml daemon Exception ftp http dlapp intercom Version SerialPort File floatingPoint
bin_PROGRAMS += Process

#lib_LTLIBRARIES = dlmodule.la

Array_SOURCES = Array.cpp
List_SOURCES = List.cpp
Queue_SOURCES = Queue.cpp
Set_SOURCES = Set.cpp
Map_SOURCES = Map.cpp
String_SOURCES = String.cpp
io_SOURCES = io.cpp
client_SOURCES = client.cpp
server_SOURCES = server.cpp
datagramClient_SOURCES = datagramClient.cpp
datagramServer_SOURCES = datagramServer.cpp
Thread_SOURCES = Thread.cpp
referenceCounting_SOURCES = referenceCounting.cpp
fileInput_SOURCES = fileInput.cpp
mtServer_SOURCES = mtServer.cpp
filesystem_SOURCES = filesystem.cpp
xml_SOURCES = xml.cpp
daemon_SOURCES = daemon.cpp
Exception_SOURCES = Exception.cpp
ftp_SOURCES = ftp.cpp
http_SOURCES = http.cpp
dlapp_SOURCES = dlapp.cpp MyLinkerModule.cpp
intercom_SOURCES = intercom.cpp
Version_SOURCES = Version.cpp
SerialPort_SOURCES = SerialPort.cpp
File_SOURCES = File.cpp
floatingPoint_SOURCES = floatingPoint.cpp
Process_SOURCES = Process.cpp

#dlmodule_la_SOURCES = dlmodule.cpp MyLinkerModule.cpp
#dlmodule_la_LDFLAGS = -avoid-version -module -no-undefined -version-info 0:1:0

EXTRA_DIST =
