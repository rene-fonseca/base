/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/string/FormatInputStream.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/Trace.h>

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

FormatInputStream::~FormatInputStream() {
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
