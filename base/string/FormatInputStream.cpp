/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorInputStream standardInputStream(FileDescriptor::getStandardInput());
FormatInputStream fin(standardInputStream);

FormatInputStream::FormatInputStream(InputStream& in, unsigned int size) throw(BindException) :
  BufferedInputStream(in, size) {
}

char FormatInputStream::getCharacter() throw(IOException) {
  char buffer;
  read(&buffer, sizeof(buffer));
  return buffer;
}

FormatInputStream::~FormatInputStream() throw(IOException) {
  TRACE_MEMBER();
}

FormatInputStream& operator>>(FormatInputStream& stream, bool& value) {
//  char* buffer = stream.peek(5);
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, char& value) {
  value = stream.getCharacter();
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, char*& value) {
//  char* buffer = stream.readUntil("\n");
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
