/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/io/FileDescriptorInputStream.h>

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
