/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileOutputStream.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // Unix
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileOutputStream::FileOutputStream(const String& p, unsigned int options, unsigned int permissions) throw(FileNotFound) :
  file(p, File::WRITE, options) {
}

FileOutputStream::FileOutputStream(const FileOutputStream& copy) throw() : file(copy.file) {
}

FileOutputStream& FileOutputStream::operator=(const FileOutputStream& eq) throw() {
  if (&eq == this) {
    file = eq.file;
  }
  return *this;
}

void FileOutputStream::close() throw(IOException) {
  file.close();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
