/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileOutputStream::FileOutputStream(const String& path, unsigned int options, unsigned int permissions) throw(FileNotFound) :
  file(path, File::WRITE, options) {
}

FileOutputStream::FileOutputStream(const FileOutputStream& copy) throw() : file(copy.file) {
}

FileOutputStream& FileOutputStream::operator=(const FileOutputStream& eq) throw() {
  file = eq.file;
  return *this;
}

void FileOutputStream::close() throw(IOException) {
  file.close();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
