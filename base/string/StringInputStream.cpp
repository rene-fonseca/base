/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/StringInputStream.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

StringInputStream::StringInputStream(String& _string) throw(BindException)
  : string(_string)
{
  index = 0;
  eof = false;
  closed = false;
}

unsigned int StringInputStream::available() const throw(IOException)
{
  bassert(!closed, IOException(this));
  return string.getLength() - index;
}

void StringInputStream::close() throw(IOException)
{
  bassert(!closed, IOException(this));
  string = String();
  closed = true;
}

unsigned int StringInputStream::read(uint8* buffer, unsigned int size, bool nonblocking) throw(IOException)
{
  bassert(!closed, IOException(this));
  if (index >= string.getLength()) {
    bassert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (size >= string.getLength() - index) {
    size = string.getLength() - index;
  }
  copy<uint8>(buffer, Cast::pointer<const uint8*>(string.getBytes()), size);
  return size;
}

unsigned int StringInputStream::skip(unsigned int count) throw(IOException)
{
  bassert(!closed, IOException(this));
  if (index >= string.getLength()) {
    bassert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (count >= string.getLength() - index) {
    count = string.getLength() - index;
  }
  index += count;
  return count;
}

StringInputStream::~StringInputStream()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
