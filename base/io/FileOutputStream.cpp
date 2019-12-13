/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FileOutputStream::FileOutputStream()
{
}

FileOutputStream::FileOutputStream(
  const String& path,
  unsigned int options,
  unsigned int permissions) throw(FileNotFound)
  : file(path, File::WRITE, options)
{
}

FileOutputStream::FileOutputStream(
  const FileOutputStream& copy) noexcept : file(copy.file) {
}

bool FileOutputStream::open(
  const String& path,
  unsigned int options,
  unsigned int permissions) throw(FileNotFound)
{
  file = File(path, File::WRITE, options);
  return true;
}

FileOutputStream& FileOutputStream::operator=(const FileOutputStream& assign) noexcept {
  file = assign.file;
  return *this;
}

bool FileOutputStream::isOpen() const noexcept
{
  return !file.isClosed();
}

void FileOutputStream::close() throw(IOException) {
  file.close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
