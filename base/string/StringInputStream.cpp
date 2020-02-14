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

StringInputStream::StringInputStream(String& _string)
  : string(_string)
{
  index = 0;
  eof = false;
  closed = false;
}

unsigned int StringInputStream::available() const
{
  bassert(!closed, IOException(this));
  return static_cast<unsigned int>(string.getLength() - index);
}

void StringInputStream::close()
{
  bassert(!closed, IOException(this));
  string = String();
  closed = true;
}

unsigned int StringInputStream::read(uint8* buffer, unsigned int size, bool nonblocking)
{
  bassert(!closed, IOException(this));
  if (index >= string.getLength()) {
    bassert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (size >= string.getLength() - index) {
    size = static_cast<unsigned int>(string.getLength() - index);
  }
  base::copy<uint8>(buffer, string.getBytes(), size);
  return size;
}

unsigned int StringInputStream::skip(unsigned int count)
{
  bassert(!closed, IOException(this));
  if (index >= string.getLength()) {
    bassert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (count >= string.getLength() - index) {
    count = static_cast<unsigned int>(string.getLength() - index);
  }
  index += count;
  return count;
}

StringInputStream::~StringInputStream()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
