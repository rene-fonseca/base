/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/EndOfFile.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FileInputStream::FileInputStream(
  const String& path, bool exclusive)
  : file(path, File::READ, exclusive ? File::EXCLUSIVE : 0), end(false) {
}

unsigned int FileInputStream::available() const {
  long long size = file.getSize();
  long long position = file.getPosition();
  if (position >= size) {
    return 0;
  }
  return (unsigned int)minimum<long long>(
    size - position,
    PrimitiveTraits<unsigned int>::MAXIMUM
  );
}

void FileInputStream::close() {
  file.close();
}

unsigned int FileInputStream::skip(unsigned int count) {
  file.setPosition(count, File::CURRENT); // may skip past end of file
  return count;
}

void FileInputStream::wait() const {
  // data is always available?
}

bool FileInputStream::wait(unsigned int timeout) const {
  // data is always available?
  return true;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
